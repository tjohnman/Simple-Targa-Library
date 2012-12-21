#include "simpleTGA.h"

TGAImage * TGA_Load(const char * path)
{
	std::fstream stream;
	stream.open(path, std::fstream::in | std::fstream::binary);

	stream.seekg(0, std::ios_base::end);
	long length = stream.tellg();
	stream.seekg(0, std::ios_base::beg);

	// Simple uncompressed true-color image
	char dumpBuffer[12];
	stream.read(dumpBuffer, 12);
	if(strcmp(dumpBuffer, "\0\0\2\0\0\0\0\0\0\0\0\0") != 0)
	{
		std::cout << "TGA: Unsupported format or invalid file.\n";
		return NULL;
	}

	unsigned short width, height;
	unsigned char bpp;

	stream.read((char *)&width, 2);
	stream.read((char *)&height, 2);
	bpp = stream.get();

	unsigned char desc;
	desc = stream.get();
	if(desc != 8)
	{
		std::cout << "TGA: Unsupported format or invalid file.\n";
		return NULL;
	}

	// Data section
	unsigned int * bytes = new unsigned int[width*height];
	stream.read((char *)bytes, width*height*4);
	stream.close();

	TGAImage * newImage = new TGAImage(width, height);
	newImage->loadPixels(bytes);
	delete bytes;

	return newImage;
}

// ----------- //

TGAImage::TGAImage(unsigned short w, unsigned short h)
{
	_mWidth = w;
	_mHeight = h;

	_mColorMap = new TGA_Color[w*h];
	_mPixels = new unsigned int[w*h];
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
	stream.write((const char *)&_mWidth, sizeof(_mWidth)); // width
	stream.write((const char *)&_mHeight, sizeof(_mHeight)); // height
	stream.put(32); // bpp
	stream.put(8);

	// Data section
	unsigned int * bytes = pixels();
	stream.write((const char *)bytes, sizeof(bytes));

	stream.close();

	return true;
}

TGA_Color * TGAImage::colorMap()
{
	return _mColorMap;
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

unsigned int * TGAImage::pixels()
{
	for(int i=0; i<_mWidth*_mHeight; ++i)
	{
		_mPixels[i] = _mColorMap[i].toBytes();
	}
	return _mPixels;
}

void TGAImage::loadPixels(unsigned int * memory)
{
	memcpy(memory, _mPixels, sizeof(memory));
}