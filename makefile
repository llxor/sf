CC = clang
SRC = main.c config.h
FLAGS = -O3 -s -lncurses -Wall -Wextra -pedantic

INSTALL_PATH = ~/../usr/bin/ # termux install path

sf: $(SRC)
	$(CC) -o sf main.c $(FLAGS)

install: sf
	mv sf $(INSTALL_PATH)
