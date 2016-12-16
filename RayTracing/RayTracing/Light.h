#pragma once
#ifndef _LIGHT
#define _LIGHT

#include "Point.h"
#include "Pixel.h"

class Light
{
public:

	Light();
	Light(Point pos, Pixel intens);

	Point position;
	Pixel intensity;
	
};

#endif
