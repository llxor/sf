#import <stdio.h>
#import <stdlib.h>
#import <stdarg.h>

const char *RED = "\e[31;1m",
           *BOLD = "\e[37;1m",
           *NORMAL = "\e[0m";

void error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  printf("%serror:%s ", RED, BOLD);
  vprintf(fmt, args);
  printf("\n%s", NORMAL);
  exit(1);
}

void execute(const char *cmd) {
  FILE *proc = popen(cmd, "r");

  if (proc == NULL) {
    error("command `%s` is invalid", cmd);
  }

  char buffer[1000];

  while (fgets(buffer, sizeof buffer, proc) != NULL) {
    printf("%s", buffer);
  }

  pclose(proc);
}

int main(int argc, char **argv) {
  // build command
  char cmd[1000] = "";

  for (int i = 1; i < argc; i++) {
    sprintf(cmd, "%s %s", cmd, argv[i]);
  }

  execute(cmd);
}
