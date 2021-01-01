#import <stdio.h>
#import <errors.h>

void execute(const char *cmd) {
  FILE *proc = popen(cmd, "r");

  if (proc == NULL) {
    error("process failed to start");
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
