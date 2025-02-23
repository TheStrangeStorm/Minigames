.PHONY: all

all: rps

rps:
	@gcc -lm -o rps rps.c
