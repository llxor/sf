CC = clang
SRC = main.c config.h
FLAGS = -O3 -s -lncurses

sf: $(SRC)
	$(CC) -o sf main.c $(FLAGS)
