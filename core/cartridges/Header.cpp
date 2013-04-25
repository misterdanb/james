#include "Header.hpp"
#include <iostream>

gbc::core::cartridges::Header::Header(int rom[])
	: platformSupport(PlatformSupport(rom[PLATFORM_SUPPORT_ADDRESS])),
	  superGameboyFlag(rom[SUPER_GAMEBOY_FLAG_ADDRESS]),
	  cartridgeType(CartridgeType(rom[CARTRIDGE_TYPE_ADDRESS])), 
	  romDimensions(rom[ROM_DIMENSIONS_ADDRESS]),
	  ramDimensions(rom[RAM_DIMENSIONS_ADDRESS]),
	  destinationCode(DestinationCode(rom[DESTINATION_CODE_ADDRESS])),
	  oldLicenseeCode(rom[OLD_LICENSEE_CODE_ADDRESS]),
	  romVersionNumber(rom[ROM_VERSION_NUMBER_ADDRESS]),
	  headerChecksum(rom[HEADER_CHECKSUM_ADDRESS])
{
	std::memcpy(entryPoint, &rom[ENTRY_POINT_ADDRESS], ENTRY_POINT_LENGTH * sizeof(int));
	std::memcpy(nintendoLogo, &rom[NINTENDO_LOGO_ADDRESS], NINTENDO_LOGO_LENGTH * sizeof(int));
	std::memcpy(oldTitle, &rom[OLD_TITLE_ADDRESS], OLD_TITLE_LENGTH * sizeof(int));
	std::memcpy(newTitle, &rom[NEW_TITLE_ADDRESS], NEW_TITLE_LENGTH * sizeof(int));
	std::memcpy(manufacturerCode, &rom[MANUFACTURER_CODE_ADDRESS], MANUFACTURER_CODE_LENGTH * sizeof(int));
	std::memcpy(newLicenseeCode, &rom[NEW_LICENSEE_CODE_ADDRESS], NEW_LICENSEE_CODE_LENGTH * sizeof(int));
	std::memcpy(globalChecksum, &rom[GLOBAL_CHECKSUM_ADDRESS], GLOBAL_CHECKSUM_LENGTH * sizeof(int));
}

gbc::core::cartridges::Header::~Header()
{
}
