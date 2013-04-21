#include "Header.hpp"
#include <iostream>
gbc::core::cartridges::Header::Header(int rom[])
	: platformSupport(PlatformSupport(rom[0x0143])),
	  superGameboyFlag(rom[0x0146]),
	  cartridgeType(CartridgeType(rom[0x0147])), 
	  romDimensions(rom[0x0148]),
	  ramDimensions(rom[0x0149]),
	  destinationCode(DestinationCode(rom[0x014A])),
	  oldLicenseeCode(rom[0x014B]),
	  romVersionNumber(rom[0x014C]),
	  headerChecksum(rom[0x014D])
{
	std::memcpy(entryPoint, &rom[0x0100], 4 * sizeof(int));
	std::memcpy(nintendoLogo, &rom[0x0104], 48 * sizeof(int));
	std::memcpy(oldTitle, &rom[0x0134], 16 * sizeof(int));
	std::memcpy(newTitle, &rom[0x0134], 11 * sizeof(int));
	std::memcpy(manufacturerCode, &rom[0x013F], 4 * sizeof(int));
	std::memcpy(newLicenseeCode, &rom[0x0144], 2 * sizeof(int));
	std::memcpy(globalChecksum, &rom[0x014E], 2 * sizeof(int));
}

gbc::core::cartridges::Header::~Header()
{
}
