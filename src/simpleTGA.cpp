#include "simpleTGA.h"

TGAImage * TGA_Load(const char * path)
{
	std::fstream stream;
	stream.open(path, std::fstream::in | std::fstream::binary);

	stream.seekg(0, std::ios_base::end);
	size_t length = stream.tellg();
	stream.seekg(0, std::ios_base::beg);

	// Simple uncompressed true-color image
	char dumpBuffer[12];
	uint8_t trueColorHeader[] = "\0\0\2\0\0\0\0\0\0\0\0\0";
	stream.read(dumpBuffer, 12);
	if(memcmp(dumpBuffer, trueColorHeader, 12) != 0)
	{
		std::cout << "TGA: Unsupported format or invalid file.\n";
		return NULL;
	}

	uint16_t width, height;
	uint8_t bpp;

	stream.read((char *)&width, 2);
	stream.read((char *)&height, 2);
	bpp = stream.get();

	uint8_t desc;
	desc = stream.get();
	if(desc != 8)
	{
		std::cout << "TGA: Unsupported format or invalid file.\n";
		return NULL;
	}

	// Data section
	uint32_t * bytes = new uint32_t[width*height];
	stream.read((char *)bytes, width*height*4);
	stream.close();

	TGAImage * newImage = new TGAImage(width, height);
	newImage->loadPixels(bytes);
	delete bytes;

	return newImage;
}

// ----------- //

TGAImage::TGAImage(uint16_t w, uint16_t h)
{
	_mWidth = w;
	_mHeight = h;

	_mColorMap = new TGA_Color[w*h];
	_mPixels = new uint32_t[w*h];
}

TGAImage::~TGAImage()
{
	delete _mColorMap;
	delete _mPixels;
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
	stream.write((char *)&_mWidth, sizeof(_mWidth)); // width
	stream.write((char *)&_mHeight, sizeof(_mHeight)); // height
	stream.put(32); // bpp
	stream.put(8);

	// Data section
	uint32_t * bytes = pixels();
	stream.write((char *)bytes, sizeof(bytes));

	stream.close();

	return true;
}

TGA_Color * TGAImage::colorMap()
{
	return _mColorMap;
}

void TGAImage::set(uint16_t x, uint16_t y, TGA_Color color)
{
	_mColorMap[x+y*_mWidth] = color;
}

void TGAImage::set(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	_mColorMap[x+y*_mWidth].r = r;
	_mColorMap[x+y*_mWidth].g = g;
	_mColorMap[x+y*_mWidth].b = b;
	_mColorMap[x+y*_mWidth].a = a;
}

TGA_Color TGAImage::get(uint16_t x, uint16_t y)
{
	return _mColorMap[x+y*_mWidth];
}

unsigned int * TGAImage::pixels()
{
	for(int i=0; i<_mWidth*_mHeight; ++i)
	{
		_mPixels[i] = _mColorMap[i].toBytes();
	}
	return _mPixels;
}

void TGAImage::loadPixels(uint32_t * memory)
{
	memcpy(memory, _mPixels, 4);
}
