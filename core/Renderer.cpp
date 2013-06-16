#include "Renderer.hpp"

Renderer::Renderer(RenderContext &rc)
	: _rc(rc), _rcClassic(rc.gameboyClassicSpecific), _rcColor(rc.gameboyColorSpecific)
{
}

Renderer::~Renderer()
{
}

void Renderer::UpdateTiles()
{
	int lastTile = 0;
	
	while (!_rc.changedTiles.empty())
	{
		int videoRamBank = _rc.changedTiles.front().first;
		int tileNumber = _rc.changedTiles.front().second;
		int videoRamAddress = tileNumber * 0x10;
		
		for (int y = 0; y < Tile::HEIGHT; y++)
		{
			int colorNumbersLow = _rc.videoRam[videoRamBank][videoRamAddress + y * 2];
			int colorNumbersHigh = _rc.videoRam[videoRamBank][videoRamAddress + y * 2 + 1];
			
			for (int x = 0; x < Tile::WIDTH; x++)
			{
				_rc.tiles[videoRamBank][tileNumber].data[x][y] = (((colorNumbersHigh << 1) >> (7 - x)) & 0b10) |
				                                                 ((colorNumbersLow >> (7 - x)) & 0b01);
			}
		}
		
		_rc.changedTiles.pop_front();
	}
}

void Renderer::UpdateBackgroundMapElements()
{
	while (!_rc.changedTileMapElements.empty())
	{
		int mapNumber = _rc.changedTileMapElements.front().first;
		int mapElementNumber = _rc.changedTileMapElements.front().second;
		
		int mapElementX = mapElementNumber % TileMap::WIDTH;
		int mapElementY = mapElementNumber / TileMap::WIDTH;
		
		_rc.tileMaps[mapNumber].data[mapElementX][mapElementY] = _rc.videoRam[0][(mapNumber == 0) ?
		                                                                         (0x9800 - 0x8000 + mapElementNumber) :
		                                                                         (0x9C00 - 0x8000 + mapElementNumber)];
		
		_rc.changedTileMapElements.pop_front();
	}
}

void Renderer::UpdateTileMapAttributes()
{
	while (!_rcColor.changedTileMapAttributes.empty())
	{
		int mapNumber = _rcColor.changedTileMapAttributes.front().first;
		int mapElementNumber = _rcColor.changedTileMapAttributes.front().second;
		
		int tileMapAttribute = _rc.videoRam[1][(mapNumber == 0) ?
		                       (0x9800 + mapElementNumber) :
		                       (0x9C00 + mapElementNumber)];
		
		TileMapAttribute &tileMapAttributeToChange = _rcColor.tileMapAttributes[mapNumber][mapElementNumber];
		
		tileMapAttributeToChange.backgroundColorPaletteNumber = tileMapAttribute & 0x07;
		tileMapAttributeToChange.tileVideoRamBankNumber = (tileMapAttribute >> 3) & 0x01;
		tileMapAttributeToChange.horizontalFlip = HorizontalFlip((tileMapAttribute >> 5) & 0x01);
		tileMapAttributeToChange.verticalFlip = VerticalFlip((tileMapAttribute >> 6) & 0x01);
		tileMapAttributeToChange.backgroundToOAMPriority = BackgroundToOAMPriority((tileMapAttribute >> 7) & 0x01);
		
		_rcColor.changedTileMapAttributes.pop_front();
	}
}

void Renderer::UpdateSpriteAttributes()
{
	while (!_rc.changedSpriteAttributes.empty())
	{
		int spriteAttributeNumber = _rc.changedSpriteAttributes.front();
		int oamAddress = spriteAttributeNumber * 4;
		
		int spriteAttributeFlags = _rc.oam[oamAddress + 3];
		
		SpriteAttribute &spriteAttributeToChange = _rc.spriteAttributes[spriteAttributeNumber];
		
		spriteAttributeToChange.y = _rc.oam[oamAddress] - 16;
		spriteAttributeToChange.x = _rc.oam[oamAddress + 1] - 8;
		spriteAttributeToChange.tileNumber = _rc.oam[oamAddress + 2];
		
		spriteAttributeToChange.spriteColorPaletteNumber = spriteAttributeFlags & 0x07;
		spriteAttributeToChange.tileVideoRamBankNumber = (spriteAttributeFlags >> 3) & 0x01;
		spriteAttributeToChange.spriteMonochromePaletteNumber = (spriteAttributeFlags >> 4) & 0x01;
		spriteAttributeToChange.horizontalFlip = HorizontalFlip((spriteAttributeFlags >> 5) & 0x01);
		spriteAttributeToChange.verticalFlip = VerticalFlip((spriteAttributeFlags >> 6) & 0x01);
		spriteAttributeToChange.spriteToBackgroundPriority = SpriteToBackgroundPriority((spriteAttributeFlags >> 7) & 0x01);
		
		_rc.changedSpriteAttributes.pop_front();
	}
}
