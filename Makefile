CC=g++
CFLAGS=-c -Wall -lsfml-graphics -lsfml-window -lsfml-system -std=c++17
# SOURCES = main
# LDFLAGS = 
# OBJECTS = $(SOURCES:.cpp=.o)
TETRIS:
	$(CC) -Wall main.cpp -o TETRIS -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# CC=g++
# CFLAGS=-c -Wall -lsfml-graphics -lsfml-window -lsfml-system -std=c++17
# SOURCES = main
# LDFLAGS = 
# OBJECTS = $(SOURCES:.cpp=.o, .hpp=.o)
# EXECUTABLE=TETRIS

# all: $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS) 
# 	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# .cpp.o:
# 	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
