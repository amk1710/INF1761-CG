/* 
 * File:   Pixel.cpp
 * Author: jcoelho
 * 
 * Created on September 11, 2016, 6:03 PM
 */
#include <cstdio>
#include "Pixel.h"



Pixel::Pixel( )
{
    _val[0] = _val[1] = _val[2] = 0.0;
}

Pixel::Pixel(float r, float g, float b)
{
	_val[0] = r;
	_val[1] = g;
	_val[2] = b;
}



float& Pixel::operator[]( unsigned int i )
{
    if (i < 3)
        return _val[i];
    else
        printf( "Erro: posicao de pixel nao existe\n" );

    return _val[0];
}



float Pixel::operator[]( unsigned int i ) const
{
    if (i < 3)
        return _val[i];
    else
        printf( "Erro: posicao de pixel nao existe\n" );

    return _val[0];
}



Pixel Pixel::operator*( const Pixel& p ) const
{
    Pixel product = Pixel();
	for (int i = 0; i < 3; i++)
	{
		product[i] = this->_val[i] * p._val[i];
	}
    return product;
}

Pixel Pixel::operator*(const float& k) const
{
	float r = _val[0] * k;
	float g = _val[1] * k;
	float b = _val[2] * k;
	return Pixel(r, g, b);
}



Pixel Pixel::operator-( const Pixel& p ) const
{
    Pixel result;
    for (int i = 0; i < 3; i++)
        result[i] = _val[i] - p[i];
    return result;
}

//AMK
Pixel Pixel::operator+( const Pixel& p ) const
{
    Pixel result;
    for (int i = 0; i < 3; i++)
        result[i] = _val[i] + p[i];
    return result;
}
