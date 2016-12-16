#pragma once
#pragma once
#ifndef _RAYTRACING
#define _RAYTRACING

#include <vector>
#include <sstream>
#include <string>
#include <math.h>
#include <iomanip>

#include "Image.h"
#include "Object.h"
#include "Point.h"
#include "Ray.h"
#include "Light.h"


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

	//obtem o raio do ponto (i,j)
	Ray getRay(int i, int j);

	//tra�a o raio e retorna o pixel obtido
	Pixel trace(Ray ray);

	//dado o �ndice de um objeto e um ponto de sua superf�cie, calcula a cor daquele ponto NAQUELE OBJET0.
	Pixel shade(int index, Point p1);

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
	//�ngulo de abertura EM GRAUS
	float fovy;
	float near;
	float far;
	int w, h;

	//object materials
	std::vector<Material> materials;

	//lights
	std::vector<Light> lights;

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