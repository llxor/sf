#import <gui.h>
#import <parser.h>
#import <stdio.h>

int main(int argc, char **argv) {
  char cmd[1000] = {};
  buildcommand(cmd, argc, argv);

  int exit = argc < 2;

  while (!exit) {
    struct error errors[MAX_ERR] = {};
    int len = parse(cmd, errors);

    if (len == 0)
      break;

    exit = init(len, errors);
  }
}
