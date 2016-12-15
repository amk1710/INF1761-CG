/* 
 * File:   main.cpp
 * Author: André Mazal Krauss
 *
 * Created on Dezember 09, 2016, 18:58 PM
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <math.h>
#include <float.h>

#include "Image.h"
#include "RayTracing.h"



/*
 * 
 */
int main( int argc, char** argv )
{

	char* name = "person.rt5";
	RayTracing rt = RayTracing(name);
	Image l;

	rt.Render(l);
        
    if (l.writeBMP( "out.bmp" ))
    {
        printf( "Escrita executada com sucesso\n" );
    }

    return 0;
}

