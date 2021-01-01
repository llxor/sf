#import <stdio.h>
#import <stdlib.h>
#import <stdarg.h>

const char *RED = "\e[31;1m", *BOLD = "\e[37;1m", *NORMAL = "\e[0m";

void error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  printf("%serror:%s ", RED, BOLD);
  vprintf(fmt, args);
  printf("\n%s", NORMAL);
  exit(1);
}
