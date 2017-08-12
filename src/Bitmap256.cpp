/* Copyright (C) 1998-2000  Matthes Bender  RedWolf Design */

/* A structure for handling 256-color bitmap files */

#include <Standard.h>
#include <Bitmap256.h>

#include <cstring>

CBitmapInfo::CBitmapInfo()
{
	Default();
}

void CBitmapInfo::Default()
{
	std::memset(this, 0, sizeof(CBitmapInfo));
}

int CBitmapInfo::FileBitsOffset()
{
	return Head.bfOffBits - sizeof(CBitmapInfo);
}

CBitmap256Info::CBitmap256Info()
{
	Default();
}

int CBitmap256Info::FileBitsOffset()
{
	return Head.bfOffBits - sizeof(CBitmap256Info);
}

void CBitmap256Info::Set(int iWdt, int iHgt, uint8_t *bypPalette)
{
	Default();
	// Set header
	Head.bfType = *((uint16_t *)"BM");
	Head.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + DWordAligned(iWdt) * iHgt;
	Head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	// Set bitmap info
	Info.biSize = sizeof(BITMAPINFOHEADER);
	Info.biWidth = iWdt;
	Info.biHeight = iHgt;
	Info.biPlanes = 1;
	Info.biBitCount = 8;
	Info.biCompression = 0;
	Info.biSizeImage = iWdt * iHgt;
	Info.biClrUsed = Info.biClrImportant = 256;
	// Set palette
	for (int cnt = 0; cnt < 256; cnt++)
	{
		Colors[cnt].rgbRed   = bypPalette[cnt * 3 + 0];
		Colors[cnt].rgbGreen = bypPalette[cnt * 3 + 1];
		Colors[cnt].rgbBlue  = bypPalette[cnt * 3 + 2];
	}
}

void CBitmap256Info::Default()
{
	std::memset(this, 0, sizeof(CBitmap256Info));
}