#ifndef _Header_hpp_
#define _Header_hpp_

#include "../gbc.hpp"

#include <cstring>
#include <algorithm>
#include "PlatformSupport.hpp"
#include "CartridgeType.hpp"
#include "RomDimensions.hpp"
#include "RamDimensions.hpp"
#include "DestinationCode.hpp"

namespace gbc
{
	namespace core
	{
		namespace cartridges
		{
			class Header
			{
			public:
				static const int TOTAL_OFFSET = 0x0100;
				
				static const int ENTRY_POINT_LENGTH = 4;
				static const int NINTENDO_LOGO_LENGTH = 48;
				static const int OLD_TITLE_LENGTH = 16;
				static const int NEW_TITLE_LENGTH = 11;
				static const int MANUFACTURER_CODE_LENGTH = 4;
				static const int PLATFORM_SUPPORT_LENGTH = 1;
				static const int NEW_LICENSEE_CODE_LENGTH = 2;
				static const int SUPER_GAMEBOY_FLAG_LENGTH = 1;
				static const int CARTRIDGE_TYPE_LENGTH = 1;
				static const int ROM_DIMENSIONS_LENGTH = 1;
				static const int RAM_DIMENSIONS_LENGTH = 1;
				static const int DESTINATION_CODE_LENGTH = 1;
				static const int OLD_LICENSEE_CODE_LENGTH = 1;
				static const int ROM_VERSION_NUMBER_LENGTH = 1;
				static const int HEADER_CHECKSUM_LENGTH = 1;
				static const int GLOBAL_CHECKSUM_LENGTH = 2;
				
				static const int ENTRY_POINT_ADDRESS = TOTAL_OFFSET;
				static const int NINTENDO_LOGO_ADDRESS = TOTAL_OFFSET + 0x04;
				static const int OLD_TITLE_ADDRESS = TOTAL_OFFSET + 0x34;
				static const int NEW_TITLE_ADDRESS = TOTAL_OFFSET + 0x34;
				static const int MANUFACTURER_CODE_ADDRESS = TOTAL_OFFSET + 0x3F;
				static const int PLATFORM_SUPPORT_ADDRESS = TOTAL_OFFSET + 0x43;
				static const int NEW_LICENSEE_CODE_ADDRESS = TOTAL_OFFSET + 0x44;
				static const int SUPER_GAMEBOY_FLAG_ADDRESS = TOTAL_OFFSET + 0x46;
				static const int CARTRIDGE_TYPE_ADDRESS = TOTAL_OFFSET + 0x47;
				static const int ROM_DIMENSIONS_ADDRESS = TOTAL_OFFSET + 0x48;
				static const int RAM_DIMENSIONS_ADDRESS = TOTAL_OFFSET + 0x49;
				static const int DESTINATION_CODE_ADDRESS = TOTAL_OFFSET + 0x4A;
				static const int OLD_LICENSEE_CODE_ADDRESS = TOTAL_OFFSET + 0x4B;
				static const int ROM_VERSION_NUMBER_ADDRESS = TOTAL_OFFSET + 0x4C;
				static const int HEADER_CHECKSUM_ADDRESS = TOTAL_OFFSET + 0x4D;
				static const int GLOBAL_CHECKSUM_ADDRESS = TOTAL_OFFSET + 0x4E;
			
			public:
				Header(int[]);
				~Header();
				
				Array<int, ENTRY_POINT_LENGTH> entryPoint;
				Array<int, NINTENDO_LOGO_LENGTH> nintendoLogo;
				Array<int, OLD_TITLE_LENGTH> oldTitle;
				Array<int, NEW_TITLE_LENGTH> newTitle;
				Array<int, MANUFACTURER_CODE_LENGTH> manufacturerCode;
				PlatformSupport platformSupport;
				Array<int, NEW_LICENSEE_CODE_LENGTH> newLicenseeCode;
				int superGameboyFlag;
				CartridgeType cartridgeType;
				RomDimensions romDimensions;
				RamDimensions ramDimensions;
				DestinationCode destinationCode;
				int oldLicenseeCode;
				int romVersionNumber;
				int headerChecksum;
				Array<int, GLOBAL_CHECKSUM_LENGTH> globalChecksum;
			};
		}
	}
}

#endif
