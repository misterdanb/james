CC=g++

CFLAGS=-c -std=c++11 -O2
CFLAGSDEBUG=-c -Wall -Wextra -std=c++11 -pg -O2

LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
LDFLAGSDEBUG=-lsfml-graphics -lsfml-window -lsfml-system -pg

OBJECTS=core/GBC.o core/MemoryBus.o core/cpu/LookUpTables.o core/cpu/Processor.o core/cartridges/RomDimensions.o core/cartridges/RamDimensions.o core/cartridges/Header.o core/cartridges/Cartridge.o core/cartridges/RomOnly.o core/cartridges/MBC1.o core/cartridges/MBC2.o core/cartridges/MBC3.o core/cartridges/MBC4.o core/cartridges/MBC5.o core/cartridges/RomRam.o core/Scanline.o core/LCD.o core/GameboyColor.o ui/GameWindow.o main.o
OBJECTSDEBUG=core/GBC-debug.o core/MemoryBus-debug.o core/cpu/LookUpTables-debug.o core/cpu/Processor-debug.o core/cartridges/RomDimensions-debug.o core/cartridges/RamDimensions-debug.o core/cartridges/Header-debug.o core/cartridges/Cartridge-debug.o core/cartridges/RomOnly-debug.o core/cartridges/MBC1-debug.o core/cartridges/MBC2-debug.o core/cartridges/MBC3-debug.o core/cartridges/MBC4-debug.o core/cartridges/MBC5-debug.o core/cartridges/RomRam-debug.o core/Scanline-debug.o core/LCD-debug.o core/GameboyColor-debug.o ui/GameWindow-debug.o main-debug.o

EXECUTABLE=gbc

all: $(EXECUTABLE)
debug: $(EXECUTABLE)-debug

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main.o

ui/GameWindow.o: ui/GameWindow.cpp
	$(CC) $(CFLAGS) ui/GameWindow.cpp -o ui/GameWindow.o

core/GameboyColor.o: core/GameboyColor.cpp
	$(CC) $(CFLAGS) core/GameboyColor.cpp -o core/GameboyColor.o

core/LCD.o: core/LCD.cpp
	$(CC) $(CFLAGS) core/LCD.cpp -o core/LCD.o

core/Scanline.o: core/Scanline.cpp
	$(CC) $(CFLAGS) core/Scanline.cpp -o core/Scanline.o

core/cartridges/RomRam.o: core/cartridges/RomRam.cpp
	$(CC) $(CFLAGS) core/cartridges/RomRam.cpp -o core/cartridges/RomRam.o

core/cartridges/MBC5.o: core/cartridges/MBC5.cpp
	$(CC) $(CFLAGS) core/cartridges/MBC5.cpp -o core/cartridges/MBC5.o

core/cartridges/MBC4.o: core/cartridges/MBC4.cpp
	$(CC) $(CFLAGS) core/cartridges/MBC4.cpp -o core/cartridges/MBC4.o

core/cartridges/MBC3.o: core/cartridges/MBC3.cpp
	$(CC) $(CFLAGS) core/cartridges/MBC3.cpp -o core/cartridges/MBC3.o

core/cartridges/MBC2.o: core/cartridges/MBC2.cpp
	$(CC) $(CFLAGS) core/cartridges/MBC2.cpp -o core/cartridges/MBC2.o

core/cartridges/MBC1.o: core/cartridges/MBC1.cpp
	$(CC) $(CFLAGS) core/cartridges/MBC1.cpp -o core/cartridges/MBC1.o

core/cartridges/RomOnly.o: core/cartridges/RomOnly.cpp
	$(CC) $(CFLAGS) core/cartridges/RomOnly.cpp -o core/cartridges/RomOnly.o

core/cartridges/Cartridge.o: core/cartridges/Cartridge.cpp
	$(CC) $(CFLAGS) core/cartridges/Cartridge.cpp -o core/cartridges/Cartridge.o

core/cartridges/Header.o: core/cartridges/Header.cpp
	$(CC) $(CFLAGS) core/cartridges/Header.cpp -o core/cartridges/Header.o

core/cartridges/RamDimensions.o: core/cartridges/RamDimensions.cpp
	$(CC) $(CFLAGS) core/cartridges/RamDimensions.cpp -o core/cartridges/RamDimensions.o

