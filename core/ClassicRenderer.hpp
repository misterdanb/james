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
			ClassicRenderer();
			~ClassicRenderer();
			
			void RenderOAMSearch(RenderContext &);
			void RenderTransferData(RenderContext &);
			void RenderHorizontalBlank(RenderContext &);
			void RenderVerticalBlank(RenderContext &);
		
		private:
			void DrawSprites(int, SpriteToBackgroundPriority, RenderContext &);
			void DrawSprites(int, RenderContext &);
			void DrawBackgroundMap(int, RenderContext &);
			void DrawWindowMap(int, RenderContext &);
			void DrawMapTile(int, int, int, int, int, RenderContext &);
			void DrawTile(int, int, Tile, HorizontalFlip, VerticalFlip, ColorPalette, int, RenderContext &);
		};
	}
}

#endif
