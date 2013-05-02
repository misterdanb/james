#ifndef _Renderer_hpp_
#define _Renderer_hpp_

#include <vector>

#include "RenderContext.hpp"
#include "Frame.hpp"

namespace gbc
{
	namespace core
	{
		class Renderer
		{
		public:
			// color enable masks
			static const int COLOR_0 = 0b00000001;
			static const int COLOR_1 = 0b00000010;
			static const int COLOR_2 = 0b00000100;
			static const int COLOR_3 = 0b00001000;
		
		public:
			virtual ~Renderer();
			
			void UpdateTiles(RenderContext &);
			void UpdateBackgroundMapElements(RenderContext &);
			void UpdateTileMapAttributes(RenderContext &);
			void UpdateSpriteAttributes(RenderContext &);
			
			virtual void RenderOAMSearch(RenderContext &) = GBC_NULL;
			virtual void RenderTransferData(RenderContext &) = GBC_NULL;
			virtual void RenderHorizontalBlank(RenderContext &) = GBC_NULL;
			virtual void RenderVerticalBlank(RenderContext &) = GBC_NULL;
		};
	}
}

#endif
