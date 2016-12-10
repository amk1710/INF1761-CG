#include "RayTracing.h"

#include <iostream>

RayTracing::RayTracing(char * path)
{
	_path = path;
}

void RayTracing::Render(Image & l)
{
	int w = 480, h = 720;
	l = Image(w, h);
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			Pixel p;
			p[0] = 0.7;
			p[1] = 0.3;
			p[2] = 0.9;
			l.setPixel(i, j, p);
		}
	}

	Material mat = Material("aco", Point(0.5, 0.5, 0.5), Point(1.0, 1.0, 1.0), 50);

	Sphere c = Sphere(Point(0.0, 5.0, 7.5), 10.0, mat);

	std::cout << "ret:" << c.intercept().getX() << " ," << c.intercept().getY() << " ," << c.intercept().getZ() << std::endl;

	return;
}
