CC = clang
CFLAGS = -O3 -s -Wall -Wextra -I.

default:; $(CC) main.c -o main $(CFLAGS) 
format:; clang-format -i *.c *.h
