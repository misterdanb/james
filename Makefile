CC=g++-4.8.1
CFLAGS=-c -std=c++11 -O3
CFLAGSDEBUG=-c -Wall -Wextra -std=c++11 -p -g
	
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lc++
LDFLAGSDEBUG=-lsfml-graphics -lsfml-window -lsfml-system -g -lc++

CFLAGS=-c -std=c++11 -O2 -stdlib=libc++
CFLAGSDEBUG=-c -Wall -Wextra -std=c++11 -g -stdlib=libc++
	
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lc++
LDFLAGSDEBUG=-lsfml-graphics -lsfml-window -lsfml-system -g -lc++
	
OBJECTS=core/gbc.o \
	core/MemoryBus.o \
	core/InterruptHandler.o \
	core/cpu/LookUpTables.o \
	core/cpu/Processor.o \
	core/cartridges/RomDimensions.o \
	core/cartridges/RamDimensions.o \
	core/cartridges/Header.o \
	core/cartridges/Cartridge.o \
	core/cartridges/RomOnly.o \
	core/cartridges/MBC1.o \
	core/cartridges/MBC2.o \
	core/cartridges/MBC3.o \
	core/cartridges/MBC4.o \
	core/cartridges/MBC5.o \
	core/cartridges/RomRam.o \
	core/cartridges/MMM01.o \
	core/Frame.o \
	core/LCD.o \
	core/Joypad.o \
	core/Renderer.o \
	core/ClassicRenderer.o \
	core/GameboyColor.o \
	core/RenderContext.o \
	ui/TileMapWindow.o \
	ui/GameWindow.o \
	main.o

OBJECTSDEBUG=core/gbc-debug.o \
	core/MemoryBus-debug.o \
	core/InterruptHandler-debug.o \
	core/cpu/LookUpTables-debug.o \
	core/cpu/Processor-debug.o \
	core/cartridges/RomDimensions-debug.o \
	core/cartridges/RamDimensions-debug.o \
	core/cartridges/Header-debug.o \
	core/cartridges/Cartridge-debug.o \
	core/cartridges/RomOnly-debug.o \
	core/cartridges/MBC1-debug.o \
	core/cartridges/MBC2-debug.o \
	core/cartridges/MBC3-debug.o \
	core/cartridges/MBC4-debug.o \
	core/cartridges/MBC5-debug.o \
	core/cartridges/RomRam-debug.o \
	core/cartridges/MMM01-debug.o \
	core/Frame-debug.o \
	core/LCD-debug.o \
	core/Joypad-debug.o \
	core/Renderer-debug.o \
	core/ClassicRenderer-debug.o \
	core/RenderContext-debug.o \
	core/GameboyColor-debug.o \
	ui/TileMapWindow-debug.o \
	ui/GameWindow-debug.o \
	main-debug.o

EXECUTABLE=gbc

all: $(EXECUTABLE)
debug: $(EXECUTABLE)-debug

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o main.o

ui/GameWindow.o: ui/GameWindow.cpp
	$(CC) $(CFLAGS) ui/GameWindow.cpp -o ui/GameWindow.o

ui/TileMapWindow.o: ui/TileMapWindow.cpp
	$(CC) $(CFLAGS) ui/TileMapWindow.cpp -o ui/TileMapWindow.o

core/GameboyColor.o: core/GameboyColor.cpp
	$(CC) $(CFLAGS) core/GameboyColor.cpp -o core/GameboyColor.o

core/ClassicRenderer.o: core/ClassicRenderer.cpp
	$(CC) $(CFLAGS) core/ClassicRenderer.cpp -o core/ClassicRenderer.o

core/RenderContext.o: core/RenderContext.cpp
	$(CC) $(CFLAGS) core/RenderContext.cpp -o core/RenderContext.o

core/Renderer.o: core/Renderer.cpp
	$(CC) $(CFLAGS) core/Renderer.cpp -o core/Renderer.o

core/Joypad.o: core/Joypad.cpp
	$(CC) $(CFLAGS) core/Joypad.cpp -o core/Joypad.o

core/LCD.o: core/LCD.cpp
	$(CC) $(CFLAGS) core/LCD.cpp -o core/LCD.o

core/Frame.o: core/Frame.cpp
	$(CC) $(CFLAGS) core/Frame.cpp -o core/Frame.o

core/cartridges/MMM01.o: core/cartridges/MMM01.cpp
	$(CC) $(CFLAGS) core/cartridges/MMM01.cpp -o core/cartridges/MMM01.o

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

core/InterruptHandler.o: core/InterruptHandler.cpp
	$(CC) $(CFLAGS) core/InterruptHandler.cpp -o core/InterruptHandler.o

core/MemoryBus.o: core/MemoryBus.cpp
	$(CC) $(CFLAGS) core/MemoryBus.cpp -o core/MemoryBus.o

core/gbc.o: core/gbc.cpp
	$(CC) $(CFLAGS) core/gbc.cpp -o core/gbc.o

######################################################################################################

