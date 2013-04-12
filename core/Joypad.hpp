#ifndef _Joypad_hpp_
#define _Joypad_hpp_

namespace gbc
{
	namespace core
	{
		class IJoypad
		{
		public:
			virtual ~IJoypad();
			virtual int GetRight() = 0;
			virtual int GetLeft() = 0;
			virtual int GetUp() = 0;
			virtual int GetDown() = 0;
			virtual int GetButtonA() = 0;
			virtual int GetButtonB() = 0;
			virtual int GetSelect() = 0;
			virtual int GetStart() = 0;
		};
	}
}

#endif
