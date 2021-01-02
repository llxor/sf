#import <gui.h>
#import <parser.h>
#import <stdio.h>

int main(int argc, char **argv) {
  char cmd[1000] = {};
  buildcommand(cmd, argc, argv);

  struct error errors[20] = {};
  int len = parse(cmd, errors);

  startgui(len, errors);
}
