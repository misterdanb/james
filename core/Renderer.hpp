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
        /**
         * @class Renderer
         * 
         * Renderer is an abstract implementation to be inherited when
         * creating a renderering technique for the game boy. This might
         * be a correctly emulated rendering for the game boy classic or
         * the game boy color, but also more arty implementations like
         * a 2.5-D gaming experience.
         */
        class Renderer : public NonCopyable
        {
        public:
            static const int BG_WIDTH = BackgroundTileNumberMap::WIDTH * Tile::WIDTH;
            static const int BG_HEIGHT = BackgroundTileNumberMap::HEIGHT * Tile::HEIGHT;
            
            // color enable masks
            static const int COLOR_0 = 0x01;
            static const int COLOR_1 = 0x02;
            static const int COLOR_2 = 0x04;
            static const int COLOR_3 = 0x08;
        
        public:
            /**
             * Constructor of Memory.
             * 
             * Initializes with the default values.
             */
            Renderer();
            
            /**
             * Destructor of Renderer.
             */
            virtual ~Renderer() = default;
            
            void SetMemory(Memory &);
            void SetInterruptHandler(InterruptHandler &);
            
            Memory &GetMemory();
            InterruptHandler &GetInterruptHandler();
            
            virtual void RenderOAMSearch() = 0;
            virtual void RenderTransferData() = 0;
            virtual void RenderHBlank() = 0;
            virtual void RenderVBlank() = 0;
            
            virtual Frame GetFrame() = GBC_NULL;
            virtual PixelMap<BG_WIDTH, BG_HEIGHT> GetRenderedBackground(int) = 0;
        
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
