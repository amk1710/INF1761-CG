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
#include <ctime>

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

	clock_t begin = clock();

	rt.Render(l);

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
      
    if (l.writeBMP( "personOut.bmp" ))
    {
        printf( "Escrita executada com sucesso\n" );
    }

	printf("tempo decorrido: %f\n", elapsed_secs);

    return 0;
}

