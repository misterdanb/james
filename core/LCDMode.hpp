#ifndef _LCDMode_hpp_
#define _LCDMode_hpp_

namespace gbc
{
	namespace core
	{
		enum class LCDMode : int
		{
			HORIZONTAL_BLANK = 0,
			VERTICAL_BLANK = 1,
			SEARCHING_OAM = 2,
			TRANSFERRING_DATA = 3
		};
	}
}

#endif
