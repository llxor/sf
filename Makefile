CC = clang
CFLAGS = -O3 -s -Wextra -lncurses
DEBUG = -Wextra -fsanitize=undefined -lncurses

default:; $(CC) src/main.c -o sf $(CFLAGS)
debug:; $(CC) src/main.c -o sf $(DEBUG)

install: default; sudo mv sf /usr/bin/
uninstall:; sudo rm /usr/bin/sf
