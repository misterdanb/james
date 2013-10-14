#include "ClassicRenderer.hpp"

ClassicRenderer::ClassicRenderer()
	: Renderer(), _vBlankInterruptAlreadyRequested(false)
{
}

ClassicRenderer::~ClassicRenderer()
{
}

void ClassicRenderer::RenderOAMSearch()
{
	(*_ioPorts).SetCoincidenceFlag((*_ioPorts).GetLCDY() == (*_ioPorts).GetLCDYCompare());
	
	if ((*_ioPorts).GetOAMInterruptEnabled())
	{
		(*_interruptHandler).SignalLCDStatusInterrupt();
	}
	
	if ((*_ioPorts).GetCoincidenceInterruptEnabled() && (*_ioPorts).GetCoincidenceFlag())
	{
		(*_interruptHandler).SignalLCDStatusInterrupt();
	}
}

void ClassicRenderer::RenderTransferData()
{
	if ((*_ioPorts).GetLCDDisplayEnabled())
	{
		if ((*_ioPorts).GetBackgroundDisplayEnabled())
		{
			DrawBackgroundMap(COLOR_0);
		}
		
		/*if (_ioPorts.GetWindowDisplayEnabled())
		{
			DrawWindowMap(COLOR_0);
		}*/
		
		DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_BEHIND_BACKGROUND);
		
		if ((*_ioPorts).GetBackgroundDisplayEnabled())
		{
			DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3);
		}
		
		if ((*_ioPorts).GetWindowDisplayEnabled())
		{
			DrawWindowMap(COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
		}
		
		DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_ABOVE_BACKGROUND);
	}
}

void ClassicRenderer::RenderHBlank()
{
	if ((*_ioPorts).GetHBlankInterruptEnabled())
	{
		(*_interruptHandler).SignalLCDStatusInterrupt();
	}
	
	if ((*_ioPorts).GetLCDY() < 144)
	{
		(*_ioPorts).SetLCDY((*_ioPorts).GetLCDY() + 1);
	}
}

void ClassicRenderer::RenderVBlank()
{
	(*_ioPorts).SetCoincidenceFlag((*_ioPorts).GetLCDY() == (*_ioPorts).GetLCDYCompare());
	
	if ((*_ioPorts).GetCoincidenceInterruptEnabled() && (*_ioPorts).GetCoincidenceFlag())
	{
		(*_interruptHandler).SignalLCDStatusInterrupt();
	}
	
	// which vblank interrupt? in memory or in ioports?
	if ((*_memory).GetVBlankInterruptEnabled() && !_vBlankInterruptAlreadyRequested)
	{
		(*_interruptHandler).SignalVBlankInterrupt();
		_vBlankInterruptAlreadyRequested = true;
	}
	
	if ((*_ioPorts).GetLCDY() >= 144)
	{
		(*_ioPorts).SetLCDY((*_ioPorts).GetLCDY() + 1);
	}
	
	if ((*_ioPorts).GetLCDY() > 153)
	{
		(*_ioPorts).SetLCDY(0);
		_vBlankInterruptAlreadyRequested = false;
	}
}

Frame ClassicRenderer::GetFrame()
{
	return Frame(_frameBuffer);
}

