#ifndef _ClassicRenderer_hpp_
#define _ClassicRenderer_hpp_

#include "James.hpp"
#include "Renderer.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		class ClassicRenderer : public Renderer
		{
		  public:
			ClassicRenderer (RenderContext&);
			~ClassicRenderer();

			void RenderOAMSearch();
			void RenderTransferData();
			void RenderHorizontalBlank();
			void RenderVerticalBlank();

			Renderer::RenderedTileMap GetRenderedTileMap (int);

		  private:
			void DrawSprites (int, SpriteToBackgroundPriority);
			void DrawSprites (int);
			void DrawBackgroundMap (int);
			void DrawWindowMap (int);
			void DrawBackgroundMapTile (int, int);
			void DrawWindowMapTile (int, int);
			void DrawTile (Vector2<int>, Tile, HorizontalFlip, VerticalFlip, Array<Color<int>, 4> , int);
		};
	}
}

#endif
