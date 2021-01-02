#import <gui.h>
#import <parser.h>
#import <stdio.h>

int main(int argc, char **argv) {
  char cmd[1000] = {};
  buildcommand(cmd, argc, argv);

  while (argc > 1) {
    struct error errors[20] = {};
    int len = parse(cmd, errors);

    if (len == 0)
      break;

    init(len, errors);
  }
}
