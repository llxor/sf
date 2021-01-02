CC = clang
CFLAGS = -O3 -s -Wall -Wextra

default:; $(CC) main.c -o main $(CFLAGS) 
