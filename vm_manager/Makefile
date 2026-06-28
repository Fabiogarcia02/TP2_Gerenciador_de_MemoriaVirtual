CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=c11
INC=-Iinclude
SRC=$(wildcard src/*.c)
BIN=vm

all:
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)

run-random: all
	./$(BIN) < data/addresses_random.txt

run-location: all
	./$(BIN) < data/addresses_location.txt
