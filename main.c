#import <parser.h>
#import <stdio.h>

int main(int argc, char **argv) {
  char cmd[1000] = {};
  buildcommand(cmd, argc, argv);

  struct error errors[20] = {};
  int len = parse(cmd, errors);

  for (int i = 1; i <= len; i++) {
    struct error e = errors[i];
    printf("%s:%d:%d: %s\n%s", e.file, e.line, e.col, e.msg, e.context);
  }
}
