.PHONY: all

CC=gcc
CFLAGS = -lm

all: $(patsubst %.c, %, $(wildcard *.c))

SRC=$(wildcard *.c)

%: %.c
	@$(CC) $(CFLAGS) -o $@ $<
