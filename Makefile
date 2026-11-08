
CFLAGS = -Wall -Wextra -pedantic -std=c2x -Wno-implicit-function-declaration

all: ForkOff

	

ForkOff: ./src/buildins.o ./src/executor.o ./src/parser.o ./src/main.o 
	gcc $(CFLAGS) ./src/main.o ./src/buildins.o ./src/executor.o ./src/parser.o -o ForkOff

./src/main.o : ./src/main.c
	gcc $(CFLAGS) ./src/main.c -c ./src/main.o

./src/buildins.o : ./src/buildins.c
	gcc $(CFLAGS) ./src/buildins.c -c ./src/buildins.o

./src/executor.o : ./src/executor.c
	gcc $(CFLAGS) ./src/executor.c -c ./src/executor.o

./src/parser.o : ./src/parser.c
	gcc $(CFLAGS) ./src/parser.c -c ./src/parser.o





clean:
	rm -f ./src/*.o
