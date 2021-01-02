CC = clang
CFLAGS = -O3 -s -Wall -Wextra -Isrc -lncurses

default:; $(CC) src/main.c -o main $(CFLAGS)
format:; clang-format -i src/*.c src/*.h
