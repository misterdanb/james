#include "BackgroundAttribute.hpp"

using namespace james;
using namespace james::core;

BackgroundAttribute::BackgroundAttribute()
	: _backgroundColorPaletteNumber(0),
	  _tileVideoRamBankNumber(0),
	  _horizontalFlip(HorizontalFlip::NOT_FLIPPED),
	  _verticalFlip(VerticalFlip::NOT_FLIPPED),
	  _backgroundToOAMPriority(BackgroundToOAMPriority::USE_OAM_PRIORITY_BIT)
{
}

BackgroundAttribute::BackgroundAttribute(int backgroundAttributeData)
	: _backgroundColorPaletteNumber(0),
	  _tileVideoRamBankNumber(0),
	  _horizontalFlip(HorizontalFlip::NOT_FLIPPED),
	  _verticalFlip(VerticalFlip::NOT_FLIPPED),
	  _backgroundToOAMPriority(BackgroundToOAMPriority::USE_OAM_PRIORITY_BIT)
{
	SetData(backgroundAttributeData);
}

BackgroundAttribute::~BackgroundAttribute()
{
}

void BackgroundAttribute::SetData(int backgroundAttributeData)
{
	_backgroundColorPaletteNumber = backgroundAttributeData & 0x07;
	_tileVideoRamBankNumber = (backgroundAttributeData >> 3) & 0x01;
	_horizontalFlip = HorizontalFlip((backgroundAttributeData >> 5) & 0x01);
	_verticalFlip = VerticalFlip((backgroundAttributeData >> 6) & 0x01);
	_backgroundToOAMPriority = BackgroundToOAMPriority((backgroundAttributeData >> 7) & 0x01);
}

void BackgroundAttribute::SetBackgroundColorPaletteNumber(int backgroundColorPaletteNumber)
{
	_backgroundColorPaletteNumber = backgroundColorPaletteNumber;
}

void BackgroundAttribute::SetTileVideoRamBankNumber(int tileVideoRamBankNumber)
{
	_tileVideoRamBankNumber = tileVideoRamBankNumber;
}

void BackgroundAttribute::SetHorizontalFlip(HorizontalFlip horizontalFlip)
{
	_horizontalFlip = horizontalFlip;
}

void BackgroundAttribute::SetVerticalFlip(VerticalFlip verticalFlip)
{
	_verticalFlip = verticalFlip;
}

void BackgroundAttribute::SetBackgroundToOAMPriority(BackgroundToOAMPriority backgroundToOAMPriority)
{
	_backgroundToOAMPriority = backgroundToOAMPriority;
}

int BackgroundAttribute::GetData()
{
	int backgroundAttributeData = 0x00;
	
	backgroundAttributeData |= _backgroundColorPaletteNumber & 0x07;
	backgroundAttributeData |= (_tileVideoRamBankNumber & 0x01) << 3;
	backgroundAttributeData |= GetEnumValue(_horizontalFlip) << 5;
	backgroundAttributeData |= GetEnumValue(_verticalFlip) << 6;
	backgroundAttributeData |= GetEnumValue(_backgroundToOAMPriority) << 7;
	
	return backgroundAttributeData;
}

int &BackgroundAttribute::GetBackgroundColorPaletteNumber()
{
	return _backgroundColorPaletteNumber;
}

int &BackgroundAttribute::GetTileVideoRamBankNumber()
{
	return _tileVideoRamBankNumber;
}

HorizontalFlip &BackgroundAttribute::GetHorizontalFlip()
{
	return _horizontalFlip;
}

VerticalFlip &BackgroundAttribute::GetVerticalFlip()
{
	return _verticalFlip;
}

BackgroundToOAMPriority &BackgroundAttribute::GetBackgroundToOAMPriority()
{
	return _backgroundToOAMPriority;
}