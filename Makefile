
CFLAGS = -Wall -Wextra -pedantic -std=c2x 
all: ForkOff

	

ForkOff: buildins.o executor.o parser.o main.o 
	gcc $(CFLAGS) main.o buildins.o executor.o parser.o -o ForkOff

main.o : ./src/main.c
	gcc $(CFLAGS) -c ./src/main.c 

buildins.o : ./src/buildins.c
	gcc $(CFLAGS) -c ./src/buildins.c 

executor.o : ./src/executor.c
	gcc $(CFLAGS) -c ./src/executor.c

parser.o : ./src/parser.c
	gcc $(CFLAGS) -c ./src/parser.c





clean:
	rm -f *.o
