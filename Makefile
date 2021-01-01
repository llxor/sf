CC = clang
CFLAGS = -O3 -s
SRC = $(wildcard *.c)

default:; $(CC) $(SRC) -o main $(CFLAGS) 
