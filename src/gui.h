#import <editor.h>
#import <ncurses.h>
#import <parser.h>

void printh(int width, const char *msg) {
  char buffer[1000] = {};
  sprintf(buffer, "%s", msg);

  for (int i = 0; i < width; i++) {
    if (buffer[i] == '\0') {
      buffer[i] = ' ';
    }
  }

  attron(A_STANDOUT | A_BOLD);
  printw(buffer);
  attroff(A_STANDOUT | A_BOLD);
}

int init_render() {
  WINDOW *win = initscr();

  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();

  int width, height;
  getmaxyx(win, height, width);

  return width;
}

void render(int selected, int W, int N, struct error errors[N]) {
  clear();

  for (int i = 0; i < N; i++) {
    struct error e = errors[i];

    char buffer[1000] = {};

    if (e.line == -1 && e.col == -1) {
      sprintf(buffer, "%s", e.msg);
    } else {
      sprintf(buffer, "%s:%d:%d: %s", e.file, e.line, e.col, e.msg);
    }

    if (i == selected) {
      printh(W, buffer);
    } else {
      printw("%s\n", buffer);
    }
  }

  printw("\nCompilation finished with exit code %d\n", exitcode);
}

int init(int N, struct error errors[N]) {
  int width = init_render();
  int selected = 0, exitcode = 0;

  while (selected != -1) {
    render(selected, width, N, errors);
    int key = getch();

    switch (key) {
    case KEY_UP:
      if (selected != 0)
        selected--;
      break;

    case KEY_DOWN:
      if (selected != N - 1)
        selected++;
      break;

    case '\n':
      if (errors[selected].line != -1 && errors[selected].col != -1) {
        load_command(errors[selected]);
        system(command);
        selected = -1;
      }
      break;

    case 'q':
      selected = -1;
      exitcode = 1;
      break;
    }
  }

  endwin();
  return exitcode;
}
