#import <stdio.h>
#import <stdlib.h>

const char *RED = "\e[31;1m", *BOLD = "\e[37;1m", *NORMAL = "\e[0m";

struct error {
  char file[100], msg[1000], context[3000];
  int line, col;
};

void error(const char *msg) {
  printf("%serror:%s %s\n%s", RED, BOLD, msg, NORMAL);
  exit(1);
}

void stradd(char *dst, char *src, char j) {
  sprintf(dst, "%s%s%c", dst, src, j);
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

    if (e.line != -1 && e.col != -1) {
      errors[++len] = e;
    } else {
      stradd(errors[len].context, buffer, '\0');
    }
  }

  pclose(proc);
  return len;
}

int main(int argc, char **argv) {
  // build command
  char cmd[1000] = {};
  for (int i = 1; i < argc; i++) {
    stradd(cmd, argv[i], ' ');
  }
  stradd(cmd, "2>&1", '\0');

  struct error errors[20] = {};
  int len = parse(cmd, errors);

  for (int i = 1; i <= len; i++) {
    struct error e = errors[i];
    printf("%s:%d:%d: %s\n%s", e.file, e.line, e.col, e.msg, e.context);
  }
}
