#import <config.h>
#import <parser.h>
#import <string.h>

char command[1000];

void load_command(struct error e) {
  command[0] = 0;

  for (const char *src = editor; *src != '\0'; src++) {
    if (*src == '[') {
      if (strncmp(src, "[file]", 6) == 0) {
        sprintf(command, "%s%s", command, e.file);
        src += 5;
      }

      if (strncmp(src, "[line]", 6) == 0) {
        sprintf(command, "%s%d", command, e.line);
        src += 5;
      }

      if (strncmp(src, "[col]", 5) == 0) {
        sprintf(command, "%s%d", command, e.col);
        src += 4;
      }
    }

    else {
      sprintf(command, "%s%c", command, *src);
    }
  }
}
