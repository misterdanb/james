#ifndef _RomDimensions_hpp_
#define _RomDimensions_hpp_

namespace gbc
{
	namespace core
	{
		namespace cartridges
		{
			class RomDimensions
			{
			public:
				RomDimensions(int);
				~RomDimensions();
				
				int romSize;
				int romBanks;
				int romBankSize;
			};
		}
	}
}

#endif
