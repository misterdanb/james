#include "Cartridge.hpp"

#include "RomOnly.hpp"
#include "MBC1.hpp"
#include "MBC2.hpp"
#include "MBC3.hpp"
#include "MBC4.hpp"
#include "MBC5.hpp"
#include "RomRam.hpp"
#include "MMM01.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

Cartridge::Cartridge(DynamicArray<int> &rom)
	: _rom(),
	  _ram(),
	  _header(rom),
	  _selectedRomBank(1),
	  _selectedRamBank(0)
{
	_rom.resize(_header.romDimensions.size + 1);
	_ram.resize(_header.ramDimensions.size + 1);
	
	std::copy(rom.begin(), rom.end(), _rom.begin());
	std::fill(_ram.begin(), _ram.end(), 0x00);
}

Cartridge::~Cartridge()
{
}

void Cartridge::Serialize(std::ostream &os)
{
	DynamicArray<char> outRam(_ram.begin(), _ram.end());
	
	os.write(&outRam[0], _header.ramDimensions.size + 1);
}

void Cartridge::Deserialize(std::istream &is)
{
	DynamicArray<char> inRam;
	
	inRam.resize(_header.ramDimensions.size + 1);
	
	is.read(&inRam[0], _header.ramDimensions.size + 1);
	
	_ram = DynamicArray<int>(inRam.begin(), inRam.end());
}

Header Cartridge::GetHeader()
{
	return _header;
}

std::string Cartridge::Info()
{
	return std::string("");
}

Cartridge *Cartridge::Create(DynamicArray<int> rom)
{
	CartridgeType cartridgeType = CartridgeType(rom[Header::CARTRIDGE_TYPE_ADDRESS]);
	
	Cartridge *createdCartridge = NULL;
	std::string createdCartridgeName = "None";
	
	switch (cartridgeType)
	{
		case CartridgeType::ROM_ONLY:
			createdCartridge = new RomOnly(rom);
			createdCartridgeName = "Rom only";
			break;
		
		case CartridgeType::MBC1:
		case CartridgeType::MBC1_RAM:
		case CartridgeType::MBC1_RAM_BATTERY:
			createdCartridge = new MBC1(rom);
			createdCartridgeName = "MBC1";
			break;
		
		case CartridgeType::MBC2:
		case CartridgeType::MBC2_BATTERY:
			createdCartridge = new MBC2(rom);
			createdCartridgeName = "MBC2";
			break;
		
		case CartridgeType::ROM_RAM:
		case CartridgeType::ROM_RAM_BATTERY:
			createdCartridge = new RomRam(rom);
			createdCartridgeName = "Rom and ram";
			break;
		
		case CartridgeType::MMM01:
		case CartridgeType::MMM01_RAM:
		case CartridgeType::MMM01_RAM_BATTERY:
			createdCartridge = new MMM01(rom);
			createdCartridgeName = "MMM01";
			break;
		
		case CartridgeType::MBC3_TIMER_BATTERY:
		case CartridgeType::MBC3_TIMER_RAM_BATTERY:
		case CartridgeType::MBC3:
		case CartridgeType::MBC3_RAM:
		case CartridgeType::MBC3_RAM_BATTERY:
			createdCartridge = new MBC3(rom);
			createdCartridgeName = "MBC3";
			break;
		
		case CartridgeType::MBC4:
		case CartridgeType::MBC4_RAM:
		case CartridgeType::MBC4_RAM_BATTERY:
			createdCartridge = new MBC4(rom);
			createdCartridgeName = "MBC4";
			break;
		
		case CartridgeType::MBC5:
		case CartridgeType::MBC5_RAM:
		case CartridgeType::MBC5_RAM_BATTERY:
		case CartridgeType::MBC5_RUMBLE:
		case CartridgeType::MBC5_RUMBLE_RAM:
		case CartridgeType::MBC5_RUMBLE_RAM_BATTERY:
			createdCartridge = new MBC5(rom);
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
	
	oss << "Created cartridge" << std::endl;
	oss << "\tEntry point: " << ToHex(createdHeader.entryPoint[0]) << " ";
	oss <<                      ToHex(createdHeader.entryPoint[1]) << " ";
	oss <<                      ToHex(createdHeader.entryPoint[2]) << " ";
	oss <<                      ToHex(createdHeader.entryPoint[3]) << std::endl;
	oss << "\tTitle: " << ToString(createdHeader.newTitle) << std::endl;
	oss << "\tManufacturer code: " << ToString(createdHeader.manufacturerCode) << std::endl;
	oss << "\tCGB flag: " << ToHex((int) createdHeader.platformSupport) << std::endl;
	oss << "\tNew licensee code: " + ToString(createdHeader.newLicenseeCode) << std::endl;
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
	
	LOG(oss.str());
	
	return createdCartridge;
}
