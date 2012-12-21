// At present will only work on Little Endian machines
// "unsigned short" must be 2 bytes
// Image starts at lower-left corner (0, 0)

#include <fstream>
#include <iostream>

struct TGA_Color
{
	unsigned char r, g, b, a;

	TGA_Color()
	{
		r = g = b = a = 0;
	}

	TGA_Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	TGA_Color & operator=(TGA_Color other)
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
		return *this;
	}

	unsigned int toBytes()
	{
		// If support for Big Endian hosts is implemented,
		// this needs to change (possibly among other things).

		return b + g*256 + r*65536 + a*16777216;
	}
};

class TGAImage
{
public:
	TGAImage(unsigned short w, unsigned short h);
	~TGAImage();

	bool save(const char * path);
	bool load(const char * path);

	void set(unsigned short x, unsigned short y, TGA_Color color);
	void set(unsigned short x, unsigned short y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	TGA_Color get(unsigned short x, unsigned short y);
	TGA_Color * colorMap();
private:
	TGA_Color * _mColorMap;
	unsigned short _mWidth, _mHeight;
};