PixelMap<Renderer::RENDERED_BACKGROUND_WIDTH, Renderer::RENDERED_BACKGROUND_HEIGHT> ClassicRenderer::GetRenderedBackground(int backgroundIndex)
{
	PixelMap<RENDERED_BACKGROUND_WIDTH, RENDERED_BACKGROUND_HEIGHT> renderedBackground;
	
	for (int mapX = 0; mapX < BackgroundTileNumberMap::WIDTH; mapX++)
	{
		for (int mapY = 0; mapY < BackgroundTileNumberMap::HEIGHT; mapY++)
		{
			int backgroundTileNumberIndex = mapY * BackgroundTileNumberMap::WIDTH + mapX;
			int backgroundTileNumber = (*_videoRam).GetBackgroundTileNumber(backgroundIndex, backgroundTileNumberIndex);
			
			int tileIndex = 0;
			
			if ((*_ioPorts).GetBackgroundAndWindowTileDataSelect())
			{
				tileIndex = (0x9000 - 0x8000) / Tile::DATA_SIZE + GetSignedValue(backgroundTileNumber);
			}
			else
			{
				tileIndex = (0x8000 - 0x8000) / Tile::DATA_SIZE + backgroundTileNumber;
			}
			
			Array2<int, Tile::WIDTH, Tile::HEIGHT> &tileArray = (*_videoRam).GetTile(tileIndex).GetArray();
			Array<Color<int>, 4> &backgroundPalette = (*_ioPorts).GetMonochromeBackgroundPalette();
			
			for (int tileX = 0; tileX < Tile::WIDTH; tileX++)
			{
				for (int tileY = 0; tileY < Tile::HEIGHT; tileY++)
				{
					Color<int> &pixel = renderedBackground.map[mapX * Tile::WIDTH + tileX][mapY * Tile::HEIGHT + tileY];
					
					pixel = backgroundPalette[tileArray[tileX][tileY]];
					
					pixel.SetRed(((pixel.GetRed() << 3) | (pixel.GetRed() >> 2)) & 0xFF),
					pixel.SetGreen(((pixel.GetGreen() << 3) | (pixel.GetGreen() >> 2)) & 0xFF),
					pixel.SetBlue(((pixel.GetBlue() << 3) | (pixel.GetBlue() >> 2)) & 0xFF);
				}
			}
		}
	}
	
	return renderedBackground;
}

void ClassicRenderer::DrawSprites(int enabledColors, SpriteToBackgroundPriority spriteToBackgroundPriority)
{
	int drawnSprites = 0;
	int hardCodedSpriteLimit = 40;
	
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = (*_spriteAttributeRam).GetSpriteAttribute(i);
		
		// only draw visible sprites
		if ((spriteAttribute.GetX() != -8) &&
		    (spriteAttribute.GetY() != -16) &&
		    (spriteAttribute.GetX() < 168) &&
		    (spriteAttribute.GetY() < 160) &&
		    ((*_ioPorts).GetLCDY() >= spriteAttribute.GetY()) &&
		    ((*_ioPorts).GetLCDY() < (spriteAttribute.GetY() + 2 * Tile::HEIGHT)) && // double tile height for 8x16 sprites
		    (drawnSprites < hardCodedSpriteLimit)) // max 10 sprites per scanline?
		{
			if (spriteAttribute.GetSpriteToBackgroundPriority() == spriteToBackgroundPriority)
			{
				Array<Color<int>, 4> &spritePalette = (*_ioPorts).GetMonochromeSpritePalette(spriteAttribute.GetMonochromePaletteNumber());
				
				if ((*_ioPorts).GetSpriteSize() == SpriteSize::SIZE_8x8)
				{
					DrawTile(Vector2<int>(spriteAttribute.GetX(), spriteAttribute.GetY()),
					         (*_videoRam).GetTile(spriteAttribute.GetTileNumber()),
					         spriteAttribute.GetHorizontalFlip(),
					         spriteAttribute.GetVerticalFlip(),
					         spritePalette,
					         enabledColors);
				}
				else if ((*_ioPorts).GetSpriteSize() == SpriteSize::SIZE_8x16)
				{
					DrawTile(Vector2<int>(spriteAttribute.GetX(), spriteAttribute.GetY()),
					         (*_videoRam).GetTile(spriteAttribute.GetTileNumber() & 0xFE),
					         spriteAttribute.GetHorizontalFlip(),
					         spriteAttribute.GetVerticalFlip(),
					         spritePalette,
					         enabledColors);
					
					DrawTile(Vector2<int>(spriteAttribute.GetX(), spriteAttribute.GetY()),
					         (*_videoRam).GetTile(spriteAttribute.GetTileNumber() | 0x01),
					         spriteAttribute.GetHorizontalFlip(),
					         spriteAttribute.GetVerticalFlip(),
					         spritePalette,
					         enabledColors);
				}
				
				drawnSprites++;
			}
		}
	}
}

