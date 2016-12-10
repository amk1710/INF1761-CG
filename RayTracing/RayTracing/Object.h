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

#include "Point.h"

class Material
{
public:
	Material(char *name, Point Kd, Point Ks, int n_especular, double k, double n_refracao, double o, char *texture);

	Material(char *name, Point Kd, Point Ks, int n_especular);

	Material();
	
	//nome do material
	char *name;
	
	//coeficiente da reflexão difusa(r,g,b)
	Point Kd;
	
	//coeficiente da reflexão especular(r,g,b)
	Point Ks;
	
	//N especular((...)^n)
	int n_especular;
	
	//coeficiente de reflexão(espelho)
	double k;
	
	//indice de refração, se opacidade < 1(snell)
	double n_refracao;

	//opacidade (0 = transparente, 1 = opaco)
	double o;

	//textura a ser utilizada (NULL para nao usar)
	char *texture;
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
	Material material;
};


class Sphere : public Object
{
public:
	Sphere(Point center, float radius, Material material);

	Point intercept();
private:
	Point center;
	float radius;
};

#endif /* OBJECT_H */

