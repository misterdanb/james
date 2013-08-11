#ifndef _RamDimensions_hpp_
#define _RamDimensions_hpp_

namespace james
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
				
				int size;
				int banks;
				int bankSize;
			};
		}
	}
}

#endif
