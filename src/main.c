#import <gui.h>
#import <help.h>
#import <parser.h>

int main(int argc, char **argv) {
  char cmd[1000] = {};
  buildcmd(cmd, argc, argv);

  int exit = argc < 2;

  if (exit) {
    display_help();
  }

  while (!exit) {
    struct error errors[MAX_ERR] = {};
    int len = parse(cmd, errors, argv[1]);

    if (len == 0) {
      break;
    }

    exit = init(len, errors);
  }

  return 0;
}
