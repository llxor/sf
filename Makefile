CC = clang
CFLAGS = -O3 -s -Wall -Wextra
SRC = $(wildcard *.c)

default:; $(CC) $(SRC) -o main $(CFLAGS) 
