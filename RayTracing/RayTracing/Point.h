#pragma once
#ifndef _POINT
#define _POINT

class Point 
{
public:
	float getX();
	float getY();
	float getZ();

	Point(float x, float y, float z);

	Point(float x, float y);

	Point();
private:
	float _x;
	float _y;
	float _z;
};

#endif