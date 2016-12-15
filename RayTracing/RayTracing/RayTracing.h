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
	
	//construtor prepara renderiza��o
	RayTracing(char* path);

	//renderiza
	void Render(Image& l);


private:
	
	//fun��o l� arquivo rt5 e adiciona informa��es a RayTracing apontado por RTpointer
	friend int readFile(std::string filename, RayTracing *RTpointer);

	//fun��o busca materiais existentes por nome e o retorna. Se n�o achar retorna material padr�o
	Material* searchMaterial(std::string name);

	//calcula valores do pr�-processamento
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
	//a e b s�o os intervalos entre pixels na janela de proje��o, na dire��o Xeye e Yeye respectivamente
	float a;
	float b;

	//vetores do sistema de coordenada do olho
	Point Xeye, Yeye, Zeye;

};

#endif