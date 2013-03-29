#include "Cartridge.hpp"

gbc::core::cartridges::Cartridge::Cartridge(int rom[])
	: _rom(rom), _header(rom)
{
}

gbc::core::cartridges::Cartridge::~Cartridge()
{
}
