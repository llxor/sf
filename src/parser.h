#import <stdio.h>
#import <utils.h>

struct error {
  char file[100], msg[500];
  int line, col;
};

const int MAX_ERR = 20;

int parse(const char *cmd, struct error *errors) {
  FILE *proc = popen(cmd, "r");

  if (proc == NULL) {
    error("process failed to start");
  }

  char buffer[1000];
  int len = 0;

  while (fgets(buffer, sizeof buffer, proc) != NULL) {
    struct error e = {.line = -1, .col = -1};
    sscanf(buffer, "%[^:]:%d:%d: %[^\n]", e.file, &e.line, &e.col, e.msg);

    if (e.line != -1 && e.col != -1) {
      errors[len++] = e;
      if (len == MAX_ERR)
        break;
    }
  }

  pclose(proc);
  return len;
}
