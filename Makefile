CC = gcc
CFLAGS = -O3 -s -Wall -Wextra -Isrc -lncurses

default:; $(CC) src/main.c -o sf $(CFLAGS)
format:; indent src/*.c src/*.h -linux && rm src/*~

install: default; sudo mv sf /usr/bin/
uninstall:; sudo rm /usr/bin/sf
