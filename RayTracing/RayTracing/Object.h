#pragma once
/*
* File:   Object.h
* Author: André Mazal Krauss
* 
* Description: Define a classe material, e a classe abstrata objeto, para serem usadas no algoritmo de ray tracing. Define as sub-classes de object: sphere, plane, box, triangle. 
* 
* Created on Dezember 09, 2016
*/

#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include "Point.h"
#include "Pixel.h"
#include "Ray.h"

class Material
{
public:
	Material(std::string name, Pixel Kd, Pixel Ks, int n_especular, float k, float n_refracao, float o, std::string texture);

	Material(std::string name, Pixel Kd, Pixel Ks, int n_especular);

	Material();
	
	//nome do material
	std::string name;
	
	//coeficiente da reflexão difusa(r,g,b)
	Pixel Kd;
	
	//coeficiente da reflexão especular(r,g,b)
	Pixel Ks;
	
	//N especular((...)^n)
	int n_especular;
	
	//coeficiente de reflexão(espelho)
	float k;
	
	//indice de refração, se opacidade < 1(snell)
	float n_refracao;

	//opacidade (0 = transparente, 1 = opaco)
	float o;

	//textura a ser utilizada (NULL para nao usar)
	std::string texture;
};

//classe abstrata objeto. Não pode ser instanciada. Super-classe dos objetos da cena como esferas, triangulos e planos
class Object
{
public:
	/**
	* Construtor default.
	*/
	Object();

	Material* getMaterial();

	//pure virtual functions a serem implementadas por cada tipo de objeto. 
	//retorna o ponto interceptado e modifica o float t para o valor de t(lançamento: p = o+td)
	virtual float intercept(Ray ray) = 0;

	//retorna a normal no ponto passado. Se o ponto não pertencer ao objeto, retorna 0
	virtual Point normal(Point p) = 0;
	
protected:
	Material *material;
};


class Sphere : public Object
{
public:
	Sphere(Point center, float radius, Material *material);

	float intercept(Ray ray);
	Point normal(Point p);
private:
	Point center;
	float radius;
};

class Box : public Object
{
public:
	Box(Point lowerLeft, Point topRight, Material *material);

	float intercept(Ray ray);
	Point normal(Point p);

private:
	Point lowerLeft;
	Point topRight;
};

class Triangle : public Object
{
public:
	Triangle(Point v1, Point v2, Point v3, Material *material);

	float intercept(Ray ray);
	Point normal(Point p);

private:
	//vertices
	Point v1, v2, v3;
	//a normal do triangulo não depende do intercepto, portanto podemos calculá-la e armazená-la
	Point n;

	//coordenadas de textura dos vértices
	//Point ct1, ct2, ct3;
};

#endif /* OBJECT_H */

