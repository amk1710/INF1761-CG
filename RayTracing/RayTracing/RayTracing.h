#pragma once
#pragma once
#ifndef _RAYTRACING
#define _RAYTRACING

#include <vector>
#include <sstream>
#include <string>
#include <math.h>

#include "Image.h"
#include "Object.h"
#include "Point.h"


class RayTracing
{
public:
	
	//construtor prepara renderização
	RayTracing(char* path);

	//renderiza
	void Render(Image& l);


private:
	
	//função lê arquivo rt5 e adiciona informações a RayTracing apontado por RTpointer
	friend int readFile(std::string filename, RayTracing *RTpointer);

	//função busca materiais existentes por nome e o retorna. Se não achar retorna material padrão
	Material* searchMaterial(std::string name);

	//calcula valores do pré-processamento
	int setup();

	//ENTRY VALUES

	char* _path;
	//scene and background proprierties
	Pixel bgColor;
	Pixel bgLightIntensity;
	//TO-DO textura
	std::string bgTexture;

	//camera parametres
	Point eye;
	Point ref;
	Point up;
	float fovy;
	float near;
	float far;
	int w, h;

	//object materials
	std::vector<Material> materials;

	//light parametres
	std::vector<Point> lightsPositions;
	std::vector<Pixel> lightsIntensities;

	//scene objects
	std::vector<Object*> objects;

	//CALCULATED VALUES

	float f;
	//a e b são os intervalos entre pixels na janela de projeção, na direção Xeye e Yeye respectivamente
	float a;
	float b;

	//vetores do sistema de coordenada do olho
	Point Xeye, Yeye, Zeye;

};

#endif