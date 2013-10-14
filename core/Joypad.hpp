#ifndef _Joypad_hpp_
#define _Joypad_hpp_

#include "James.hpp"

namespace james
{
	namespace core
	{
		/*
		 * Joypad is an interface to the physical input unit to
		 * get recent inputs.
		 */
		
		class Joypad
		{
		public:
			virtual ~Joypad();
			
			virtual bool GetRight() = GBC_NULL;
			virtual bool GetLeft() = GBC_NULL;
			virtual bool GetUp() = GBC_NULL;
			virtual bool GetDown() = GBC_NULL;
			virtual bool GetButtonA() = GBC_NULL;
			virtual bool GetButtonB() = GBC_NULL;
			virtual bool GetSelect() = GBC_NULL;
			virtual bool GetStart() = GBC_NULL;
		};
	}
}

#endif
