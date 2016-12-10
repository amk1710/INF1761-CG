#include "Point.h"

Point::Point(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

Point::Point(float x, float y)
{
	_x = x;
	_y = y;
	_z = 0.0;
}

Point::Point()
{
	_x = 0;
	_y = 0;
	_z = 0;
}

float Point::getX()
{
	return _x;
}

float Point::getY()
{
	return _y;
}

float Point::getZ()
{
	return _z;
}
