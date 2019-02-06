all: main

main: main.c
	gcc -o main -lui -L./build/out main.c

run:
	DYLD_LIBRARY_PATH=./build/out ./main