core/cartridges/RomDimensions.o: core/cartridges/RomDimensions.cpp
	$(CC) $(CFLAGS) core/cartridges/RomDimensions.cpp -o core/cartridges/RomDimensions.o

core/cpu/Processor.o: core/cpu/Processor.cpp
	$(CC) $(CFLAGS) core/cpu/Processor.cpp -o core/cpu/Processor.o

core/cpu/LookUpTables.o: core/cpu/LookUpTables.cpp
	$(CC) $(CFLAGS) core/cpu/LookUpTables.cpp -o core/cpu/LookUpTables.o

core/MemoryBus.o: core/MemoryBus.cpp
	$(CC) $(CFLAGS) core/MemoryBus.cpp -o core/MemoryBus.o

core/GBC.o: core/GBC.cpp
	$(CC) $(CFLAGS) core/GBC.cpp -o core/GBC.o

######################################################################################################

$(EXECUTABLE)-debug: $(OBJECTSDEBUG)
	$(CC) $(LDFLAGSDEBUG) $(OBJECTSDEBUG) -o $(EXECUTABLE)-debug

main-debug.o: main.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) main.cpp -o main-debug.o

ui/GameWindow-debug.o: ui/GameWindow.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) ui/GameWindow.cpp -o ui/GameWindow-debug.o

core/GameboyColor-debug.o: core/GameboyColor.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/GameboyColor.cpp -o core/GameboyColor-debug.o

core/LCD-debug.o: core/LCD.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/LCD.cpp -o core/LCD-debug.o

core/Scanline-debug.o: core/Scanline.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/Scanline.cpp -o core/Scanline-debug.o

core/cartridges/RomRam-debug.o: core/cartridges/RomRam.cpp
	$(CC) $(LDFLAGS) $(CFLAGS) core/cartridges/RomRam.cpp -o core/cartridges/RomRam-debug.o

core/cartridges/MBC5-debug.o: core/cartridges/MBC5.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/MBC5.cpp -o core/cartridges/MBC5-debug.o

core/cartridges/MBC4-debug.o: core/cartridges/MBC4.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/MBC4.cpp -o core/cartridges/MBC4-debug.o

core/cartridges/MBC3-debug.o: core/cartridges/MBC3.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/MBC3.cpp -o core/cartridges/MBC3-debug.o

core/cartridges/MBC2-debug.o: core/cartridges/MBC2.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/MBC2.cpp -o core/cartridges/MBC2-debug.o

core/cartridges/MBC1-debug.o: core/cartridges/MBC1.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/MBC1.cpp -o core/cartridges/MBC1-debug.o

core/cartridges/RomOnly-debug.o: core/cartridges/RomOnly.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/RomOnly.cpp -o core/cartridges/RomOnly-debug.o

core/cartridges/Cartridge-debug.o: core/cartridges/Cartridge.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/Cartridge.cpp -o core/cartridges/Cartridge-debug.o

core/cartridges/Header-debug.o: core/cartridges/Header.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/Header.cpp -o core/cartridges/Header-debug.o

core/cartridges/RamDimensions-debug.o: core/cartridges/RamDimensions.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/RamDimensions.cpp -o core/cartridges/RamDimensions-debug.o

core/cartridges/RomDimensions-debug.o: core/cartridges/RomDimensions.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cartridges/RomDimensions.cpp -o core/cartridges/RomDimensions-debug.o

core/cpu/Processor-debug.o: core/cpu/Processor.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cpu/Processor.cpp -o core/cpu/Processor-debug.o

core/cpu/LookUpTables-debug.o: core/cpu/LookUpTables.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/cpu/LookUpTables.cpp -o core/cpu/LookUpTables-debug.o

core/MemoryBus-debug.o: core/MemoryBus.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/MemoryBus.cpp -o core/MemoryBus-debug.o

core/GBC-debug.o: core/GBC.cpp
	$(CC) $(LDFLAGSDEBUG) $(CFLAGSDEBUG) core/GBC.cpp -o core/GBC-debug.o

clean:
	rm -rf *.o $(EXECUTABLE) $(EXECUTABLE)-debug
	rm -rf core/*.o
	rm -rf core/cpu/*.o
	rm -rf core/cartridges/*.o
	rm -rf ui/*.o
