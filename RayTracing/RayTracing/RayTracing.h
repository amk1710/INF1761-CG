#pragma once
#pragma once
#ifndef _RAYTRACING
#define _RAYTRACING

#include "Image.h"
#include "Object.h"

class RayTracing
{
public:
	
	//construtor prepara renderização
	RayTracing(char* path);

	//renderiza
	void Render(Image& l);


private:
	char* _path;
};

#endif