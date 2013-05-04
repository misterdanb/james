#include "Renderer.hpp"

gbc::core::Renderer::~Renderer()
{
}

void gbc::core::Renderer::UpdateTiles(RenderContext &rc)
{
	int lastTile = 0;
	
	while (rc.changedTiles.size() > 0)
	{
		int videoRamBank = rc.changedTiles.back()[0];
		int tileNumber = rc.changedTiles.back()[1];
		int videoRamAddress = tileNumber * 0x10;
		
		for (int y = 0; y < Tile::HEIGHT; y++)
		{
			int colorNumbersLow = rc.videoRam[videoRamBank][videoRamAddress + y * 2];
			int colorNumbersHigh = rc.videoRam[videoRamBank][videoRamAddress + y * 2 + 1];
			
			for (int x = 0; x < Tile::WIDTH; x++)
			{
				rc.tiles[videoRamBank][tileNumber].data[x][y] = (((colorNumbersHigh << 1) >> (7 - x)) & 0b10) |
				                                                ((colorNumbersLow >> (7 - x)) & 0b01);
			}
		}
		
		delete[] rc.changedTiles.back();
		
		rc.changedTiles.pop_back();
	}
}

void gbc::core::Renderer::UpdateBackgroundMapElements(RenderContext &rc)
{
	while (rc.changedTileMapElements.size() > 0)
	{
		int mapNumber = rc.changedTileMapElements.back()[0];
		int mapElementNumber = rc.changedTileMapElements.back()[1];
		
		int mapElementX = mapElementNumber % TileMap::WIDTH;
		int mapElementY = mapElementNumber / TileMap::WIDTH;
		
		rc.tileMaps[mapNumber].data[mapElementX][mapElementY] = rc.videoRam[0][(mapNumber == 0) ?
		                                                                       (0x9800 - 0x8000 + mapElementNumber) :
		                                                                       (0x9C00 - 0x8000 + mapElementNumber)];
		
		delete[] rc.changedTileMapElements.back();
		
		rc.changedTileMapElements.pop_back();
	}
}

void gbc::core::Renderer::UpdateTileMapAttributes(RenderContext &rc)
{
	RenderContext::GameboyColorSpecificRenderContext &rcColor = rc.gameboyColorSpecific;
	
	while (rcColor.changedTileMapAttributes.size() > 0)
	{
		int mapNumber = rcColor.changedTileMapAttributes.back()[0];
		int mapElementNumber = rcColor.changedTileMapAttributes.back()[1];
		
		int tileMapAttribute = rc.videoRam[1][(mapNumber == 0) ?
		                       (0x9800 + mapElementNumber) :
		                       (0x9C00 + mapElementNumber)];
		
		TileMapAttribute &tileMapAttributeToChange = rcColor.tileMapAttributes[mapNumber][mapElementNumber];
		
		tileMapAttributeToChange.backgroundColorPaletteNumber = tileMapAttribute & 0x07;
		tileMapAttributeToChange.tileVideoRamBankNumber = (tileMapAttribute >> 3) & 0x01;
		tileMapAttributeToChange.horizontalFlip = HorizontalFlip((tileMapAttribute >> 5) & 0x01);
		tileMapAttributeToChange.verticalFlip = VerticalFlip((tileMapAttribute >> 6) & 0x01);
		tileMapAttributeToChange.backgroundToOAMPriority = BackgroundToOAMPriority((tileMapAttribute >> 7) & 0x01);
		
		delete[] rcColor.changedTileMapAttributes.back();
		
		rcColor.changedTileMapAttributes.pop_back();
	}
}

void gbc::core::Renderer::UpdateSpriteAttributes(RenderContext &rc)
{
	while (rc.changedSpriteAttributes.size() > 0)
	{
		int spriteAttributeNumber = rc.changedSpriteAttributes.back();
		int oamAddress = spriteAttributeNumber * 4;
		
		int spriteAttributeFlags = rc.oam[oamAddress + 3];
		
		SpriteAttribute &spriteAttributeToChange = rc.spriteAttributes[spriteAttributeNumber];
		
		spriteAttributeToChange.y = rc.oam[oamAddress] - 16;
		spriteAttributeToChange.x = rc.oam[oamAddress + 1] - 8;
		spriteAttributeToChange.tileNumber = rc.oam[oamAddress + 2];
		
		spriteAttributeToChange.spriteColorPaletteNumber = spriteAttributeFlags & 0x07;
		spriteAttributeToChange.tileVideoRamBankNumber = (spriteAttributeFlags >> 3) & 0x01;
		spriteAttributeToChange.spriteMonochromePaletteNumber = (spriteAttributeFlags >> 4) & 0x01;
		spriteAttributeToChange.horizontalFlip = HorizontalFlip((spriteAttributeFlags >> 5) & 0x01);
		spriteAttributeToChange.verticalFlip = VerticalFlip((spriteAttributeFlags >> 6) & 0x01);
		spriteAttributeToChange.spriteToBackgroundPriority = SpriteToBackgroundPriority((spriteAttributeFlags >> 7) & 0x01);
		
		rc.changedSpriteAttributes.pop_back();
	}
}
