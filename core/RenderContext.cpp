#include "RenderContext.hpp"

RenderContext::RenderContext()
	:  videoRam(),
	   workRam(),
	   oam(),
	   ioPorts(),
	   highRam(),
	   interruptEnableRegister(),
	   selectedWorkRamBank(),
	   selectedVideoRamBank(),
	   memoryBus(),
	   interruptHandler(),
	   verticalBlankInterruptAlreadyRequested(),
	   verticalBlankInterruptEnabled(),
	   lcdStatusInterruptEnabled(),
	   timerInterruptEnabled(),
	   serialInterruptEnabled(),
	   joypadInterruptEnabled(),
	   lcdDisplayEnabled(),
	   windowMapDisplaySelect(),
	   windowDisplayEnabled(),
	   backgroundAndWindowTileDataSelect(),
	   backgroundMapDisplaySelect(),
	   spriteSize(),
	   spriteDisplayEnabled(),
	   backgroundDisplayEnabled(),
	   lcdMode(),
	   coincidenceFlag(),
	   horizontalBlankInterruptEnabled(),
	   verticalBlankInterruptEnabledInLCD(),
	   oamInterruptEnabled(),
	   coincidenceInterruptEnabled(),
	   scrollX(),
	   scrollY(),
	   lcdY(),
	   lcdYCompare(),
	   windowX(),
	   windowY(),
	   changedTiles(),
	   changedBackgroundMapElements(),
	   spriteAttributes(),
	   changedSpriteAttributes(),
	   gameboyClassicSpecific(),
	   gameboyColorSpecific()
{
}

RenderContext::~RenderContext()
{
}

GameboyClassicSpecificRenderContext::GameboyClassicSpecificRenderContext()
	:  monochromeBackgroundPalette(),
	   monochromeSpritePalette0(),
	   monochromeSpritePalette1()
{
}

GameboyClassicSpecificRenderContext::~GameboyClassicSpecificRenderContext()
{
}

GameboyColorSpecificRenderContext::GameboyColorSpecificRenderContext()
	:  backgroundAttributes(),
	   changedBackgroundAttributes(),
	   colorBackgroundPalettes(),
	   colorSpritePalettes(),
	   dmaTransferActive(),
	   dmaTransferSourceAddress(),
	   dmaTransferDestinationAddress(),
	   dmaTransferLength(),
	   currentDMATransferOffset()
{
}

GameboyColorSpecificRenderContext::~GameboyColorSpecificRenderContext()
{
}

