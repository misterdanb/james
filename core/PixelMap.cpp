#include "PixelMap.hpp"

gbc::core::PixelMap::PixelMap(int width, int height)
	: _width(width),
	  _height(height),
	  _rawMap(new Color[width * height])
{
}

gbc::core::PixelMap::~PixelMap()
{
	delete[] _rawMap;
}

int gbc::core::PixelMap::GetWidth()
{
	return _width;
}

int gbc::core::PixelMap::GetHeight()
{
	return _height;
}

gbc::core::Color gbc::core::PixelMap::GetPixel(int x, int y)
{
	return _rawMap[y * _width + x];
}

gbc::core::Color *gbc::core::PixelMap::GetRawMap()
{
	return _rawMap;
}
