CC=g++

TETRIS:
	$(CC) -Wall mainOOP.cpp -o TETRIS -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

clean:
	rm -rf *.o
