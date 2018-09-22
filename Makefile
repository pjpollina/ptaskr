# Compiler info
CC			= gcc
CFLAGS	= -Wall

# Program info
NAME = plistr
MAIN = src/tasklist.c
DEPS = src/defs.h
OBJS = obj/deltask.o obj/editor.o obj/io.o obj/menu.o obj/newtask.o obj/structs.o
LIBS = -lncurses

bin/$(NAME): $(MAIN) $(DEPS) $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

obj/%.o: src/%.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf bin/*
	rm -rf obj/*
	rm -rf data/*