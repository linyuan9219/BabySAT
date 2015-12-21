#pragma once

#include <stdio.h>
#include <algorithm>
#include <stdlib.h>

class ColorRGB
{
public:
	unsigned char B, G, R;

	ColorRGB(void);
	ColorRGB(int, int, int);
	~ColorRGB(void);

	ColorRGB operator + (ColorRGB& tmp);
	ColorRGB operator * (double);
	bool operator != (ColorRGB&);
	bool operator == (ColorRGB& c);
};

