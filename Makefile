all: main

main: main.c
	gcc -o main -lui -lsimpleservo -L./build/out -L/Users/paul/git/servo/target/release/ main.c

run:
	DYLD_LIBRARY_PATH=./build/out:/Users/paul/git/servo/target/release/ ./main

debug:
	RUST_LOG=debug DYLD_LIBRARY_PATH=./build/out ./main

