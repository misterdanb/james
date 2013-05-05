#include "ClassicRenderer.hpp"

using namespace gbc;
using namespace gbc::core;

ClassicRenderer::ClassicRenderer()
{
}

ClassicRenderer::~ClassicRenderer()
{
}

void ClassicRenderer::RenderOAMSearch(RenderContext &rc)
{
	rc.lcdMode = LCDMode::SEARCHING_OAM;
	
	rc.coincidenceFlag = (rc.lcdY == rc.lcdYCompare);
	
	if (rc.oamInterruptEnabled)
	{
		rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	if (rc.coincidenceInterruptEnabled && rc.coincidenceFlag)
	{
		rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	UpdateSpriteAttributes(rc);
}

void ClassicRenderer::RenderTransferData(RenderContext &rc)
{
	rc.lcdMode = LCDMode::TRANSFERRING_DATA;
	
	UpdateTiles(rc);
	UpdateBackgroundMapElements(rc);
	UpdateSpriteAttributes(rc);
	
	if (rc.lcdDisplayEnabled)
	{
		if (rc.backgroundDisplayEnabled)
		{
			DrawBackgroundMap(COLOR_0, rc);
		}
		
		if (rc.windowDisplayEnabled)
		{
			DrawWindowMap(COLOR_0, rc);
		}
		
		DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_BEHIND_BACKGROUND, rc);
		
		if (rc.backgroundDisplayEnabled)
		{
			DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3, rc);
		}
		
		if (rc.windowDisplayEnabled)
		{
			DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3, rc);
		}
		
		DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_ABOVE_BACKGROUND, rc);
	}
}

void ClassicRenderer::RenderHorizontalBlank(RenderContext &rc)
{
	rc.lcdMode = LCDMode::HORIZONTAL_BLANK;
	
	if (rc.horizontalBlankInterruptEnabled)
	{
		rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	if (rc.lcdY < 144)
	{
		rc.lcdY++;
	}
}

void ClassicRenderer::RenderVerticalBlank(RenderContext &rc)
{
	rc.lcdMode = LCDMode::VERTICAL_BLANK;
	
	rc.coincidenceFlag = (rc.lcdY == rc.lcdYCompare);
	
	if (rc.coincidenceInterruptEnabled && rc.coincidenceFlag)
	{
		rc.interruptHandler->SignalLCDStatusInterrupt();
	}
	
	if (rc.verticalBlankInterruptEnabled && !rc.verticalBlankInterruptAlreadyRequested)
	{
		rc.interruptHandler->SignalVBlankInterrupt();
		rc.verticalBlankInterruptAlreadyRequested = GBC_TRUE;
	}
	
	if (rc.lcdY >= 144)
	{
		rc.lcdY++;
	}
	
	if (rc.lcdY > 153)
	{
		rc.lcdY = 0;
		rc.verticalBlankInterruptAlreadyRequested = GBC_FALSE;
	}
}

void ClassicRenderer::DrawSprites(int enabledColors,
                                  SpriteToBackgroundPriority spriteToBackgroundPriority,
                                  RenderContext &rc)
{
	RenderContext::GameboyClassicSpecificRenderContext &rcClassic = rc.gameboyClassicSpecific;
	
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = rc.spriteAttributes[i];
		
		if (spriteAttribute.spriteToBackgroundPriority == spriteToBackgroundPriority)
		{
			ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                            rcClassic.monochromeSpritePalette0 :
			                            rcClassic.monochromeSpritePalette1;
			
			DrawTile(spriteAttribute.x,
			         spriteAttribute.y,
					 rc.tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
					 spriteAttribute.horizontalFlip,
					 spriteAttribute.verticalFlip,
					 colorPalette,
					 enabledColors,
					 rc);
		}
	}
}

