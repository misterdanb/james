#include "ClassicRenderer.hpp"

ClassicRenderer::ClassicRenderer (RenderContext& rc)
	: Renderer (rc)
{
}

ClassicRenderer::~ClassicRenderer()
{
}

void ClassicRenderer::RenderOAMSearch()
{
	_rc.coincidenceFlag = (_rc.lcdY == _rc.lcdYCompare);

	if (_rc.oamInterruptEnabled)
	{
		_rc.interruptHandler->SignalLCDStatusInterrupt();
	}

	if (_rc.coincidenceInterruptEnabled && _rc.coincidenceFlag)
	{
		_rc.interruptHandler->SignalLCDStatusInterrupt();
	}
}

void ClassicRenderer::RenderTransferData()
{
	if (_rc.lcdDisplayEnabled)
	{
		if (_rc.backgroundDisplayEnabled)
		{
			DrawBackgroundMap (COLOR_0);
		}

		/*if (_rc.windowDisplayEnabled)
		{
			DrawWindowMap(COLOR_0);
		}*/

		DrawSprites (COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_BEHIND_BACKGROUND);

		if (_rc.backgroundDisplayEnabled)
		{
			DrawBackgroundMap (COLOR_1 | COLOR_2 | COLOR_3);
		}

		if (_rc.windowDisplayEnabled)
		{
			DrawWindowMap (COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
		}

		DrawSprites (COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_ABOVE_BACKGROUND);
	}
}

void ClassicRenderer::RenderHorizontalBlank()
{
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

Renderer::RenderedTileMap ClassicRenderer::GetRenderedTileMap (int tileMapNumber)
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
							 (((0x9000 - 0x8000) / 16) + GetSignedValue (backgroundMapElement)) :
							 (((0x8000 - 0x8000) / 16) + backgroundMapElement);

			for (int tileX = 0; tileX < Tile::WIDTH; tileX++)
			{
				for (int tileY = 0; tileY < Tile::HEIGHT; tileY++)
				{
					Color<int>& pixel = renderedTileMap.map[mapX * Tile::WIDTH + tileX][mapY * Tile::HEIGHT + tileY];

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

void ClassicRenderer::DrawSprites (int enabledColors,
								   SpriteToBackgroundPriority spriteToBackgroundPriority)
{
	int drawnSprites = 0;
	int hardCodedSpriteLimit = 40;

	for (SpriteAttribute spriteAttribute : _rc.spriteAttributes)
	{
		// only draw visible sprites
		if ((spriteAttribute.x != -8) &&
				(spriteAttribute.y != -16) &&
				(spriteAttribute.x < 168) &&
				(spriteAttribute.y < 160) &&
				(_rc.lcdY >= spriteAttribute.y) &&
				(_rc.lcdY < (spriteAttribute.y + 2 * Tile::HEIGHT)) && // double tile height for 8x16 sprites
				(drawnSprites < hardCodedSpriteLimit)) // max 10 sprites per scanline
		{
			if (spriteAttribute.spriteToBackgroundPriority == spriteToBackgroundPriority)
			{
				ColorPalette colorPalette = spriteAttribute.monochromePaletteNumber == 0 ?
											_rcClassic.monochromeSpritePalette0 :
											_rcClassic.monochromeSpritePalette1;

				if (_rc.spriteSize == 0)
				{
					DrawTile (Vector2<int> (spriteAttribute.x, spriteAttribute.y),
							  _rc.tiles[0][ (0x8000 - 0x8000) + spriteAttribute.tileNumber],
							  spriteAttribute.horizontalFlip,
							  spriteAttribute.verticalFlip,
							  colorPalette,
							  enabledColors);
				}
				else if (_rc.spriteSize == 1)
				{
					DrawTile (Vector2<int> (spriteAttribute.x, spriteAttribute.y),
							  _rc.tiles[0][ (0x8000 - 0x8000) + (spriteAttribute.tileNumber & 0xFE)],
							  spriteAttribute.horizontalFlip,
							  spriteAttribute.verticalFlip,
							  colorPalette,
							  enabledColors);

					DrawTile (Vector2<int> (spriteAttribute.x, spriteAttribute.y + 8),
							  _rc.tiles[0][ (0x8000 - 0x8000) + (spriteAttribute.tileNumber | 0x01)],
							  spriteAttribute.horizontalFlip,
							  spriteAttribute.verticalFlip,
							  colorPalette,
							  enabledColors);
				}

				drawnSprites++;
			}
		}
	}
}

void ClassicRenderer::DrawSprites (int enabledColors)
{
	// find a better name than counter
	int drawnSprites = 0;
	int hardCodedSpriteLimit = 40;

	for (SpriteAttribute spriteAttribute : _rc.spriteAttributes)
	{
		// only draw visible sprites
		if ((spriteAttribute.x != -8) &&
				(spriteAttribute.y != -16) &&
				(spriteAttribute.x < 168) &&
				(spriteAttribute.y < 160) &&
				(_rc.lcdY >= spriteAttribute.y) &&
				(_rc.lcdY < (spriteAttribute.y + 2 * Tile::HEIGHT)) && // double tile height for 8x16 sprites
				(drawnSprites < hardCodedSpriteLimit)) // max 10 sprites per scanline
		{
			ColorPalette colorPalette = spriteAttribute.monochromePaletteNumber == 0 ?
										_rcClassic.monochromeSpritePalette0 :
										_rcClassic.monochromeSpritePalette1;

			if (_rc.spriteSize == 0)
			{
				DrawTile (Vector2<int> (spriteAttribute.x, spriteAttribute.y),
						  _rc.tiles[0][ (0x8000 - 0x8000) + spriteAttribute.tileNumber],
						  spriteAttribute.horizontalFlip,
						  spriteAttribute.verticalFlip,
						  colorPalette,
						  enabledColors);
			}
			else if (_rc.spriteSize == 1)
			{
				DrawTile (Vector2<int> (spriteAttribute.x, spriteAttribute.y),
						  _rc.tiles[0][ (0x8000 - 0x8000) + (spriteAttribute.tileNumber & 0xFE)],
						  spriteAttribute.horizontalFlip,
						  spriteAttribute.verticalFlip,
						  colorPalette,
						  enabledColors);

				DrawTile (Vector2<int> (spriteAttribute.x, spriteAttribute.y + 8),
						  _rc.tiles[0][ (0x8000 - 0x8000) + (spriteAttribute.tileNumber | 0x01)],
						  spriteAttribute.horizontalFlip,
						  spriteAttribute.verticalFlip,
						  colorPalette,
						  enabledColors);
			}

			drawnSprites++;
		}
	}
}

void ClassicRenderer::DrawBackgroundMap (int enabledColors)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		DrawBackgroundMapTile (mapX, enabledColors);
	}
}

void ClassicRenderer::DrawWindowMap (int enabledColors)
{
	if (_rc.lcdY >= _rc.windowY)
	{
		for (int mapX = 0; mapX < Frame::WIDTH / Tile::WIDTH; mapX++)
		{
			DrawWindowMapTile (mapX, enabledColors);
		}
	}
}

/* DrawBackgroundMapTile
 * Draws a Tile of a TileMap
 *
 * mapX:                 the x position of the tile in the map
 * enabledColors:        whether we draw with colors or not
 */

void ClassicRenderer::DrawBackgroundMapTile (int mapX, int enabledColors)
{
	Vector2<int> position ((mapX * Tile::WIDTH) - _rc.scrollX,
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
	else
	{
		mapElementY %= TileMap::HEIGHT;
	}

	int backgroundMapElement = _rc.tileMaps
							   [_rc.backgroundTileMapDisplaySelect].data
							   [mapElementX][mapElementY];

	int tileVideoRamBankNumber = 0;

	int tileNumber = (!_rc.backgroundAndWindowTileDataSelect) ?
					 (((0x9000 - 0x8000) / 16) + GetSignedValue (backgroundMapElement)) :
					 (((0x8000 - 0x8000) / 16) + backgroundMapElement);

	Tile tile = _rc.tiles[tileVideoRamBankNumber][tileNumber];

	ColorPalette colorPalette = _rcClassic.monochromeBackgroundPalette;

	DrawTile (position,
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

void ClassicRenderer::DrawWindowMapTile (int mapX, int enabledColors)
{
	Vector2<int> position ((mapX * Tile::WIDTH) + _rc.windowX,
						   _rc.lcdY - ((_rc.lcdY - _rc.windowY) % Tile::HEIGHT));

	int mapElementX = mapX;
	int mapElementY = (_rc.lcdY - _rc.windowY) / Tile::HEIGHT;

	if (mapElementX < Frame::WIDTH / Tile::WIDTH && mapElementY < Frame::HEIGHT / Tile::HEIGHT)
	{
		int windowMapElement = _rc.tileMaps
							   [_rc.windowTileMapDisplaySelect].data
							   [mapElementX][mapElementY];

		int tileVideoRamBankNumber = 0;

		int tileNumber = (!_rc.backgroundAndWindowTileDataSelect) ?
						 (((0x9000 - 0x8000) / 16) + GetSignedValue (windowMapElement)) :
						 (((0x8000 - 0x8000) / 16) + windowMapElement);

		Tile tile = _rc.tiles[tileVideoRamBankNumber][tileNumber];

		ColorPalette colorPalette = _rcClassic.monochromeBackgroundPalette;

		DrawTile (position,
				  tile,
				  HorizontalFlip::NOT_FLIPPED,
				  VerticalFlip::NOT_FLIPPED,
				  colorPalette,
				  enabledColors);
	}
}

void ClassicRenderer::DrawTile (Vector2<int> position,
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