void ClassicRenderer::DrawSprites(int enabledColors)
{
	int drawnSprites = 0;
	int hardCodedSpriteLimit = 40;
	
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = (*_spriteAttributeRam).GetSpriteAttribute(i);
		
		// only draw visible sprites
		if ((spriteAttribute.GetX() != -8) &&
		    (spriteAttribute.GetY() != -16) &&
		    (spriteAttribute.GetX() < 168) &&
		    (spriteAttribute.GetY() < 160) &&
		    ((*_ioPorts).GetLCDY() >= spriteAttribute.GetY()) &&
		    ((*_ioPorts).GetLCDY() < (spriteAttribute.GetY() + 2 * Tile::HEIGHT)) && // double tile height for 8x16 sprites
		    (drawnSprites < hardCodedSpriteLimit)) // max 10 sprites per scanline?
		{
			Array<Color<int>, 4> &spritePalette = (*_ioPorts).GetMonochromeSpritePalette(spriteAttribute.GetMonochromePaletteNumber());
			
			if ((*_ioPorts).GetSpriteSize() == SpriteSize::SIZE_8x8)
			{
				DrawTile(Vector2<int>(spriteAttribute.GetX(), spriteAttribute.GetY()),
				         (*_videoRam).GetTile(spriteAttribute.GetTileNumber()),
				         spriteAttribute.GetHorizontalFlip(),
				         spriteAttribute.GetVerticalFlip(),
				         spritePalette,
				         enabledColors);
			}
			else if ((*_ioPorts).GetSpriteSize() == SpriteSize::SIZE_8x16)
			{
				DrawTile(Vector2<int>(spriteAttribute.GetX(), spriteAttribute.GetY()),
				         (*_videoRam).GetTile(spriteAttribute.GetTileNumber() | 0x01),
				         spriteAttribute.GetHorizontalFlip(),
				         spriteAttribute.GetVerticalFlip(),
				         spritePalette,
				         enabledColors);
				
				DrawTile(Vector2<int>(spriteAttribute.GetX(), spriteAttribute.GetY()),
				         (*_videoRam).GetTile(spriteAttribute.GetTileNumber() & 0xFE),
				         spriteAttribute.GetHorizontalFlip(),
				         spriteAttribute.GetVerticalFlip(),
				         spritePalette,
				         enabledColors);
			}
			
			drawnSprites++;
		}
	}
}

void ClassicRenderer::DrawBackgroundMap(int enabledColors)
{
	for (int mapX = 0; mapX < BackgroundTileNumberMap::WIDTH; mapX++)
	{
		DrawBackgroundMapTile(mapX, enabledColors);
	}
}

