#include "config.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct error {
        char file[100], msg[500];
        int line, col, off;
};

static void concat(char **dst, const char *src)
{
        while (*src)
                *(*dst)++ = *src++;
}

static struct error ERR_BUFF[MAX_ERR] = {0};
static int ERR_COUNT = 0;
static int EXIT_CODE = 0;

static void parse(const char *cmd)
{
        FILE *proc = popen(cmd, "r");
        if (proc == NULL) {
                puts("error process failed to start");
                exit(-1);
        }

        ERR_COUNT = 0;
        char buffer[1000] = {0};
#define ERROR (ERR_BUFF[ERR_COUNT])

        while (ERR_COUNT < MAX_ERR && fgets(buffer, sizeof buffer, proc)) {
                ERROR.line = ERROR.col = -1;
                sscanf(buffer, "%[^:]:%d:%[^\n]", ERROR.file, &ERROR.line,
                       ERROR.msg);

                if (ERROR.line != -1) {
                        char *end = ERROR.msg;
                        ERROR.col = strtol(end, &end, 10);
                        ERROR.off = end - (char *)ERROR.msg + (*end == ':');
                        ERR_COUNT++;
                }
        }
#undef ERROR

        EXIT_CODE = WEXITSTATUS(pclose(proc));
}

static void edit(const int selected)
{
#define ERROR (ERR_BUFF[selected])

        char cmd[1000] = {0};
        char *write = cmd;

        const char *read = EDITOR;
        char c;

        while ((c = *read++)) {
                if (c == '%') {
                        switch (*read++) {
                        case 'f':
                                concat(&write, ERROR.file);
                                break;
                        case 'l':
                                write += sprintf(write, "%d", ERROR.line);
                                break;
                        case 'c':
                                write += sprintf(write, "%d", ERROR.col);
                                break;
                        }
                }

                else {
                        *write++ = c;
                }
        }

        if (system(cmd)) {
                endwin();
                printf("editor failed to open file %s\n", ERROR.file);
                exit(-1);
        };
#undef ERROR
}

static void render(const int selected, const int width)
{
        clear();

        for (int i = 0; i < ERR_COUNT; i++) {
                char buffer[width];
#define ERROR (ERR_BUFF[i])

                int off =
                    snprintf(buffer, width, "%s:%d:%d:%s", ERROR.file,
                             ERROR.line, ERROR.col, ERROR.msg + ERROR.off);

                char display[width];
                memset(display, ' ', width);
                int count = 0;

                for (int i = 0; i < off; i++) {
                        if (buffer[i] == '\t')
                                for (int j = 0; j < TABSTOP; j++)
                                        display[count++] = ' ';

                        else
                                display[count++] = buffer[i];
                }

                int attrib = (A_STANDOUT | A_BOLD) * (selected == i);

                attron(attrib);
                printw("%.*s", width, display);
                attroff(attrib);
        }

        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);

        attron(COLOR_PAIR(1) | A_BOLD);
        printw("[ exit code %d ]\n", EXIT_CODE);
        attroff(COLOR_PAIR(1) | A_BOLD);

        for (int i = 0; i < 10; i++)
                printw("\n");
}

static int main_gui()
{
        WINDOW *win = initscr();
        keypad(stdscr, TRUE);
        curs_set(0);
        noecho();

        int width = getmaxx(win);
        int selected = 0;

        do {
                render(selected, width);
                int key = getch();

                switch (key) {
                case 'k':
                case KEY_UP:
                        selected--;
                        break;

                case 'j':
                case KEY_DOWN:
                        selected++;
                        break;

                case '\n':
                        endwin();
                        edit(selected);
                        return 0;

                case 'q':
                        endwin();
                        return -1;
                }

                selected += (selected == -1);
                selected -= (selected == ERR_COUNT);
        } while (1);
}

int main(int argc, char **argv)
{
        if (argc < 2) {
                puts("USAGE: sf [command]\n\n"
                     "sf provides a simple frontend to command line tools.\n"
                     "Report bugs to https://github.com/ellxor/sf");
                exit(0);
        }

        char cmd[1000] = {0};
        char *write = cmd;

        for (int i = 1; i < argc; i++) {
                concat(&write, argv[i]);
                *write++ = ' ';
        }
        concat(&write, "2>&1");

        do {
                parse(cmd);
        } while (ERR_COUNT && main_gui() != -1);

        return EXIT_CODE;
}
