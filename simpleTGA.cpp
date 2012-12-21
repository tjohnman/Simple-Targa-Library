#include "simpleTGA.h"

TGAImage::TGAImage(unsigned short w, unsigned short h)
{
	_mWidth = w;
	_mHeight = h;

	_mColorMap = new TGA_Color[w*h];
}

TGAImage::~TGAImage()
{
	delete _mColorMap;
}


bool TGAImage::save(const char * path)
{
	std::fstream stream;
	stream.open(path, std::fstream::out | std::fstream::binary);

	// Simple uncompressed true-color image
	stream.write("\0\0\2", 3);

	// Color map
	stream.write("\0\0\0\0\0", 5);

	// Image specification
	stream.write("\0\0\0\0", 4); // Origin
	stream.write((const char *)&_mWidth, sizeof(_mWidth)); // width
	stream.write((const char *)&_mHeight, sizeof(_mHeight)); // height
	stream.put(32); // bpp
	stream.put(8);

	// Data section
	unsigned int bytes = 0;

	for(int y=0; y<_mHeight; ++y)
	{
		for(int x=0; x<_mWidth; x++)
		{
			bytes = _mColorMap[x+y*_mWidth].toBytes();
			
			stream.write((const char*)&bytes, sizeof(bytes));
		}
	}

	stream.close();

	return true;
}

TGA_Color * TGAImage::colorMap()
{
	return _mColorMap;
}

bool TGAImage::load(const char * path)
{
	// TODO: Implementation
}

void TGAImage::set(unsigned short x, unsigned short y, TGA_Color color)
{
	_mColorMap[x+y*_mWidth] = color;
}

void TGAImage::set(unsigned short x, unsigned short y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_mColorMap[x+y*_mWidth].r = r;
	_mColorMap[x+y*_mWidth].g = g;
	_mColorMap[x+y*_mWidth].b = b;
	_mColorMap[x+y*_mWidth].a = a;
}

TGA_Color TGAImage::get(unsigned short x, unsigned short y)
{
	return _mColorMap[x+y*_mWidth];
}
