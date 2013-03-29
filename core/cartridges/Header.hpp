#ifndef _Header_hpp_
#define _Header_hpp_

#include <cstring>
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
				Header(int[]);
				~Header();
				
				int entryPoint[4];
				int nintendoLogo[48];
				int oldTitle[16];
				int newTitle[11];
				int manufacturerCode[4];
				PlatformSupport platformSupport;
				int newLicenseeCode[2];
				int superGameboyFlag;
				CartridgeType cartridgeType;
				RomDimensions romDimensions;
				RamDimensions ramDimensions;
				DestinationCode destinationCode;
				int oldLicenseeCode;
				int romVersionNumber;
				int headerChecksum;
				int globalChecksum[2];
			};
		}
	}
}

#endif
