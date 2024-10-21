CC = gcc
CFLAGS = -Wall -lncurses
LDFLAGS = -I./include/ -lcurl -lncurses -lzip

SCRS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, build/%.o, $(SCRS))

all: dir bin/main run

dir:
	@mkdir -p build bin

bin/main: $(OBJS)
	@$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$(OBJS): build/%.o: src/%.c
	@$(CC) -c $^ -o $@ $(CFLAGS) $(LDFLAGS)

.PHONY: run clean
run: bin/main
	@bin/main

clean:
	@rm bin/*
	@rm build/*

debug:
	@echo $(SCRS)
	@echo $(OBJS)
