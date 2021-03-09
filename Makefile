CC = clang
CFLAGS = -O3 -s -Wextra -lncurses

default:; $(CC) src/main.c -o sf $(CFLAGS)
install: default; sudo mv sf /usr/bin/
uninstall:; sudo rm /usr/bin/sf
