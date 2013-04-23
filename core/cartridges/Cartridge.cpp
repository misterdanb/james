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
	  _selectedRomBank(1),
	  _selectedRamBank(0)
{
	_rom = new int[size];
	_ram = new int[_header.ramDimensions.size];
	
	std::memcpy(_rom, rom, size * sizeof(int));
	std::memset(_ram, 0x00, _header.ramDimensions.size);
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
	CartridgeType cartridgeType = CartridgeType(rom[Header::CARTRIDGE_TYPE_ADDRESS]);
	
	Cartridge *createdCartridge = NULL;
	std::string createdCartridgeName = "None";
	
	switch (cartridgeType)
	{
		case CartridgeType::ROM_ONLY:
			createdCartridge = new RomOnly(rom, size);
			createdCartridgeName = "Rom only";
			break;
		
		case CartridgeType::MBC1:
		case CartridgeType::MBC1_RAM:
		case CartridgeType::MBC1_RAM_BATTERY:
			createdCartridge = new MBC1(rom, size);
			createdCartridgeName = "MBC1";
			break;
		
		case CartridgeType::MBC2:
		case CartridgeType::MBC2_BATTERY:
			createdCartridge = new MBC2(rom, size);
			createdCartridgeName = "MBC2";
			break;
		
		case CartridgeType::ROM_RAM:
		case CartridgeType::ROM_RAM_BATTERY:
			createdCartridge = new RomRam(rom, size);
			createdCartridgeName = "Rom and ram";
			break;
		
		case CartridgeType::MMM01:
		case CartridgeType::MMM01_RAM:
		case CartridgeType::MMM01_RAM_BATTERY:
			createdCartridge = new MMM01(rom, size);
			createdCartridgeName = "MMM01";
			break;
		
		case CartridgeType::MBC3_TIMER_BATTERY:
		case CartridgeType::MBC3_TIMER_RAM_BATTERY:
		case CartridgeType::MBC3:
		case CartridgeType::MBC3_RAM:
		case CartridgeType::MBC3_RAM_BATTERY:
			createdCartridge = new MBC3(rom, size);
			createdCartridgeName = "MBC3";
			break;
		
		case CartridgeType::MBC4:
		case CartridgeType::MBC4_RAM:
		case CartridgeType::MBC4_RAM_BATTERY:
			createdCartridge = new MBC4(rom, size);
			createdCartridgeName = "MBC4";
			break;
		
		case CartridgeType::MBC5:
		case CartridgeType::MBC5_RAM:
		case CartridgeType::MBC5_RAM_BATTERY:
		case CartridgeType::MBC5_RUMBLE:
		case CartridgeType::MBC5_RUMBLE_RAM:
		case CartridgeType::MBC5_RUMBLE_RAM_BATTERY:
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

#ifdef DEBUG
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
	oss << "\tDestination code: " << ((createdHeader.destinationCode == DestinationCode::JAPANESE) ? "Japanese" : "non Japanese") << std::endl;
	oss << "\tOld licensee code: " << ToHex(createdHeader.oldLicenseeCode)  << std::endl;
	oss << "\tRom version Number: " << ToDec(createdHeader.romVersionNumber) << std::endl;
	oss << "\tHeader checksum: " << ToHex(createdHeader.headerChecksum) << std::endl;
	oss << "\tGlobal checksum: " << ToHex(createdHeader.globalChecksum[0]) << " ";
	oss <<                          ToHex(createdHeader.globalChecksum[1]) << std::endl;
	
	LOG_L2(oss.str());
#endif
	
	return createdCartridge;
}
