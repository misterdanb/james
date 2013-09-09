#ifndef _Renderer_hpp_
#define _Renderer_hpp_

#include "James.hpp"
#include "RenderContext.hpp"
#include "Frame.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		class Renderer
		{
		public:
			// further types
			typedef PixelMap<TileMap::WIDTH * Tile::WIDTH, TileMap::HEIGHT * Tile::HEIGHT> RenderedTileMap;
			
			// color enable masks
			static const int COLOR_0 = 1;
			static const int COLOR_1 = 2;
			static const int COLOR_2 = 4;
			static const int COLOR_3 = 16;
		
		public:
			Renderer(RenderContext &);
			virtual ~Renderer();
			
			virtual void RenderOAMSearch() = GBC_NULL;
			virtual void RenderTransferData() = GBC_NULL;
			virtual void RenderHorizontalBlank() = GBC_NULL;
			virtual void RenderVerticalBlank() = GBC_NULL;
			
			virtual RenderedTileMap GetRenderedTileMap(int) = GBC_NULL;
		
		public:
			RenderContext &_rc;
			GameboyClassicSpecificRenderContext &_rcClassic;
			GameboyColorSpecificRenderContext &_rcColor;
		};
	}
}

#endif
