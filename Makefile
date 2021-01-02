CC = clang
CFLAGS = -O3 -s -Wall -Wextra -Isrc -lncurses

default:; $(CC) src/main.c -o sf $(CFLAGS)
format:; clang-format -i src/*.c src/*.h

install: default; sudo mv sf /usr/bin/
uninstall:; sudo rm /usr/bin/sf
