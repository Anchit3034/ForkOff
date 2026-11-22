
CFLAGS = -Wall -Wextra -pedantic -std=c2x 

.PHONY: all

all: ForkOff clean execute

	

ForkOff: buildins.o executor.o parser.o main.o 
	gcc $(CFLAGS) $^ -o $@

main.o : ./src/main.c ./Include/includes.h
	gcc $(CFLAGS) -c $^

buildins.o : ./src/buildins.c ./Include/buildins.h ./Include/includes.h
	gcc $(CFLAGS) -c $^

executor.o : ./src/executor.c ./Include/executor.h ./Include/includes.h
	gcc $(CFLAGS) -c $^

parser.o : ./src/parser.c ./Include/parser.h ./Include/includes.h
	gcc $(CFLAGS) -c $^

clean:
	rm -f *.o
	rm -f ./Include/*.gch

execute :
	./ForkOff



