#include "Scanline.hpp"

gbc::core::Scanline::Scanline()
	: _lineIndex(0), _gameboyColors(new Color[160]), _realColors(new Color[160])
{
}

gbc::core::Scanline::Scanline(int lineIndex, Color gameboyColors[])
	: _lineIndex(lineIndex), _gameboyColors(new Color[160]), _realColors(new Color[160])
{
	for (int i = 0; i < 160; i++)
	{
		_gameboyColors[i].red = gameboyColors[i].red;
		_gameboyColors[i].green = gameboyColors[i].green;
		_gameboyColors[i].blue = gameboyColors[i].blue;
		
		_realColors[i].red = ((gameboyColors[i].red << 3) | (gameboyColors[i].red >> 2)) & 0xFF,
		_realColors[i].green = ((gameboyColors[i].green << 3) | (gameboyColors[i].green >> 2)) & 0xFF,
		_realColors[i].blue = ((gameboyColors[i].blue << 3) | (gameboyColors[i].blue >> 2)) & 0xFF;
	}
}

gbc::core::Scanline::~Scanline()
{
	delete[] _gameboyColors;
	delete[] _realColors;
}

int gbc::core::Scanline::GetLineIndex()
{
	return _lineIndex;
}

gbc::core::Color *gbc::core::Scanline::GetGameboyColors()
{
	return _gameboyColors;
}

gbc::core::Color *gbc::core::Scanline::GetRealColors()
{
	return _realColors;
}

int gbc::core::Scanline::GetLength()
{
	return 160;
}
