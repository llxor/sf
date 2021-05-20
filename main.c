#include "config.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORMAL    1
#define HIGHLIGHT 2
#define EXIT_MSG  3

struct error_t
{
	char file[100], msg[500];
	int line, col, off;
};

static void concat(char **dst, const char *src)
{
	while (*src)
	{
		**dst = *src;
		*dst += 1;
		 src += 1;
	}
}

static struct error_t ERR_BUFF[MAX_ERR];
static int ERR_COUNT = 0;
static int EXIT_CODE = 0;

static void parse(const char *cmd)
{
	FILE *proc = popen(cmd, "r");
	if (proc == NULL)
	{
		puts("error process failed to start");
		exit(-1);
	}

	ERR_COUNT = 0;
	char buffer[1000] = {0};
	#define ERROR (ERR_BUFF[ERR_COUNT])

	while (ERR_COUNT < MAX_ERR && fgets(buffer, sizeof buffer, proc))
	{
		ERROR.line = ERROR.col = -1;

		sscanf(buffer, "%[^:]:%d:%[^\n]",
			ERROR.file, &ERROR.line, ERROR.msg);

		if (ERROR.line != -1)
		{
			char *end = ERROR.msg;
			ERROR.col = strtol(end, &end, 10);
			ERROR.off = end - (char *)ERROR.msg + (*end == ':');
			ERR_COUNT += 1;
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

	while ((c = *read))
	{
		read += 1;

		if (c == '%')
		{
			switch (*read)
			{
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

			read += 1;
		}

		else {
			*write = c;
			write += 1;
		}
	}

	if (system(cmd))
	{
		endwin();
		printf("editor failed to open file `%s`\n", ERROR.file);
		exit(-1);
	}

	#undef ERROR
}

static void render(const int selected, const int width)
{
	clear();

	for (int i = 0; i < ERR_COUNT; i += 1)
	{
		char buffer[width + 1];
		#define ERROR (ERR_BUFF[i])

		int off = snprintf(buffer, sizeof(buffer), " %s:%d:%d:%s",
				   ERROR.file, ERROR.line, ERROR.col,
				   ERROR.msg + ERROR.off);

		if (off > width)
		{
			off = width;
		}

		char display[width];
		memset(display, ' ', width);
		int count = 0;

		for (int i = 0; i < off; i += 1)
		{
			if (buffer[i] == '\t')
			{
				for (int j = 0; j < TABSTOP; j += 1)
				{
					display[count] = ' ';
					count += 1;
				}
			}

			else {
				display[count] = buffer[i];
				count += 1;
			}
		}

                int attrib = 0;
                if (selected == i) attrib = COLOR_PAIR(HIGHLIGHT);

		attron(attrib);
		printw("%.*s", width, display);
		attroff(attrib);
	}

	attron(COLOR_PAIR(EXIT_MSG));
	printw("\n [ exit code %d ]\n", EXIT_CODE);
	attroff(COLOR_PAIR(EXIT_MSG));
}

static int main_gui()
{
	WINDOW *win = initscr();
	keypad(stdscr, TRUE);
	start_color();
	curs_set(0);
	noecho();

	init_pair(NORMAL, FOREGROUND_COLOR, BACKGROUND_COLOR);
        init_pair(HIGHLIGHT, HIGHLIGHT_COLOR, BACKGROUND_COLOR);
	init_pair(EXIT_MSG, EXIT_CODE_COLOR, BACKGROUND_COLOR);

	wbkgd(win, COLOR_PAIR(NORMAL));

	const int width = getmaxx(win);
	int selected = 0;

	do {
		render(selected, width);
		int key = getch();

		switch (key)
		{
		case 'k':
			case KEY_UP:
			selected -= 1;
			break;

		case 'j':
			case KEY_DOWN:
			selected += 1;
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
	}
	while (1);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		puts("USAGE: sf [command]\n\n"
		     "sf provides a simple frontend to command line tools.\n"
		     "Report bugs to ellxor@protonmail.ch");
		exit(0);
	}

	char cmd[1000] = {0};
	char *write = cmd;

	for (int i = 1; i < argc; i += 1)
	{
		concat(&write, argv[i]);
		*write = ' ';
		write += 1;
	}
	concat(&write, "2>&1");

	do {
		parse(cmd);
	}
	while (ERR_COUNT && main_gui() != -1);

	return EXIT_CODE;
}
