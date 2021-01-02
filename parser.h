#import <stdio.h>
#import <utils.h>

struct error {
  char file[100], msg[1000], context[3000];
  int line, col;
};

void buildcommand(char *cmd, int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    stradd(cmd, argv[i], ' ');
  }

  // capture stdout and stderr
  stradd(cmd, "2>&1", '\0');
}

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

    if (e.line == -1 || e.col == -1) {
      if (len) {
        stradd(errors[len - 1].context, buffer, '\0');
      }
    } else {
      errors[len++] = e;
    }
  }

  pclose(proc);
  return len;
}
