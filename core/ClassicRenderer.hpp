#ifndef _ClassicRenderer_hpp_
#define _ClassicRenderer_hpp_

#include "James.hpp"
#include "Renderer.hpp"
#include "SpriteSize.hpp"
#include "PixelMap.template"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		class ClassicRenderer : public Renderer
		{
		public:
			ClassicRenderer();
			~ClassicRenderer();
			
			void RenderOAMSearch();
			void RenderTransferData();
			void RenderHBlank();
			void RenderVBlank();
			
			Frame GetFrame();
			PixelMap<Renderer::BG_WIDTH, Renderer::BG_HEIGHT> GetRenderedBackground(int);
		
		private:
			void DrawSprites(int, SpriteToBackgroundPriority);
			void DrawSprites(int);
			void DrawBackgroundMap(int);
			void DrawWindowMap(int);
			void DrawBackgroundMapTile(int, int);
			void DrawWindowMapTile(int, int);
			void DrawTile(Vector2<int>, Tile, HorizontalFlip, VerticalFlip, Array<Color<int>, 4>, int);
			
			bool _vBlankInterruptAlreadyRequested;
			
			Array2<Color<int>, Frame::WIDTH, Frame::HEIGHT> _frameBuffer;
		};
	}
}

#endif
