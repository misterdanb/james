#ifndef _ClassicRenderer_hpp_
#define _ClassicRenderer_hpp_

#include "Renderer.hpp"

namespace gbc
{
	namespace core
	{
		class ClassicRenderer : public Renderer
		{
		public:
			ClassicRenderer(RenderContext &);
			~ClassicRenderer();
			
			void RenderOAMSearch();
			void RenderTransferData();
			void RenderHorizontalBlank();
			void RenderVerticalBlank();
			
			Renderer::RenderedTileMap GetRenderedTileMap(int);
		
		private:
			void DrawSprites(int, SpriteToBackgroundPriority);
			void DrawSprites(int);
			void DrawBackgroundMap(int);
			void DrawWindowMap(int);
			void DrawMapTile(int, int, int, int, int);
			void DrawTile(int, int, Tile, HorizontalFlip, VerticalFlip, ColorPalette, int);
		};
	}
}

#endif
