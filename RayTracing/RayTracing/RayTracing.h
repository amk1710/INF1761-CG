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

	//obtem o raio do ponto (i,j)
	Ray getRay(int i, int j);

	//traça o raio e retorna o pixel obtido
	Pixel trace(Ray ray);

	//dado o índice de um objeto e um ponto de sua superfície, calcula a cor daquele ponto NAQUELE OBJET0.
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
	//ângulo de abertura EM GRAUS
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
	//a e b são os intervalos entre pixels na janela de projeção, na direção Xeye e Yeye respectivamente
	float a;
	float b;

	//vetores do sistema de coordenada do olho
	Point Xeye, Yeye, Zeye;


};

#endif