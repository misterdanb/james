#include "Scanline.hpp"

gbc::core::Scanline::Scanline(Color gameboyColors[])
	: _gameboyColors(gameboyColors), _realColors(new Color[sizeof(gameboyColors) / sizeof(Color)])
{
}

gbc::core::Scanline::~Scanline()
{
	delete[] _gameboyColors;
	delete[] _realColors;
}

Color *gbc::core::Scanline::GetGameboyColors()
{
	return _gameboyColors;
}

Color *gbc::core::Scanline::GetRealColors()
{
	return _realColors;
}

int gbc::core::Scanline::GetLength()
{
	return sizeof(_gameboyColors) / sizeof(Color);
}
