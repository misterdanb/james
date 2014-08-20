#include "SpriteAttribute.hpp"

using namespace james;
using namespace james::core;

SpriteAttribute::SpriteAttribute()
	: _y (0),
	  _x (0),
	  _tileNumber (0),
	  _colorPaletteNumber (0),
	  _tileVideoRamBankNumber (0),
	  _monochromePaletteNumber (0),
	  _horizontalFlip (HorizontalFlip::NOT_FLIPPED),
	  _verticalFlip (VerticalFlip::NOT_FLIPPED),
	  _spriteToBackgroundPriority (SpriteToBackgroundPriority::SPRITE_ABOVE_BACKGROUND)
{
}

SpriteAttribute::SpriteAttribute (const SpriteAttribute& spriteAttribute)
	: _y (spriteAttribute.GetY()),
	  _x (spriteAttribute.GetX()),
	  _tileNumber (spriteAttribute.GetTileNumber()),
	  _colorPaletteNumber (spriteAttribute.GetColorPaletteNumber()),
	  _tileVideoRamBankNumber (spriteAttribute.GetTileVideoRamBankNumber()),
	  _monochromePaletteNumber (spriteAttribute.GetMonochromePaletteNumber()),
	  _horizontalFlip (spriteAttribute.GetHorizontalFlip()),
	  _verticalFlip (spriteAttribute.GetVerticalFlip()),
	  _spriteToBackgroundPriority (spriteAttribute.GetSpriteToBackgroundPriority())
{
}

SpriteAttribute::SpriteAttribute (Array<int, 4> spriteAttributeData)
{
	SetData (spriteAttributeData);
}

SpriteAttribute::~SpriteAttribute()
{
}

void SpriteAttribute::SetData (Array<int, 4> spriteAttributeData)
{
	_y = spriteAttributeData[0] - 16;
	_x = spriteAttributeData[1] - 8;
	_tileNumber = spriteAttributeData[2];

	_colorPaletteNumber = spriteAttributeData[3] & 0x07;
	_tileVideoRamBankNumber = (spriteAttributeData[3] >> 3) & 0x01;
	_monochromePaletteNumber = (spriteAttributeData[3] >> 4) & 0x01;
	_horizontalFlip = HorizontalFlip ((spriteAttributeData[3] >> 5) & 0x01);
	_verticalFlip = VerticalFlip ((spriteAttributeData[3] >> 6) & 0x01);
	_spriteToBackgroundPriority = SpriteToBackgroundPriority ((spriteAttributeData[3] >> 7) & 0x01);
}

void SpriteAttribute::SetX (int x)
{
	_x = x;
}

void SpriteAttribute::SetY (int y)
{
	_y = y;
}

void SpriteAttribute::SetTileNumber (int tileNumber)
{
	_tileNumber = tileNumber;
}

void SpriteAttribute::SetColorPaletteNumber (int colorPaletteNumber)
{
	_colorPaletteNumber = colorPaletteNumber;
}

void SpriteAttribute::SetTileVideoRamBankNumber (int tileVideoRamBankNumber)
{
	_tileVideoRamBankNumber = tileVideoRamBankNumber;
}

void SpriteAttribute::SetMonochromePaletteNumber (int monochromePaletteNumber)
{
	_monochromePaletteNumber = monochromePaletteNumber;
}

void SpriteAttribute::SetHorizontalFlip (HorizontalFlip horizontalFlip)
{
	_horizontalFlip = horizontalFlip;
}

void SpriteAttribute::SetVerticalFlip (VerticalFlip verticalFlip)
{
	_verticalFlip = verticalFlip;
}

void SpriteAttribute::SetSpriteToBackgroundPriority (SpriteToBackgroundPriority spriteToBackgroundPriority)
{
	_spriteToBackgroundPriority = spriteToBackgroundPriority;
}

Array<int, 4> SpriteAttribute::GetData() const
{
	int spriteAttributeFlags = 0x00;

	spriteAttributeFlags |= _colorPaletteNumber;
	spriteAttributeFlags |= _tileVideoRamBankNumber << 3;
	spriteAttributeFlags |= _monochromePaletteNumber << 4;
	spriteAttributeFlags |= GetEnumValue (_horizontalFlip) << 5;
	spriteAttributeFlags |= GetEnumValue (_verticalFlip) << 6;
	spriteAttributeFlags |= GetEnumValue (_spriteToBackgroundPriority) << 7;

	Array<int, 4> spriteAttributeData = { _y, _x, _tileNumber, spriteAttributeFlags };

	return spriteAttributeData;
}

int SpriteAttribute::GetX() const
{
	return _x;
}

int SpriteAttribute::GetY() const
{
	return _y;
}

int SpriteAttribute::GetTileNumber() const
{
	return _tileNumber;
}

int SpriteAttribute::GetColorPaletteNumber() const
{
	return _colorPaletteNumber;
}

int SpriteAttribute::GetTileVideoRamBankNumber() const
{
	return _tileVideoRamBankNumber;
}

int SpriteAttribute::GetMonochromePaletteNumber() const
{
	return _monochromePaletteNumber;
}

HorizontalFlip SpriteAttribute::GetHorizontalFlip() const
{
	return _horizontalFlip;
}

VerticalFlip SpriteAttribute::GetVerticalFlip() const
{
	return _verticalFlip;
}

SpriteToBackgroundPriority SpriteAttribute::GetSpriteToBackgroundPriority() const
{
	return _spriteToBackgroundPriority;
}