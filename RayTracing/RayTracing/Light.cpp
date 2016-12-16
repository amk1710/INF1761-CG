#include "Light.h"

Light::Light()
{
	position = Point();
	intensity = Pixel();
}

Light::Light(Point pos, Pixel intens)
{
	position = pos;
	intensity = intens;
}
