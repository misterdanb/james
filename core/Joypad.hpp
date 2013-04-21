#ifndef _Joypad_hpp_
#define _Joypad_hpp_

#include "GBC.hpp"

namespace gbc
{
	namespace core
	{
		class IJoypad
		{
		public:
			virtual ~IJoypad();
			
			virtual int GetRight() = GBC_NULL;
			virtual int GetLeft() = GBC_NULL;
			virtual int GetUp() = GBC_NULL;
			virtual int GetDown() = GBC_NULL;
			virtual int GetButtonA() = GBC_NULL;
			virtual int GetButtonB() = GBC_NULL;
			virtual int GetSelect() = GBC_NULL;
			virtual int GetStart() = GBC_NULL;
		};
	}
}

#endif
