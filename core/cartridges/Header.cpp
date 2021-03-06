#include "Header.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

Header::Header (DynamicArray<int>& rom)
	: entryPoint(),
	  nintendoLogo(),
	  oldTitle(),
	  newTitle(),
	  manufacturerCode(),
	  platformSupport (PlatformSupport (rom[PLATFORM_SUPPORT_ADDRESS])),
	  newLicenseeCode(),
	  superGameboyFlag (rom[SUPER_GAMEBOY_FLAG_ADDRESS]),
	  cartridgeType (CartridgeType (rom[CARTRIDGE_TYPE_ADDRESS])),
	  romDimensions (rom[ROM_DIMENSIONS_ADDRESS]),
	  ramDimensions (rom[RAM_DIMENSIONS_ADDRESS]),
	  destinationCode (DestinationCode (rom[DESTINATION_CODE_ADDRESS])),
	  oldLicenseeCode (rom[OLD_LICENSEE_CODE_ADDRESS]),
	  romVersionNumber (rom[ROM_VERSION_NUMBER_ADDRESS]),
	  headerChecksum (rom[HEADER_CHECKSUM_ADDRESS]),
	  globalChecksum()
{
	std::copy (rom.begin() + ENTRY_POINT_ADDRESS,
			   rom.begin() + ENTRY_POINT_ADDRESS + ENTRY_POINT_LENGTH,
			   entryPoint.begin());

	std::copy (rom.begin() + NINTENDO_LOGO_ADDRESS,
			   rom.begin() + NINTENDO_LOGO_ADDRESS + NINTENDO_LOGO_LENGTH,
			   nintendoLogo.begin());

	std::copy (rom.begin() + OLD_TITLE_ADDRESS,
			   rom.begin() + OLD_TITLE_ADDRESS + OLD_TITLE_LENGTH,
			   oldTitle.begin());

	std::copy (rom.begin() + NEW_TITLE_ADDRESS,
			   rom.begin() + NEW_TITLE_ADDRESS + NEW_TITLE_LENGTH,
			   newTitle.begin());

	std::copy (rom.begin() + MANUFACTURER_CODE_ADDRESS,
			   rom.begin() + MANUFACTURER_CODE_ADDRESS + MANUFACTURER_CODE_LENGTH,
			   manufacturerCode.begin());

	std::copy (rom.begin() + NEW_LICENSEE_CODE_ADDRESS,
			   rom.begin() + NEW_LICENSEE_CODE_ADDRESS + NEW_LICENSEE_CODE_LENGTH,
			   newLicenseeCode.begin());

	std::copy (rom.begin() + GLOBAL_CHECKSUM_ADDRESS,
			   rom.begin() + GLOBAL_CHECKSUM_ADDRESS + GLOBAL_CHECKSUM_LENGTH,
			   globalChecksum.begin());
}

Header::~Header()
{
}
