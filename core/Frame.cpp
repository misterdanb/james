#include "Frame.hpp"

Frame::Frame()
	: PixelMap<WIDTH, HEIGHT>()
{
}

Frame::Frame(Array2<Color<int>, WIDTH, HEIGHT> &frameData)
	: PixelMap<WIDTH, HEIGHT>()
{
	/*for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int pixelIndex = y * WIDTH + x;
			
			data[x][y].red = ((gameboyColors[pixelIndex].red << 3) | (gameboyColors[pixelIndex].red >> 2)) & 0xFF,
			data[x][y].green = ((gameboyColors[pixelIndex].green << 3) | (gameboyColors[pixelIndex].green >> 2)) & 0xFF,
			data[x][y].blue = ((gameboyColors[pixelIndex].blue << 3) | (gameboyColors[pixelIndex].blue >> 2)) & 0xFF;
		}
	}*/
	
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			_matrixElements[x][y].SetRed(frameData[x][y].GetRed());
			_matrixElements[x][y].SetGreen(frameData[x][y].GetGreen());
			_matrixElements[x][y].SetBlue(frameData[x][y].GetBlue());
		}
	}
}

Frame::~Frame()
{
}
