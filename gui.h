#import <parser.h>

void startgui(int N, struct error errors[N]) {
  for (int i = 0; i < N; i++) {
    struct error e = errors[i];
    printf("%s:%d:%d: %s\n", e.file, e.line, e.col, e.msg);
  }
}
