SHELL = /bin/sh
CC    = g++

INCLUDE      = -I .
LIBS         =  -ljames -lsfml-window -lsfml-system -lsfml-graphics
FLAGS        = -std=c++11
CFLAGS       = -O3 -c
LDFLAGS      = -L../core
DEBUGFLAGS   = -O0 -D DEBUG -pg
RELEASEFLAGS = -O3 -D NODEBUG

TARGET  = james-sfml
SOURCES = $(shell echo *.cpp)
HEADERS = $(shell echo *.hpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

debug: $(TARGET)-debug

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(LDFLAGS) $(RELEASEFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

$(TARGET)-debug: $(OBJECTS)
	$(CC) $(FLAGS) $(LDFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

main.o: main.cpp
	$(CC) $(FLAGS) $(CFLAGS) main.cpp -o main.o

GameWindow.o: GameWindow.cpp
	$(CC) $(FLAGS) $(CFLAGS) GameWindow.cpp -o GameWindow.o

TileMapWindow.o: TileMapWindow.cpp
	$(CC) $(FLAGS) $(CFLAGS) TileMapWindow.cpp -o TileMapWindow.o

clean:
	rm -rf $(TARGET)
	rm -rf *.o

install:
	cp ../core/libjames.so /usr/lib/
	cp $(TARGET) /usr/bin/
