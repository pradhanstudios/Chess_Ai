CC = g++
CFLAGS = -Wall -Wextra -Wpedantic

default: 
	g++ *.cpp -o fast_chess_ai $(CFLAGS) -I ./include/

# chess_AI: chess_AI.o $(DEPENDENCIES)
# 	$(CC) -o fast_chess_ai$^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm chess_AI *.o