CFLAGS = -Wall -Wextra -pedantic -std=c2x 

.PHONY: all
all: ForkOff clean

	

ForkOff: buildins.o executor.o parser.o main.o 
	gcc $(CFLAGS) $^ -o $@

main.o : ./src/main.c
	gcc $(CFLAGS) -c $<

buildins.o : ./src/buildins.c
	gcc $(CFLAGS) -c $<

executor.o : ./src/executor.c
	gcc $(CFLAGS) -c $<

parser.o : ./src/parser.c
	gcc $(CFLAGS) -c $<





clean:
	rm -f *.o
