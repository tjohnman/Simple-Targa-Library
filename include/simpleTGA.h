// At present will only work on Little Endian machines.
// Image starts at lower-left corner (0, 0).
// Supports 24/32 bit images only.

#include <fstream>
#include <iostream>
#include <cstring>

struct TGA_Color
{
	uint8_t r, g, b, a;

	TGA_Color()
	{
		r = g = b = 0;
		a = 255;
	}

	TGA_Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
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

	uint32_t toBytes()
	{
		// If support for Big Endian hosts is implemented,
		// this needs to change (possibly among other things).

		return b + g*256 + r*65536 + a*16777216;
	}
};

class TGAImage
{
public:
	TGAImage(uint16_t w, uint16_t h);
	~TGAImage();

	bool save(const char * path);
	bool load(const char * path);

	void set(uint16_t x, uint16_t y, TGA_Color color);
	void set(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	TGA_Color get(uint16_t x, uint16_t y);
	TGA_Color * colorMap();
	uint32_t * pixels();
	void loadPixels(uint32_t * memory);
private:
	TGA_Color * _mColorMap;
	uint16_t _mWidth, _mHeight;
	uint32_t * _mPixels;
};