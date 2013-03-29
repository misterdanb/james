#ifndef _Header_hpp_
#define _Header_hpp_

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
				
				int *entryPoint;
				int *nintendoLogo;
				int *title;
				int *manufacturerCode;
				int *platformFlag;
				int *newLicenseeCode;
				int *superGameboyFlag;
				int *cartridgeType;
				int *romType;
				int *ramType;
				int *destinationCode;
				int *oldLicenseeCode;
				int *romVersionNumber;
				int *headerChecksum;
				int *globalChecksum;
			};
		}
	}
}

#endif
