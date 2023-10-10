CC = g++
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: chess_AI

chess_AI: chess_AI.o $(DEPENDENCIES)
	$(CC) -o chess_AI $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm chess_AI *.o