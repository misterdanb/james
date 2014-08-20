#include "Frame.hpp"

Frame::Frame()
	: PixelMap<WIDTH, HEIGHT>(), data (_matrixElements)
{
}

Frame::Frame (Color<int> gameboyColors[])
	: PixelMap<WIDTH, HEIGHT>(), data (_matrixElements)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int pixelIndex = y * WIDTH + x;

			data[x][y].SetRed (((gameboyColors[pixelIndex].GetRed() << 3) |
								(gameboyColors[pixelIndex].GetRed() >> 2)) & 0xFF);
			data[x][y].SetGreen (((gameboyColors[pixelIndex].GetGreen() << 3) |
								  (gameboyColors[pixelIndex].GetGreen() >> 2)) & 0xFF);
			data[x][y].SetBlue (((gameboyColors[pixelIndex].GetBlue() << 3) |
								 (gameboyColors[pixelIndex].GetBlue() >> 2)) & 0xFF);
		}
	}
}

Frame::~Frame()
{
}
