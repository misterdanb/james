#include "Cartridge.hpp"

#include "RomOnly.hpp"
#include "MBC1.hpp"
#include "MBC2.hpp"
#include "MBC3.hpp"
#include "MBC4.hpp"
#include "MBC5.hpp"
#include "RomRam.hpp"
#include "MMM01.hpp"

gbc::core::cartridges::Cartridge::Cartridge(int rom[], int size)
	: _header(rom),
	  //_rom(new int[size]),
	  //_ram(new int[_header.ramDimensions.size]),
	  _selectedRomBank(1),
	  _selectedRamBank(0)
{
	_rom = new int[size];
	_ram = new int[_header.ramDimensions.size] { 0, };
	
	memcpy(_rom, rom, size * sizeof(int));
	
	LOG_L2(ToHex(_rom[0x0100]));
	LOG_L2(ToHex(_rom[0x0101]));
	LOG_L2(ToHex(_rom[0x0102]));
	LOG_L2(ToHex(_rom[0x0103]));
}

gbc::core::cartridges::Cartridge::~Cartridge()
{
	delete _rom;
	delete _ram;
}

gbc::core::cartridges::Header gbc::core::cartridges::Cartridge::GetHeader()
{
	return _header;
}

gbc::core::cartridges::Cartridge *gbc::core::cartridges::Cartridge::Create(int rom[], int size)
{
	CartridgeType cartridgeType = CartridgeType(rom[0x0147]);
	
	Cartridge *createdCartridge = NULL;
	std::string createdCartridgeName = "None";
	
	switch (cartridgeType)
	{
		case romOnly:
			createdCartridge = new RomOnly(rom, size);
			createdCartridgeName = "Rom only";
			break;
		
		case mbc1:
		case mbc1Ram:
		case mbc1RamBattery:
			createdCartridge = new MBC1(rom, size);
			createdCartridgeName = "MBC1";
			break;
		
		case mbc2:
		case mbc2Battery:
			createdCartridge = new MBC2(rom, size);
			createdCartridgeName = "MBC2";
			break;
		
		case romRam:
		case romRamBattery:
			createdCartridge = new RomRam(rom, size);
			createdCartridgeName = "Rom and ram";
			break;
		
		case mmm01:
		case mmm01Ram:
		case mmm01RamBattery:
			createdCartridge = new MMM01(rom, size);
			createdCartridgeName = "MMM01";
			break;
		
		case mbc3TimerBattery:
		case mbc3TimerRamBattery:
		case mbc3:
		case mbc3Ram:
		case mbc3RamBattery:
			createdCartridge = new MBC3(rom, size);
			createdCartridgeName = "MBC3";
			break;
		
		case mbc4:
		case mbc4Ram:
		case mbc4RamBattery:
			createdCartridge = new MBC4(rom, size);
			createdCartridgeName = "MBC4";
			break;
		
		case mbc5:
		case mbc5Ram:
		case mbc5RamBattery:
		case mbc5Rumble:
		case mbc5RumbleRam:
		case mbc5RumbleRamBattery:
			createdCartridge = new MBC5(rom, size);
			createdCartridgeName = "MBC5";
			break;
		
		// ahahahahahaha... no.jpg
		/*case pocketCamera:
			return new PocketCamera(rom);
		
		case bandaiTamas:
			return new BandaiTamas(rom);
		
		case huC3:
			return new HuC3(rom);
		
		case huC1RamBattery:
			return new HuC1(rom);*/
		
		default: break;
	}
	
	Header createdHeader = createdCartridge->GetHeader();
	
	std::ostringstream oss;
	
	oss << "Loaded cartridge" << std::endl;
	oss << "\tEntry point: " << ToHex(createdHeader.entryPoint[0]) << " ";
	oss <<                      ToHex(createdHeader.entryPoint[1]) << " ";
	oss <<                      ToHex(createdHeader.entryPoint[2]) << " ";
	oss <<                      ToHex(createdHeader.entryPoint[3]) << std::endl;
	oss << "\tTitle: " << ToString(createdHeader.newTitle, 11) << std::endl;
	oss << "\tManufacturer code: " << ToString(createdHeader.manufacturerCode, 4) << std::endl;
	oss << "\tCGB flag: " << ToHex((int) createdHeader.platformSupport) << std::endl;
	oss << "\tNew licensee code: " + ToString(createdHeader.newLicenseeCode, 2) << std::endl;
	oss << "\tSGB flag: " << ToHex(createdHeader.superGameboyFlag) << std::endl;
	oss << "\tCartridge type: " << createdCartridgeName << std::endl;
	oss << "\tRom size: " << ToDec(createdHeader.romDimensions.size) << " bytes splitted into "; 
	oss <<                   ToDec(createdHeader.romDimensions.banks) << " banks" << std::endl;
	oss << "\tRam size: " << ToDec(createdHeader.ramDimensions.size) << " bytes splitted into ";
	oss <<                   ToDec(createdHeader.ramDimensions.banks) << " banks" << std::endl;
	oss << "\tDestination code: " << ((createdHeader.destinationCode == japanese) ? "Japanese" : "non Japanese") << std::endl;
	oss << "\tOld licensee code: " << ToHex(createdHeader.oldLicenseeCode)  << std::endl;
	oss << "\tRom version Number: " << ToDec(createdHeader.romVersionNumber) << std::endl;
	oss << "\tHeader checksum: " << ToHex(createdHeader.headerChecksum) << std::endl;
	oss << "\tGlobal checksum: " << ToHex(createdHeader.globalChecksum[0]) << " ";
	oss <<                          ToHex(createdHeader.globalChecksum[1]) << std::endl;
	
	LOG_L2(oss.str());
	
	return createdCartridge;
}
