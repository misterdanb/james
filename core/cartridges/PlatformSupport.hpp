#ifndef _PlatformSupport_hpp_
#define _PlatformSupport_hpp_

namespace gbc
{
	namespace core
	{
		namespace cartridges
		{
			enum class PlatformSupport
			{
				GAMEBOY_COLOR_SUPPORT = 0x80,
				GAMEBOY_COLOR_ONLY = 0xC0
			};
		}
	}
}

#endif
