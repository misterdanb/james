#ifndef _SpriteToBackgroundPriority_hpp_
#define _SpriteToBackgroundPriority_hpp_

namespace gbc
{
	namespace core
	{
		enum class SpriteToBackgroundPriority : int
		{
			SPRITE_ABOVE_BACKGROUND = 0,
			SPRITE_BEHIND_BACKGROUND = 1
		};
	}
}

#endif