void ClassicRenderer::DrawWindowMap(int enabledColors)
{
	if ((*_ioPorts).GetLCDY() >= (*_ioPorts).GetWindowY())
	{
		for (int mapX = 0; mapX < Frame::WIDTH / Tile::WIDTH; mapX++)
		{
			DrawWindowMapTile(mapX, enabledColors);
		}
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
	Vector2<int> position((mapX * Tile::WIDTH) - (*_ioPorts).GetScrollX(),
	                      (*_ioPorts).GetLCDY() - (((*_ioPorts).GetLCDY() + (*_ioPorts).GetScrollY()) % Tile::HEIGHT));
	
	if (position.GetX() < -8)
	{
		position.SetX(position.GetX() + BackgroundTileNumberMap::WIDTH * Tile::WIDTH); // IAW: 256, eeyup
	}
	
	position.SetX(position.GetX() % BackgroundTileNumberMap::WIDTH * Tile::WIDTH);
	
	int mapElementX = mapX;
	int mapElementY = ((*_ioPorts).GetLCDY() + (*_ioPorts).GetScrollY()) / Tile::HEIGHT;
	
	if (mapElementY < 0)
	{
		mapElementY += BackgroundTileNumberMap::HEIGHT;
	}
	else
	{
		mapElementY %= BackgroundTileNumberMap::HEIGHT;
	}
	
	int backgroundTileNumberIndex = mapElementY * BackgroundTileNumberMap::WIDTH + mapElementX;
	int backgroundTileNumber = (*_videoRam).GetBackgroundTileNumber((*_ioPorts).GetBackgroundMapDisplaySelect(), backgroundTileNumberIndex);
	
	int tileIndex = 0;
	
	if ((*_ioPorts).GetBackgroundAndWindowTileDataSelect())
	{
		tileIndex = (0x9000 - 0x8000) / Tile::DATA_SIZE + GetSignedValue(backgroundTileNumber);
	}
	else
	{
		tileIndex = (0x8000 - 0x8000) / Tile::DATA_SIZE + backgroundTileNumber;
	}
	
	DrawTile(position,
		     (*_videoRam).GetTile(tileIndex),
		     HorizontalFlip::NOT_FLIPPED,
		     VerticalFlip::NOT_FLIPPED,
		     (*_ioPorts).GetMonochromeBackgroundPalette(),
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
	Vector2<int> position((mapX * Tile::WIDTH) + (*_ioPorts).GetWindowX(),
	                      (*_ioPorts).GetLCDY() - (((*_ioPorts).GetLCDY() - (*_ioPorts).GetWindowY()) % Tile::HEIGHT));
	
	int mapElementX = mapX;
	int mapElementY = ((*_ioPorts).GetLCDY() - (*_ioPorts).GetWindowY()) / Tile::HEIGHT;
	
	if (mapElementX < Frame::WIDTH / Tile::WIDTH && mapElementY < Frame::HEIGHT / Tile::HEIGHT)
	{
		int backgroundTileNumberIndex = mapElementY * BackgroundTileNumberMap::WIDTH + mapElementX;
		int backgroundTileNumber = (*_videoRam).GetBackgroundTileNumber((*_ioPorts).GetWindowMapDisplaySelect(), backgroundTileNumberIndex);
		
		int tileIndex = 0;
		
		if ((*_ioPorts).GetBackgroundAndWindowTileDataSelect())
		{
			tileIndex = (0x9000 - 0x8000) / Tile::DATA_SIZE + GetSignedValue(backgroundTileNumber);
		}
		else
		{
			tileIndex = (0x8000 - 0x8000) / Tile::DATA_SIZE + backgroundTileNumber;
		}
		
		DrawTile(position,
		         (*_videoRam).GetTile(tileIndex),
		         HorizontalFlip::NOT_FLIPPED,
		         VerticalFlip::NOT_FLIPPED,
		         (*_ioPorts).GetMonochromeBackgroundPalette(),
		         enabledColors);
	}
}

void ClassicRenderer::DrawTile(Vector2<int> position,
                               Tile tile,
                               HorizontalFlip horizontalFlip,
                               VerticalFlip verticalFlip,
                               Array<Color<int>, 4> colorPalette,
                               int enabledColors)
{
	int tileY = (*_ioPorts).GetLCDY() - position.GetY();
	
	if (tileY >= 0 && tileY < Tile::HEIGHT)
	{
		Array2<int, Tile::WIDTH, Tile::HEIGHT> &tileArray = tile.GetArray();
		
		for (int tileX = 0; tileX < Tile::WIDTH; tileX++)
		{
			int scanlinePosition = position.GetX() + tileX;
			
			if (scanlinePosition >= 0 && scanlinePosition < Frame::WIDTH &&
			    (*_ioPorts).GetLCDY() >= 0 && (*_ioPorts).GetLCDY() < Frame::HEIGHT)
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
				
				int colorNumber = tileArray[realTileX][realTileY];
				
				if (((colorNumber == 0) && (enabledColors & COLOR_0)) ||
				    ((colorNumber == 1) && (enabledColors & COLOR_1)) ||
				    ((colorNumber == 2) && (enabledColors & COLOR_2)) ||
				    ((colorNumber == 3) && (enabledColors & COLOR_3)))
				{
					_frameBuffer[scanlinePosition][(*_ioPorts).GetLCDY()] = colorPalette[colorNumber];
				}
			}
		}
	}
}
