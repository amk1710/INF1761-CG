#pragma once
#ifndef _POINT
#define _POINT

#include <math.h>

class Point 
{
public:
	float getX();
	float getY();
	float getZ();

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);

	//realiza o produto vetorial entre o vetor p1 e o vetor parametro p2
	Point produto_vetorial(Point p2);

	//normaliza o ponto
	void normalize();

	Point(float x, float y, float z);

	Point(float x, float y);

	Point();

	/**
	* Realiza o produto escalar entre dois pontos.
	* @param p - ponto a ser multiplicado pelo ponto corrente.
	* @return - resulatado do produto escalar.
	*/
	float operator*(const Point& p) const;

	/**
	* Realiza o produto de um ponto por um escalar.
	* @param t - escalar multiplicativo.
	* @return - novo ponto multiplicado.
	*/

	Point operator*(const float t) const;

	/**
	* Sobrecarga do operador -.
	* @param p - ponto a ser subtraido.
	* @return - resultado da operacao.
	*/
	Point operator-(const Point& p) const;

	/** AMK
	* Sobrecarga do operador +.
	* @param p - ponto a ser somado.
	* @return - resultado da operacao.
	*/
	Point operator+(const Point& p) const;

private:
	float _x;
	float _y;
	float _z;
};

#endif