#include "Header.hpp"

gbc::core::cartridges::Header::Header(int rom[])
{
	entryPoint = &rom[0x0100];
	nintendoLogo = &rom[0x0104];
	title = &rom[0x0134];
	manufacturerCode = &rom[0x013F];
	platformFlag = &rom[0x0143];
	newLicenseeCode = &rom[0x0144];
	superGameboyFlag = &rom[0x0146];
	cartridgeType = &rom[0x0147];
	romType = &rom[0x0148];
	ramType = &rom[0x0149];
	destinationCode = &rom[0x014A];
	oldLicenseeCode = &rom[0x014B];
	romVersionNumber = &rom[0x14C];
	headerChecksum = &rom[0x014D];
	globalChecksum = &rom[0x014E];
}

gbc::core::cartridges::Header::~Header()
{
}