$(EXECUTABLE)-debug: $(OBJECTSDEBUG)
	$(CC) $(LDFLAGSDEBUG) $(OBJECTSDEBUG) -o $(EXECUTABLE)-debug

main-debug.o: main.cpp
	$(CC) $(CFLAGSDEBUG) main.cpp -o main-debug.o

ui/GameWindow-debug.o: ui/GameWindow.cpp
	$(CC) $(CFLAGSDEBUG) ui/GameWindow.cpp -o ui/GameWindow-debug.o

ui/TileMapWindow-debug.o: ui/TileMapWindow.cpp
	$(CC) $(CFLAGSDEBUG) ui/TileMapWindow.cpp -o ui/TileMapWindow-debug.o

core/GameboyColor-debug.o: core/GameboyColor.cpp
	$(CC) $(CFLAGSDEBUG) core/GameboyColor.cpp -o core/GameboyColor-debug.o

core/ClassicRenderer-debug.o: core/ClassicRenderer.cpp
	$(CC) $(CFLAGSDEBUG) core/ClassicRenderer.cpp -o core/ClassicRenderer-debug.o

core/RenderContext-debug.o: core/RenderContext.cpp
	$(CC) $(CFLAGSDEBUG) core/RenderContext.cpp -o core/RenderContext-debug.o

core/Renderer-debug.o: core/Renderer.cpp
	$(CC) $(CFLAGSDEBUG) core/Renderer.cpp -o core/Renderer-debug.o

core/Joypad-debug.o: core/Joypad.cpp
	$(CC) $(CFLAGSDEBUG) core/Joypad.cpp -o core/Joypad-debug.o

core/LCD-debug.o: core/LCD.cpp
	$(CC) $(CFLAGSDEBUG) core/LCD.cpp -o core/LCD-debug.o

core/Frame-debug.o: core/Frame.cpp
	$(CC) $(CFLAGSDEBUG) core/Frame.cpp -o core/Frame-debug.o

core/cartridges/MMM01-debug.o: core/cartridges/MMM01.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/MMM01.cpp -o core/cartridges/MMM01-debug.o

core/cartridges/RomRam-debug.o: core/cartridges/RomRam.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/RomRam.cpp -o core/cartridges/RomRam-debug.o

core/cartridges/MBC5-debug.o: core/cartridges/MBC5.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/MBC5.cpp -o core/cartridges/MBC5-debug.o

core/cartridges/MBC4-debug.o: core/cartridges/MBC4.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/MBC4.cpp -o core/cartridges/MBC4-debug.o

core/cartridges/MBC3-debug.o: core/cartridges/MBC3.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/MBC3.cpp -o core/cartridges/MBC3-debug.o

core/cartridges/MBC2-debug.o: core/cartridges/MBC2.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/MBC2.cpp -o core/cartridges/MBC2-debug.o

core/cartridges/MBC1-debug.o: core/cartridges/MBC1.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/MBC1.cpp -o core/cartridges/MBC1-debug.o

core/cartridges/RomOnly-debug.o: core/cartridges/RomOnly.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/RomOnly.cpp -o core/cartridges/RomOnly-debug.o

core/cartridges/Cartridge-debug.o: core/cartridges/Cartridge.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/Cartridge.cpp -o core/cartridges/Cartridge-debug.o

core/cartridges/Header-debug.o: core/cartridges/Header.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/Header.cpp -o core/cartridges/Header-debug.o

core/cartridges/RamDimensions-debug.o: core/cartridges/RamDimensions.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/RamDimensions.cpp -o core/cartridges/RamDimensions-debug.o

core/cartridges/RomDimensions-debug.o: core/cartridges/RomDimensions.cpp
	$(CC) $(CFLAGSDEBUG) core/cartridges/RomDimensions.cpp -o core/cartridges/RomDimensions-debug.o

core/cpu/Processor-debug.o: core/cpu/Processor.cpp
	$(CC) $(CFLAGSDEBUG) core/cpu/Processor.cpp -o core/cpu/Processor-debug.o

core/cpu/LookUpTables-debug.o: core/cpu/LookUpTables.cpp
	$(CC) $(CFLAGSDEBUG) core/cpu/LookUpTables.cpp -o core/cpu/LookUpTables-debug.o

core/InterruptHandler-debug.o: core/InterruptHandler.cpp
	$(CC) $(CFLAGSDEBUG) core/InterruptHandler.cpp -o core/InterruptHandler-debug.o

core/MemoryBus-debug.o: core/MemoryBus.cpp
	$(CC) $(CFLAGSDEBUG) core/MemoryBus.cpp -o core/MemoryBus-debug.o

core/gbc-debug.o: core/gbc.cpp
	$(CC) $(CFLAGSDEBUG) core/gbc.cpp -o core/gbc-debug.o

clean:
	rm -rf *.o $(EXECUTABLE) $(EXECUTABLE)-debug
	rm -rf core/*.o
	rm -rf core/cpu/*.o
	rm -rf core/cartridges/*.o
	rm -rf ui/*.o
