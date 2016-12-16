#include "Point.h"

void Point::normalize()
{
	double norm = sqrt(_x * _x + _y * _y + _z * _z);
	_x = _x / norm;
	_y = _y / norm;
	_z = _z / norm;
}

float Point::norma()
{
	float norma2 = _x * _x + _y * _y + _z * _z;
	return  sqrt(norma2);
}

Point Point::reflect(Point p2)
{
	Point p1 = *this;
	Point r = (p2 * 2 * (p1 * p2)) - p1;
	return r;

}

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

void Point::SetX(float x)
{
	_x = x;
}

void Point::SetY(float y)
{
	_y = y;
}

void Point::SetZ(float z)
{
	_z = z;
}

Point Point::produto_vetorial(Point p2)
{
	Point p1 = *this;
	float x = p1._y * p2._z - p1._z * p2._y;
	float y = p1._z * p2._x - p1._x * p2._z;
	float z = p1._x * p2._y - p1._y * p2._x;

	Point result = Point(x, y, z);
	return result;
	
}



float Point::operator*(const Point& p) const
{
	float product = _x*p._x + _y*p._y + _z*p._z;
	return product;
}



Point Point::operator*(const float t) const
{
	float tx = _x*t;
	float ty = _y*t;
	float tz = _z*t;
	
	Point result(tx, ty, tz);
	return result;
}

Point Point::operator-(const Point& p) const
{
	float x = _x - p._x;
	float y = _y - p._y;
	float z = _z - p._z;

	Point result(x, y, z);
	return result;
}

//AMK
Point Point::operator+(const Point& p) const
{
	float x = _x + p._x;
	float y = _y + p._y;
	float z = _z + p._z;

	Point result(x, y, z);
	return result;
}
