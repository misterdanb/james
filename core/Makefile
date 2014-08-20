SHELL = /bin/sh
CC    = g++

INCLUDE      = -I .
FLAGS        = -std=c++11
CFLAGS       = -O3 -fPIC -c
LDFLAGS      = -shared
DEBUGFLAGS   = -O0 -D DEBUG -pg -g -ggdb
RELEASEFLAGS = -O3 -D NODEBUG

TARGET  = libjames.so
SOURCES = $(shell echo cpu/*.cpp cartridges/*.cpp *.cpp)
HEADERS = $(shell echo cpu/*.hpp cartridges/*.hpp *.hpp *.template)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

debug: $(TARGET)-debug

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(LDFLAGS) $(RELEASEFLAGS) -o $(TARGET) $(OBJECTS)

$(TARGET)-debug: $(OBJECTS)
	$(CC) $(FLAGS) $(LDFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)

RenderContext.o: RenderContext.cpp
	$(CC) $(FLAGS) $(CFLAGS) RenderContext.cpp -o RenderContext.o
	
Frontend.o: Frontend.cpp
	$(CC) $(FLAGS) $(CFLAGS) Frontend.cpp -o Frontend.o

GameboyColor.o: GameboyColor.cpp
	$(CC) $(FLAGS) $(CFLAGS) GameboyColor.cpp -o GameboyColor.o

ClassicRenderer.o: ClassicRenderer.cpp
	$(CC) $(FLAGS) $(CFLAGS) ClassicRenderer.cpp -o ClassicRenderer.o

Renderer.o: Renderer.cpp
	$(CC) $(FLAGS) $(CFLAGS) Renderer.cpp -o Renderer.o

Joypad.o: Joypad.cpp
	$(CC) $(FLAGS) $(CFLAGS) Joypad.cpp -o Joypad.o

LCD.o: LCD.cpp
	$(CC) $(FLAGS) $(CFLAGS) LCD.cpp -o LCD.o

Frame.o: Frame.cpp
	$(CC) $(FLAGS) $(CFLAGS) Frame.cpp -o Frame.o

Tile.o: Tile.cpp
	$(CC) $(FLAGS) $(CFLAGS) Tile.cpp -o Tile.o

SpriteAttribute.o: SpriteAttribute.cpp
	$(CC) $(FLAGS) $(CFLAGS) SpriteAttribute.cpp -o SpriteAttribute.o

BackgroundAttribute.o: BackgroundAttribute.cpp
	$(CC) $(FLAGS) $(CFLAGS) BackgroundAttribute.cpp -o BackgroundAttribute.o

cartridges/MMM01.o: cartridges/MMM01.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/MMM01.cpp -o cartridges/MMM01.o

cartridges/RomRam.o: cartridges/RomRam.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/RomRam.cpp -o cartridges/RomRam.o

cartridges/MBC5.o: cartridges/MBC5.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/MBC5.cpp -o cartridges/MBC5.o

cartridges/MBC4.o: cartridges/MBC4.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/MBC4.cpp -o cartridges/MBC4.o

cartridges/MBC3.o: cartridges/MBC3.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/MBC3.cpp -o cartridges/MBC3.o

cartridges/MBC2.o: cartridges/MBC2.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/MBC2.cpp -o cartridges/MBC2.o

cartridges/MBC1.o: cartridges/MBC1.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/MBC1.cpp -o cartridges/MBC1.o

cartridges/RomOnly.o: cartridges/RomOnly.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/RomOnly.cpp -o cartridges/RomOnly.o

cartridges/Cartridge.o: cartridges/Cartridge.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/Cartridge.cpp -o cartridges/Cartridge.o

cartridges/Header.o: cartridges/Header.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/Header.cpp -o cartridges/Header.o

cartridges/RamDimensions.o: cartridges/RamDimensions.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/RamDimensions.cpp -o cartridges/RamDimensions.o

cartridges/RomDimensions.o: cartridges/RomDimensions.cpp
	$(CC) $(FLAGS) $(CFLAGS) cartridges/RomDimensions.cpp -o cartridges/RomDimensions.o

cpu/Processor.o: cpu/Processor.cpp
	$(CC) $(FLAGS) $(CFLAGS) cpu/Processor.cpp -o cpu/Processor.o

cpu/LookUpTables.o: cpu/LookUpTables.cpp
	$(CC) $(FLAGS) $(CFLAGS) cpu/LookUpTables.cpp -o cpu/LookUpTables.o

InterruptHandler.o: InterruptHandler.cpp
	$(CC) $(FLAGS) $(CFLAGS) InterruptHandler.cpp -o InterruptHandler.o

MemoryBlock.o: MemoryBlock.cpp
	$(CC) $(FLAGS) $(CFLAGS) MemoryBlock.cpp -o MemoryBlock.o

MemoryBus.o: MemoryBus.cpp
	$(CC) $(FLAGS) $(CFLAGS) MemoryBus.cpp -o MemoryBus.o

cpu/Timer.o: cpu/Timer.cpp
	$(CC) $(FLAGS) $(CFLAGS) cpu/Timer.cpp -o cpu/Timer.o

James.o: James.cpp
	$(CC) $(FLAGS) $(CFLAGS) James.cpp -o James.o

clean:
	rm -rf $(TARGET).1.0 $(TARGET).1 $(TARGET)
	rm -rf *.o
	rm -rf cpu/*.o
	rm -rf cartridges/*.o

install:
	cp $(TARGET) /usr/lib/