void ClassicRenderer::DrawSprites(int enabledColors,
                                             RenderContext &rc)
{
	RenderContext::GameboyClassicSpecificRenderContext &rcClassic = rc.gameboyClassicSpecific;
	
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = rc.spriteAttributes[i];
		
		int tileNumber = (0x8000 - 0x8000) + spriteAttribute.tileNumber;
		
		ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                        rcClassic.monochromeSpritePalette0 :
			                        rcClassic.monochromeSpritePalette1;
		
		DrawTile(spriteAttribute.x,
		         spriteAttribute.y,
				 rc.tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
				 spriteAttribute.horizontalFlip,
				 spriteAttribute.verticalFlip,
				 colorPalette,
				 enabledColors,
				 rc);
	}
}

void ClassicRenderer::DrawBackgroundMap(int enabledColors,
                                        RenderContext &rc)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		DrawMapTile(mapX,
		            -rc.scrollX,
		            -rc.scrollY, 
		            rc.backgroundTileMapDisplaySelect,
		            enabledColors,
		            rc);
	}
}

void ClassicRenderer::DrawWindowMap(int enabledColors,
                                    RenderContext &rc)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		DrawMapTile(mapX,
		            rc.windowX,
		            rc.windowY,
		            rc.windowTileMapDisplaySelect,
		            enabledColors,
		            rc);
	}
}

void ClassicRenderer::DrawMapTile(int mapX,
                                  int xOffset,
                                  int yOffset,
                                  int tileMapDisplaySelect,
                                  int enabledColors,
                                  RenderContext &rc)
{
	RenderContext::GameboyClassicSpecificRenderContext &rcClassic = rc.gameboyClassicSpecific;
	
	int x = (mapX * Tile::WIDTH) + xOffset;
	int y = (rc.lcdY - yOffset) - ((rc.lcdY - yOffset) % Tile::HEIGHT) + yOffset;
	
	int mapElementX = ((mapX * Tile::WIDTH) - xOffset) / Tile::WIDTH;
	int mapElementY = (rc.lcdY - yOffset) / Tile::HEIGHT;
	
	if (mapElementX < 0)
	{
		mapElementX += TileMap::WIDTH;
	}
	
	if (mapElementY < 0)
	{
		mapElementY += TileMap::HEIGHT;
	}
	
	mapElementX %= TileMap::WIDTH;
	mapElementY %= TileMap::HEIGHT;
	
	int mapElementNumber = (mapElementY * TileMap::WIDTH) + mapElementX;
	
	int backgroundMapElement = rc.tileMaps
		                       [tileMapDisplaySelect].data
		                       [mapElementX][mapElementY];
	
	int tileVideoRamBankNumber = 0;
	
	int tileNumber = (!rc.backgroundAndWindowTileDataSelect) ?
		             (((0x9000 - 0x8000) / 16) + GetSignedValue(backgroundMapElement)) :
		             (((0x8000 - 0x8000) / 16) + backgroundMapElement);
	
	Tile tile = rc.tiles[tileVideoRamBankNumber][tileNumber];
	
	ColorPalette colorPalette = rcClassic.monochromeBackgroundPalette;
	
	DrawTile(x,
	         y,
		     tile,
		     HorizontalFlip::NOT_FLIPPED,
		     VerticalFlip::NOT_FLIPPED,
		     colorPalette,
		     enabledColors,
		     rc);
}

void ClassicRenderer::DrawTile(int x,
                               int y,
                               Tile tile,
                               HorizontalFlip horizontalFlip,
                               VerticalFlip verticalFlip,
                               ColorPalette colorPalette,
                               int enabledColors,
                               RenderContext &rc)
{
	int tileY = rc.lcdY - y;
	
	if (tileY >= 0 && tileY < Tile::HEIGHT)
	{
		for (int tileX = 0; tileX < Tile::WIDTH; tileX++)
		{
			int scanlinePosition = x + tileX;
			
			if (scanlinePosition >= 0 && scanlinePosition < Frame::WIDTH &&
			    rc.lcdY >= 0 && rc.lcdY < Frame::HEIGHT)
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
					rc.rawFrame[rc.lcdY * Frame::WIDTH + scanlinePosition] = colorPalette.colors[colorNumber];
				}
			}
		}
	}
}
