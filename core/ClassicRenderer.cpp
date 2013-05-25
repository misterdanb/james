#include "ClassicRenderer.hpp"

using namespace gbc;
using namespace gbc::core;

ClassicRenderer::ClassicRenderer(RenderContext &rc)
	: Renderer(rc)
{
}

ClassicRenderer::~ClassicRenderer()
{
}

void ClassicRenderer::RenderOAMSearch()
{
	_rc.lcdMode = LCDMode::SEARCHING_OAM;
	
	_rc.coincidenceFlag = (_rc.lcdY == _rc.lcdYCompare);
	
	if (_rc.oamInterruptEnabled)
	{
		_rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	if (_rc.coincidenceInterruptEnabled && _rc.coincidenceFlag)
	{
		_rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	UpdateSpriteAttributes();
}

void ClassicRenderer::RenderTransferData()
{
	_rc.lcdMode = LCDMode::TRANSFERRING_DATA;
	
	UpdateTiles();
	UpdateBackgroundMapElements();
	UpdateSpriteAttributes();
	
	if (_rc.lcdDisplayEnabled)
	{
		if (_rc.backgroundDisplayEnabled)
		{
			DrawBackgroundMap(COLOR_0);
		}
		
		if (_rc.windowDisplayEnabled)
		{
			DrawWindowMap(COLOR_0);
		}
		
		DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_BEHIND_BACKGROUND);
		
		if (_rc.backgroundDisplayEnabled)
		{
			DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3);
		}
		
		if (_rc.windowDisplayEnabled)
		{
			DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3);
		}
		
		DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_ABOVE_BACKGROUND);
	}
}

void ClassicRenderer::RenderHorizontalBlank()
{
	_rc.lcdMode = LCDMode::HORIZONTAL_BLANK;
	
	if (_rc.horizontalBlankInterruptEnabled)
	{
		_rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	if (_rc.lcdY < 144)
	{
		_rc.lcdY++;
	}
}

void ClassicRenderer::RenderVerticalBlank()
{
	_rc.lcdMode = LCDMode::VERTICAL_BLANK;
	
	_rc.coincidenceFlag = (_rc.lcdY == _rc.lcdYCompare);
	
	if (_rc.coincidenceInterruptEnabled && _rc.coincidenceFlag)
	{
		_rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	if (_rc.verticalBlankInterruptEnabled && !_rc.verticalBlankInterruptAlreadyRequested)
	{
		_rc.interruptHandler->SignalVBlankInterrupt();
		_rc.verticalBlankInterruptAlreadyRequested = GBC_TRUE;
	}
	
	if (_rc.lcdY >= 144)
	{
		_rc.lcdY++;
	}
	
	if (_rc.lcdY > 153)
	{
		_rc.lcdY = 0;
		_rc.verticalBlankInterruptAlreadyRequested = GBC_FALSE;
	}
}

Renderer::RenderedTileMap ClassicRenderer::GetRenderedTileMap(int tileMapNumber)
{
	RenderedTileMap renderedTileMap;
	
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		for (int mapY = 0; mapY < TileMap::HEIGHT; mapY++)
		{
			int backgroundMapElement = _rc.tileMaps
			                           [tileMapNumber].data
			                           [mapX][mapY];
			
			int tileNumber = (!_rc.backgroundAndWindowTileDataSelect) ?
			                 (((0x9000 - 0x8000) / 16) + GetSignedValue(backgroundMapElement)) :
			                 (((0x8000 - 0x8000) / 16) + backgroundMapElement);
			
			for (int tileX = 0; tileX < Tile::WIDTH; tileX++)
			{
				for (int tileY = 0; tileY < Tile::HEIGHT; tileY++)
				{
					Color &pixel = renderedTileMap.map[mapX * Tile::WIDTH + tileX][mapY * Tile::HEIGHT + tileY];
					
					pixel = _rcClassic.monochromeBackgroundPalette.colors[_rc.tiles[0][tileNumber].data[tileX][tileY]];
					
					pixel.red = ((pixel.red << 3) | (pixel.red >> 2)) & 0xFF,
					pixel.green = ((pixel.green << 3) | (pixel.green >> 2)) & 0xFF,
					pixel.blue = ((pixel.blue << 3) | (pixel.blue >> 2)) & 0xFF;
				}
			}
		}
	}
	
	return renderedTileMap;
}

void ClassicRenderer::DrawSprites(int enabledColors,
                                  SpriteToBackgroundPriority spriteToBackgroundPriority)
{
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = _rc.spriteAttributes[i];
		
		if (spriteAttribute.spriteToBackgroundPriority == spriteToBackgroundPriority)
		{
			ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                            _rcClassic.monochromeSpritePalette0 :
			                            _rcClassic.monochromeSpritePalette1;
			
			DrawTile(Vector2<int>(spriteAttribute.x, spriteAttribute.y),
					 _rc.tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
					 spriteAttribute.horizontalFlip,
					 spriteAttribute.verticalFlip,
					 colorPalette,
					 enabledColors);
		}
	}
}

void ClassicRenderer::DrawSprites(int enabledColors)
{
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = _rc.spriteAttributes[i];
		
		int tileNumber = (0x8000 - 0x8000) + spriteAttribute.tileNumber;
		
		ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                        _rcClassic.monochromeSpritePalette0 :
			                        _rcClassic.monochromeSpritePalette1;
		
		DrawTile(Vector2<int>(spriteAttribute.x, spriteAttribute.y),
				 _rc.tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
				 spriteAttribute.horizontalFlip,
				 spriteAttribute.verticalFlip,
				 colorPalette,
				 enabledColors);
	}
}

