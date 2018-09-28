# Compiler info
CC	    = gcc
CFLAGS = -Wall

# Program info
NAME = ptaskr
MAIN = src/tasklist.c
DEPS = src/defs.h
OBJS = obj/io.o obj/menus.o obj/prompts.o obj/rendering.o obj/structs.o
LIBS = -lncurses

# Install info
INS_PRE = /usr
INS_BIN = $(INS_PRE)/bin
INS_LIB = $(INS_PRE)/lib
INS_MAN = $(INS_PRE)/share/man
INS_CNF = ~/.config/$(NAME)

bin/$(NAME): $(MAIN) $(DEPS) $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

obj/%.o: src/%.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf bin/*
	rm -rf obj/*

install:
	cp bin/$(NAME) $(INS_BIN)/$(NAME)
	install -g 0 -o 0 -m 0644 doc/$(NAME).1 $(INS_MAN)/man1
	gzip $(INS_MAN)/man1/$(NAME).1
	mkdir -m0777 $(INS_CNF)

uninstall:
	rm -rf $(INS_BIN)/$(NAME)
	rm -rf $(INS_MAN)/man1/$(NAME).1.gz
	rm -rf $(INS_CNF)