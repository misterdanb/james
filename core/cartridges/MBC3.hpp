#ifndef _MBC3_hpp_
#define _MBC3_hpp_

#include "Cartridge.hpp"

/*
 * To be implemented
 */

namespace james
{
	namespace core
	{
		namespace cartridges
		{
			class MBC3 : public Cartridge
			{
			public:
				MBC3(DynamicArray<int>);
				~MBC3();
				
				int ReadByte(int);
				void WriteByte(int, int);
			
			private:
				bool _ramRtcEnabled;
				int _ramRtcMode;
				Array<int, 5> _rtc;
			};
		}
	}
}

#endif
