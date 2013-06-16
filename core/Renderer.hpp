#ifndef _Renderer_hpp_
#define _Renderer_hpp_

#include "gbc.hpp"
#include "RenderContext.hpp"
#include "Frame.hpp"

using namespace gbc;
using namespace gbc::core;

namespace gbc
{
	namespace core
	{
		class Renderer
		{
		public:
			// further types
			typedef PixelMap<TileMap::WIDTH * Tile::WIDTH, TileMap::HEIGHT * Tile::HEIGHT> RenderedTileMap;
			
			// color enable masks
			static const int COLOR_0 = 0b00000001;
			static const int COLOR_1 = 0b00000010;
			static const int COLOR_2 = 0b00000100;
			static const int COLOR_3 = 0b00001000;
		
		public:
			Renderer(RenderContext &);
			virtual ~Renderer();
			
			void UpdateTiles();
			void UpdateBackgroundMapElements();
			void UpdateTileMapAttributes();
			void UpdateSpriteAttributes();
			
			virtual void RenderOAMSearch() = GBC_NULL;
			virtual void RenderTransferData() = GBC_NULL;
			virtual void RenderHorizontalBlank() = GBC_NULL;
			virtual void RenderVerticalBlank() = GBC_NULL;
			
			virtual RenderedTileMap GetRenderedTileMap(int) = GBC_NULL;
		
		public:
			RenderContext &_rc;
			RenderContext::GameboyClassicSpecificRenderContext &_rcClassic;
			RenderContext::GameboyColorSpecificRenderContext &_rcColor;
		};
	}
}

#endif
