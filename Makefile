all: main

main: main.c
	gcc -o main -lui -L./build/out main.c

run:
	DYLD_LIBRARY_PATH=./build/out ./main

debug:
	RUST_LOG=debug DYLD_LIBRARY_PATH=./build/out ./main

