#import <ncurses.h>
#import <parser.h>
#import <string.h>
#import <utils.h>

void printh(int width, const char *msg) {
  char buffer[1000] = {};
  sprintf(buffer, "%s", msg);

  for (int i = 0; i < width; i++) {
    if (buffer[i] == '\0') {
      buffer[i] = ' ';
    }
  }

  attron(A_STANDOUT);
  printw(buffer);
  attroff(A_STANDOUT);
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
    sprintf(buffer, "%s:%d:%d: %s", e.file, e.line, e.col, e.msg);

    if (i == selected) {
      printh(W, buffer);
      printw("%s\n", e.context);
    } else {
      printw("%s\n", buffer);
    }
  }
}

void edit(struct error e) {
  char cmd[1000] = {};
  sprintf(cmd, "vim %s '+normal %dG%d|'", e.file, e.line, e.col);
  system(cmd);
}

void init(int N, struct error errors[N]) {
  int width = init_render();
  int selected = 0;

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
      edit(errors[selected]);

    case 'q':
      selected = -1;
      break;
    }
  }

  endwin();
}
