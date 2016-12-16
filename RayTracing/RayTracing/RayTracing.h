#pragma once
#pragma once
#ifndef _RAYTRACING
#define _RAYTRACING

//recurs�o m�xima
#define REC_MAX 5

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

	//tra�a o raio e retorna o pixel obtido. rec � o n�vel de recurs�o.
	Pixel trace(Ray ray, int rec);

	//dado o �ndice de um objeto e um ponto de sua superf�cie, calcula a cor daquele ponto NAQUELE OBJET0. cp � o "centro de proje��o"(se n�o for raio refletido, ser� eye, se for, ser� o ponto onde foi refletido)
	Pixel shade(int index, Point p1, int rec, Point cp);

	//dado um ponto em um objeto e um �ndice da luz, retorna true se o ponto est� na sombra da luz
	bool sombra(Point p, int index);

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