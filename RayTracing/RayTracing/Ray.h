#pragma once
#ifndef _RAY
#define _RAY

#include "Point.h"

class Ray
{
public:

	Ray();
	Ray(Point eye, Point d);
	Point o;
	Point d;
};

#endif