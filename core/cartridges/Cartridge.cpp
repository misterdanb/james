#include "Cartridge.hpp"

#include "RomOnly.hpp"
#include "MBC1.hpp"
#include "MBC2.hpp"
#include "MBC3.hpp"
#include "MBC4.hpp"
#include "MBC5.hpp"
#include "RomRam.hpp"
#include "MMM01.hpp"

gbc::core::cartridges::Cartridge::Cartridge(int rom[])
	: _rom(rom), _header(rom), _selectedRomBank(0), _selectedRamBank(0)
{
}

gbc::core::cartridges::Cartridge::~Cartridge()
{
}

gbc::core::cartridges::Cartridge *gbc::core::cartridges::Cartridge::Create(int rom[])
{
	CartridgeType cartridgeType = CartridgeType(rom[0x0147]);
	
	switch (cartridgeType)
	{
		case romOnly:
			return new RomOnly(rom);
		
		case mbc1:
		case mbc1Ram:
		case mbc1RamBattery:
			return new MBC1(rom);
		
		case mbc2:
		case mbc2Battery:
			return new MBC2(rom);
		
		case romRam:
		case romRamBattery:
			return new RomRam(rom);
		
		case mmm01:
		case mmm01Ram:
		case mmm01RamBattery:
			return new MMM01(rom);
		
		case mbc3TimerBattery:
		case mbc3TimerRamBattery:
		case mbc3:
		case mbc3Ram:
		case mbc3RamBattery:
			return new MBC3(rom);
		
		case mbc4:
		case mbc4Ram:
		case mbc4RamBattery:
			return new MBC4(rom);
		
		case mbc5:
		case mbc5Ram:
		case mbc5RamBattery:
		case mbc5Rumble:
		case mbc5RumbleRam:
		case mbc5RumbleRamBattery:
			return new MBC5(rom);
		
		// ahahahahahaha... no.jpg
		/*case pocketCamera:
			return new PocketCamera(rom);
		
		case bandaiTamas:
			return new BandaiTamas(rom);
		
		case huC3:
			return new HuC3(rom);
		
		case huC1RamBattery:
			return new HuC1(rom);*/
	}
	
	return NULL;
}
