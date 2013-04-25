#include "Frame.hpp"

gbc::core::Frame::Frame()
	: PixelMap(WIDTH, HEIGHT)
{
}

gbc::core::Frame::Frame(Color gameboyColors[])
	: PixelMap(WIDTH, HEIGHT)
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			int pixelIndex = y * _width + x;
			
			_rawMap[pixelIndex].red = ((gameboyColors[pixelIndex].red << 3) | (gameboyColors[pixelIndex].red >> 2)) & 0xFF,
			_rawMap[pixelIndex].green = ((gameboyColors[pixelIndex].green << 3) | (gameboyColors[pixelIndex].green >> 2)) & 0xFF,
			_rawMap[pixelIndex].blue = ((gameboyColors[pixelIndex].blue << 3) | (gameboyColors[pixelIndex].blue >> 2)) & 0xFF;
		}
	}
}

gbc::core::Frame::~Frame()
{
}
