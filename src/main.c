#import <gui.h>
#import <help.h>
#import <parser.h>
#import <stdio.h>

void buildcommand(char *cmd, int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    sprintf(cmd, "%s%s%c", cmd, argv[i], ' ');
  }
  sprintf(cmd, "%s 2>&1", cmd);
}

int main(int argc, char **argv) {
  char cmd[1000] = {};
  buildcommand(cmd, argc, argv);

  int exit = argc < 2;

  if (exit)
    display_help();

  while (!exit) {
    struct error errors[MAX_ERR] = {};
    int len = parse(cmd, errors);

    if (len == 0)
      break;

    exit = init(len, errors);
  }

  return 0;
}
