#ifndef _Renderer_hpp_
#define _Renderer_hpp_

#include "James.hpp"
#include "Memory.hpp"
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
			static const int RENDERED_BACKGROUND_WIDTH = BackgroundTileNumberMap::WIDTH * Tile::WIDTH;
			static const int RENDERED_BACKGROUND_HEIGHT = BackgroundTileNumberMap::HEIGHT * Tile::HEIGHT;
			
			// color enable masks
			static const int COLOR_0 = 0x01;
			static const int COLOR_1 = 0x02;
			static const int COLOR_2 = 0x04;
			static const int COLOR_3 = 0x08;
		
		public:
			Renderer();
			virtual ~Renderer();
			
			void SetMemory(Memory &);
			void SetInterruptHandler(InterruptHandler &);
			
			Memory &GetMemory();
			InterruptHandler &GetInterruptHandler();
			
			virtual void RenderOAMSearch() = GBC_NULL;
			virtual void RenderTransferData() = GBC_NULL;
			virtual void RenderHBlank() = GBC_NULL;
			virtual void RenderVBlank() = GBC_NULL;
			
			virtual Frame GetFrame() = GBC_NULL;
			virtual PixelMap<RENDERED_BACKGROUND_WIDTH, RENDERED_BACKGROUND_HEIGHT> GetRenderedBackground(int) = GBC_NULL;
		
		protected:
			Memory *_memory;
			InterruptHandler *_interruptHandler;
			
			VideoRam *_videoRam;
			SpriteAttributeRam *_spriteAttributeRam;
			IOPorts *_ioPorts;
		};
	}
}

#endif
