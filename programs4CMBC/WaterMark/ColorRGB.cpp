#include "ColorRGB.h"

int min(int l, int r)
{
	if(l < r)
		return l;
	return r;
}

ColorRGB::ColorRGB(void)
{
}

ColorRGB::ColorRGB(int r, int g, int b)
{
	R = r;
	G = g;
	B = b;
}

ColorRGB::~ColorRGB(void)
{
}

ColorRGB ColorRGB::operator + (ColorRGB& tmp)
{
    int int_R = R;
	int int_G = G;
	int int_B = B;
	int tmp_R = tmp.R;
	int tmp_G = tmp.G;
	int tmp_B = tmp.B;
	int_R = min(int_R + tmp_R, 255);
	int_G = min(int_G + tmp_G, 255);
	int_B = min(int_B + tmp_B, 255);
	return ColorRGB(int_R, int_G, int_B);
}

ColorRGB ColorRGB::operator * (double a)
{
	int int_R = R;
	int int_G = G;
	int int_B = B;
	int_R = min(int_R * a, 255);
	int_G = min(int_G * a, 255);
	int_B = min(int_B * a, 255);
	return ColorRGB(int_R, int_G, int_B);
}

bool ColorRGB::operator != (ColorRGB& c)
{
	return (R != c.R || G != c.G || B != c.B);
}

bool ColorRGB::operator == (ColorRGB& c)
{
	return (R == c.R && G == c.G && B == c.B);
}