void ClassicRenderer::DrawBackgroundMap(int enabledColors)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		DrawBackgroundMapTile(mapX, enabledColors);
	}
}

void ClassicRenderer::DrawWindowMap(int enabledColors)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		DrawWindowMapTile(mapX, enabledColors);
	}
}

/* DrawBackgroundMapTile
 * Draws a Tile of a TileMap
 * 
 * mapX:                 the x position of the tile in the map
 * enabledColors:        whether we draw with colors or not
 */

void ClassicRenderer::DrawBackgroundMapTile(int mapX, int enabledColors)
{
	Vector2<int> position((mapX * Tile::WIDTH) - _rc.scrollX,
	                      _rc.lcdY - ((_rc.lcdY + _rc.scrollY) % Tile::HEIGHT));
	
	if (position.x < -8)
	{
		position.x += TileMap::WIDTH * Tile::WIDTH; // IAW: 256, eeyup
	}
	
	position.x %= TileMap::WIDTH * Tile::WIDTH;
	
	int mapElementX = mapX;
	int mapElementY = (_rc.lcdY + _rc.scrollY) / Tile::HEIGHT;
	
	if (mapElementY < 0)
	{
		mapElementY += TileMap::HEIGHT;
	}
	
	mapElementY %= TileMap::HEIGHT;
	
	int backgroundMapElement = _rc.tileMaps
		                       [_rc.backgroundTileMapDisplaySelect].data
		                       [mapElementX][mapElementY];
	
	int tileVideoRamBankNumber = 0;
	
	int tileNumber = (!_rc.backgroundAndWindowTileDataSelect) ?
		             (((0x9000 - 0x8000) / 16) + GetSignedValue(backgroundMapElement)) :
		             (((0x8000 - 0x8000) / 16) + backgroundMapElement);
	
	Tile tile = _rc.tiles[tileVideoRamBankNumber][tileNumber];
	
	ColorPalette colorPalette = _rcClassic.monochromeBackgroundPalette;
	
	DrawTile(position,
		     tile,
		     HorizontalFlip::NOT_FLIPPED,
		     VerticalFlip::NOT_FLIPPED,
		     colorPalette,
		     enabledColors);
}

/* DrawWindowMapTile
 * Draws a Tile of a TileMap
 * 
 * mapX:                 the x position of the tile in the map
 * enabledColors:        whether we draw with colors or not
 */

void ClassicRenderer::DrawWindowMapTile(int mapX, int enabledColors)
{
	Vector2<int> position((mapX * Tile::WIDTH) + _rc.windowX,
	                      _rc.lcdY - ((_rc.lcdY - _rc.windowY) % Tile::HEIGHT));
	
	int mapElementX = mapX;
	int mapElementY = (_rc.lcdY - _rc.windowY) / Tile::HEIGHT;
	
	if (mapElementX < 20 && mapElementY < 17)
	{
		int backgroundMapElement = _rc.tileMaps
								   [_rc.windowTileMapDisplaySelect].data
								   [mapElementX][mapElementY];
		
		int tileVideoRamBankNumber = 0;
		
		int tileNumber = (!_rc.backgroundAndWindowTileDataSelect) ?
						 (((0x9000 - 0x8000) / 16) + GetSignedValue(backgroundMapElement)) :
						 (((0x8000 - 0x8000) / 16) + backgroundMapElement);
		
		Tile tile = _rc.tiles[tileVideoRamBankNumber][tileNumber];
		
		ColorPalette colorPalette = _rcClassic.monochromeBackgroundPalette;
		
		DrawTile(position,
				 tile,
				 HorizontalFlip::NOT_FLIPPED,
				 VerticalFlip::NOT_FLIPPED,
				 colorPalette,
				 enabledColors);
	}
}

void ClassicRenderer::DrawTile(Vector2<int> position,
                               Tile tile,
                               HorizontalFlip horizontalFlip,
                               VerticalFlip verticalFlip,
                               ColorPalette colorPalette,
                               int enabledColors)
{
	int tileY = _rc.lcdY - position.y;
	
	if (tileY >= 0 && tileY < Tile::HEIGHT)
	{
		for (int tileX = 0; tileX < Tile::WIDTH; tileX++)
		{
			int scanlinePosition = position.x + tileX;
			
			if (scanlinePosition >= 0 && scanlinePosition < Frame::WIDTH &&
			    _rc.lcdY >= 0 && _rc.lcdY < Frame::HEIGHT)
			{
				int realTileX = tileX;
				int realTileY = tileY;
				
				if (horizontalFlip == HorizontalFlip::FLIPPED)
				{
					realTileX = 7 - tileX;
				}
				
				if (verticalFlip == VerticalFlip::FLIPPED)
				{
					realTileY = 7 - tileY;
				}
				
				int colorNumber = tile.data[realTileX][realTileY];
				
				if (((colorNumber == 0) && (enabledColors & COLOR_0)) ||
				    ((colorNumber == 1) && (enabledColors & COLOR_1)) ||
				    ((colorNumber == 2) && (enabledColors & COLOR_2)) ||
				    ((colorNumber == 3) && (enabledColors & COLOR_3)))
				{
					_rc.rawFrame[_rc.lcdY * Frame::WIDTH + scanlinePosition] = colorPalette.colors[colorNumber];
				}
			}
		}
	}
}
