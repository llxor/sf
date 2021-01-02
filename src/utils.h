#import <stdio.h>
#import <stdlib.h>

const char *RED = "\e[31;1m", *BOLD = "\e[37;1m", *NORMAL = "\e[0m";

void error(const char *msg) {
  printf("%serror:%s %s\n%s", RED, BOLD, msg, NORMAL);
  exit(1);
}

void stradd(char *dst, const char *src, const char sep) {
  sprintf(dst, "%s%s%c", dst, src, sep);
}
