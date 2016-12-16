#include "Ray.h"

Ray::Ray()
{
	o = Point();
	d = Point();
}

//classe raio. 
Ray::Ray(Point o, Point d)
{
	this->o = o;
	this->d = d;
}

//FIM. Classe raio