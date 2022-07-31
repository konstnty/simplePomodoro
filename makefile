CC=gcc

all: main

main: main.c
	$(CC)  main.c -o simple
