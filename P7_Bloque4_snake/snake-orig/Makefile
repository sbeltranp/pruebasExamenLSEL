CC=$(PREFIX)gcc
CFLAGS += -g -O0

all: output/snake-orig

snake : snake.c 

output :
	mkdir output

output/snake-orig : snake output
	cp snake output/snake-orig

clean:
	rm -rf snake output/*
