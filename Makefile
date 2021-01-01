CC = clang
CFLAGS = -O3 -s -I. -Wall -Wextra
SRC = $(wildcard *.c)

default:; $(CC) $(SRC) -o main $(CFLAGS) 
