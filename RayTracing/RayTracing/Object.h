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

	//pure virtual function a ser implementada por cada tipo de objeto
	virtual Point intercept() = 0;
	
protected:
	Material *material;
};


class Sphere : public Object
{
public:
	Sphere(Point center, float radius, Material *material);

	Point intercept();
private:
	Point center;
	float radius;
};

class Box : public Object
{
public:
	Box(Point lowerLeft, Point topRight, Material *material);

	Point intercept();

private:
	Point lowerLeft;
	Point topRight;
};

class Triangle : public Object
{
public:
	Triangle(Point v1, Point v2, Point v3, Material *material);

	Point intercept();

private:
	//vertices
	Point v1, v2, v3;

	//coordenadas de textura dos vértices
	//Point ct1, ct2, ct3;
};

#endif /* OBJECT_H */

