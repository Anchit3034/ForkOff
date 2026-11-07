

.PHONY: all clean

all: clean ForkOff

	

ForkOff: ./src/main.c obj.o
	$(CC) ./src/main.c $1 $2 $3 -o ForkOff -Wall -Wextra -pedantic -std=c2x

obj.o : ./src/buildins.c ./src/parser.c ./src/executor.c
	$(CC) ./src/buildins.c ./src/parser.c ./src/executor.c -c obj.o -Wall -Wextra -pedantic -std=c2x


clean:
	rm -f *.o
