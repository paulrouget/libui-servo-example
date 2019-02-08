all: main

main: main.c
	clang ${CFLAGS} -o main -lui -lsimpleservo main.c

clean:
	rm -f ./main
