#ifndef _MBC1_hpp_
#define _MBC1_hpp_

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
			class MBC1 : public Cartridge
			{
			  public:
				MBC1 (DynamicArray<int>);
				~MBC1();

				void Serialize(std::ostream&);
				void Deserialize(std::istream&);

				int ReadByte (int);
				void WriteByte (int, int);

			  private:
				int _ramEnabled;
				int _romRamMode;
			};
		}
	}
}

#endif
