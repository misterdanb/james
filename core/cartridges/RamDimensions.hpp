#ifndef _RamDimensions_hpp_
#define _RamDimensions_hpp_

namespace gbc
{
	namespace core
	{
		namespace cartridges
		{
			class RamDimensions
			{
			public:
				RamDimensions(int);
				~RamDimensions();
				
				int ramSize;
				int ramBanks;
				int ramBankSize;
			};
		}
	}
}

#endif
