/* 
 * File:   main.cpp
 * Author: jcoelho
 *
 * Created on September 11, 2016, 2:18 PM
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include "Image.h"
#include "Cluster.h"
#include <math.h>
#include <float.h>

using namespace std;

//erro considerado satisfatório, usado para testar fim de algoritmo
#define ERR 0.001
//numero máximo de iterações
#define NIT 10



/**
 * FEITO: converte uma imagem rgb em uma imagem Lab.
 * @param rgb - imagem rgb de entrada.
 * @return - imagem em Lab.
 */

Image convertImageFromRGB2Lab( const Image& rgb )
{
	int i, j;
	Image lab = Image(rgb.getW(), rgb.getH());
	for(i = 0; i < rgb.getW(); i++)
	{
		for(j = 0; j < rgb.getH(); j++)
		{
			//converte de RGB para XYZ, lendo em rgb e gravando em xyz			
			lab.setPixel(i, j, rgb.rgbToXYZ(rgb.getPixel(i,j)));
			//converte de xyz para Lab
			lab.setPixel(i, j, lab.XYZToLab(lab.getPixel(i, j)));
		}
	}
	return lab;
}



/**
 * FEITO: converte uma imagem Lab em uma imagem em rgb.
 * @param Lab - imagem Lab de entrada.
 * @return - imagem em RGB.
 */

Image convertImageFromLAB2RGB( const Image& Lab )
{
	int i, j;
	Image rgb = Image(Lab.getW(), Lab.getH());
	for(i = 0; i < Lab.getW(); i++)
	{
		for(j = 0; j < Lab.getH(); j++)
		{
			//converte de LAB para XYZ, lendo de Lab e gravando em rgb
			rgb.setPixel(i,j,Lab.LabToXYZ(Lab.getPixel(i,j)));
			//converte de xyz para rgb
			rgb.setPixel(i,j,rgb.XYZTorgb(rgb.getPixel(i,j)));
		}
	}
	return rgb;
}


/**
 *  Calcula pixel com menor gradiente na vizinhança 3x3.
 * @param Lab - imagem em Lab.
 * @param x& - referencia para x central. Supõe que x não está na borda
 * @param y& - referencia para y central. supoe que y não está na borda
 */

void vizinhanca(Image Lab, int& x, int& y)
{
	int tx;
	int ty;
	float mingrad = FLT_MAX;
	float tgrad;
	for(int i = x - 1; i <= x + 1; i++)
	{
		for(int j  = y-1; j <= y+1; j++)
		//para cada pixel na vizinhanca 3x3
		{
			// tem uma parte dessa conta que teoricamente é /2, mas na pratica não faz diferença.
			//estou usando somente a luminância.
			tgrad = sqrt(pow((Lab.getPixel(i+1,j)[0] - Lab.getPixel(i-1,j)[0]),2) + pow((Lab.getPixel(i,j+1)[0] - Lab.getPixel(i,j-1)[0]),2));
			if(tgrad < mingrad)
			//se o valor calculado é menor do o que se tinha anteriormente, troca
			{
				mingrad = tgrad;
				tx = i;
				ty = j;
			}

		}
	}
	x = tx;
	y = ty;
}






/**
 * FEITO: inicializa os clusters.
 * @param clusters - clusters que DEVEM ser alocados e inicializados.
 * @param Lab - imagem em Lab.
 * @param k - numero desejado de superpixels.
 * @param s - tamanho em pixels da janela. AMK
 * @return - numero de superpixels.
 */
int initializeClusters( Cluster*& clusters, Image& Lab, int k, int s )
{
	//floor(raiz de k)
	int raizKX = sqrt((double) k);
	int raizKY = ceil((double)sqrt((double) k)); 
	//distancia entre os centros dos sp's para x e y
	int incX = Lab.getW() / raizKX;
	int incY = Lab.getH() / raizKY;
	int tx, ty;
	//array de cluster com numero real de sp's
	clusters = new Cluster[raizKX*raizKY];
	for(int i = 0, y = incY/2; i < raizKY; i++, y += incY)
	{
		for(int j = 0, x = incX/2; j < raizKX; j++, x += incX)
		{
			//sp em (raizK* i + j) é tratado
			tx = x;
			ty = y;
			vizinhanca(Lab, tx, ty);
			//calcula média do entorno daquele pixel
			Pixel media = Pixel();
			for(int t = tx - s/2; t >  tx + s/2; t++)
			{
				for(int u = ty - s/2; t > ty + s/2; u++)
				{
					media + Lab.getPixel(t,u);
				}
			}
			media[0] = media[0] / (s*s);
			media[1] = media[1] / (s*s);
			media[2] = media[2] / (s*s);
			clusters[raizKX*i + j] = Cluster(media, tx, ty);
		}
	}
	return raizKX*raizKY;
}

/**
 * calcula a distancia entre dois pixels
 * @param pixel1 - pixel 1.
 * @param x1 - posição x do pixel 1.
 * @param y1 - posição y do pixel 1
 * @param pixel2 - pixel 2.
 * @param x2 - posição x do pixel 2.
 * @param y2 - posição y do pixel 2
 * @param M2 - compacidade AO QUADRADO
 * @param s - tamanho da janela(distância máxima esperada)
 * @param ret - distancia entre os dois pixel
 */

float dist2Pixel(Pixel pixel1, int x1, int y1, Pixel pixel2, int x2, int y2, double M2, int s)
{
	Pixel sub = Pixel();
	sub = pixel1 - pixel2;
	
	//distância de cor
	float dc = sqrt((sub[0]*sub[0] + sub[1]*sub[1] + sub[2]*sub[2]));
	//distância espacial
	float ds = sqrt((pow((double)(x1 - x2), 2) + pow((double)(y1 - y2), 2)));
	return sqrt(((dc)*(dc) + ((ds/(s*s))*(ds/(s*s)))*M2) );
}


/**
 * FEITO: realiza o algoritmo de superpixels.
 * @param Lab - Imagem em lab.
 * @param clusters - clusters inicializados.
 * @param labels - labels inicializadas.
 * @param k - numero de superpixels.
 * @param M - compacidade.
	@param s - lado da janela. AMK
 */
void performSuperPixelsAlgorithm( Image& Lab, Cluster* clusters, int *labels, int k, double M, int s )
{
	int height  = Lab.getH();
	int width = Lab.getW();
	float* distances = new float[height * width];
	for(int i = 0; i < height * width; i++) distances[i] = FLT_MAX;
	//numero da iteracao
	int iteracao = 0;
	// mudança em distância de pixels do antigo centro para o novo, acumulada para todos sp's 
	int Error;

	double M2 = M*M;

	//medias para cada superPixel
	int* avgX = new int[k];
	int* avgY = new int[k];
	Pixel* avgPixel = new Pixel[k];
	//num de pixel por superPixel
	int* qtdPixel = new int[k];

	for(int i = 0; i < k; i++)
	//para cada superPixel
	{
		avgPixel[i] = Pixel();
	}

	for(int i = 0; i < width*height; i++)
	//para cada pixel,
	{
		distances[i] = FLT_MAX;
	}

	//for(int c = 0; c < k; c++)
	////para cada cluster,
	//{
	//	for(int i = max(0 , (int const) clusters[c].getX() - 2*s); i < min(width, (int const) clusters[c].getX() + 2*s); i++)
	//	{
	//		for(int j = max(0, (int const) clusters[c].getY() - 2*s); j < min(height, (int const) clusters[c].getY() + 2*s); j++)
	//		//para cada pixel na janela 2s por 2s,
	//		{
	//			avgPixel[c] = avgPixel[c] + Lab.getPixel(i,j);
	//		}
	//	}
	//	avgPixel[c][0] = avgPixel[c][0] / (s*s);
	//	avgPixel[c][1] = avgPixel[c][1] / (s*s);
	//	avgPixel[c][2] = avgPixel[c][2] / (s*s);
	//	clusters[c].setPixel(avgPixel[c]);
	//}


	
	do
	//faça para até NIT > iteracao ou Error < ERR
	{
		//assigment step
		for(int c = 0; c < k; c++)
		//para cada cluster,
		{
			for(int i = max(0 , (int const) clusters[c].getX() - 2*s); i < min(width, (int const) clusters[c].getX() + 2*s); i++)
			{
				for(int j = max(0, (int const) clusters[c].getY() - 2*s); j < min(height, (int const) clusters[c].getY() + 2*s); j++)
				//para cada pixel na janela 2s por 2s,
				{
			
					float D = dist2Pixel(clusters[c].getPixel(), clusters[c].getX(), clusters[c].getY(), Lab.getPixel(i,j), i, j, M2, s);
					if(D < distances[j*width + i])
					//se distância é menor, troca.
					{
						labels[j*width + i] = c;
						distances[j*width + i] = D;
					}
				}
			}
		}

		//update step
		
		

		for(int i = 0; i < k; i++)
		//para cada superPixel
		{
			avgX[i] = 0;
			avgY[i] = 0;
			qtdPixel[i] = 0;
			avgPixel[i][0] = 0;
			avgPixel[i][1] = 0;
			avgPixel[i][2] = 0;
		}
	
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			//para cada pixel
			{
				int id = labels[y*width + x];
				avgX[id] += x;
				avgY[id] += y;
				avgPixel[id] = avgPixel[id] + Lab.getPixel(x,y);
				qtdPixel[id] += 1;
			}
		}
		// = 1 -> parada por erro desabilitada
		Error = 0;
		for(int i = 0; i < k; i++)
		//para cada superPixel
		{
			if(qtdPixel[i] != 0)
			{
				avgX[i] = avgX[i] / qtdPixel[i];
				avgY[i] = avgY[i] / qtdPixel[i];
				avgPixel[i][0] = avgPixel[i][0] / qtdPixel[i];
				avgPixel[i][1] = avgPixel[i][1] / qtdPixel[i];
				avgPixel[i][2] = avgPixel[i][2] / qtdPixel[i];
				Error += dist2Pixel(clusters[i].getPixel(), clusters[i].getX(), clusters[i].getY(), avgPixel[i], avgX[i], avgY[i], M2, s);
				clusters[i].set(avgPixel[i], avgX[i], avgY[i]);
			}
		}
		iteracao++;
	}while(Error > ERR && iteracao < NIT);
	//end do


}



void drawContoursAroundSegments( Image& rgb, int* labels, Pixel borderColor = Pixel( ) )
{
    int w = rgb.getW( );
    int h = rgb.getH( );

    const int dx8[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

    int sz = w * h;
    vector<bool> istaken( sz, false );
    vector<int> contourx( sz );
    vector<int> contoury( sz );
    int mainindex( 0 );
    int cind( 0 );
    for (int j = 0; j < h; j++)
    {
        for (int k = 0; k < w; k++)
        {
            int np( 0 );
            for (int i = 0; i < 8; i++)
            {
                int x = k + dx8[i];
                int y = j + dy8[i];

                if (( x >= 0 && x < w ) && ( y >= 0 && y < h ))
                {
                    int index = y * w + x;

                    if (false == istaken[index])//comment this to obtain internal contours
                    {
                        if (labels[mainindex] != labels[index]) np++;
                    }
                }
            }
            if (np > 1)
            {
                contourx[cind] = k;
                contoury[cind] = j;
                istaken[mainindex] = true;
                //img[mainindex] = color;
                cind++;
            }
            mainindex++;
        }
    }

    int numboundpix = cind; //int(contourx.size());
    for (int j = 0; j < numboundpix; j++)
    {
        for (int n = 0; n < 8; n++)
        {
            int x = contourx[j] + dx8[n];
            int y = contoury[j] + dy8[n];
            if (( x >= 0 && x < w ) && ( y >= 0 && y < h ))
            {
                int ind = rgb.computePosition( x, y );
                if (!istaken[ind])
                {
                    rgb.setPixel( ind, borderColor );
                }
            }
        }
    }
}



void enforceLabelConnectivity( const int* labels, //input labels that need to be corrected to remove stray labels
                               const int width,
                               const int height,
                               int*& nlabels, //new labels
                               int& numlabels, //the number of labels changes in the end if segments are removed
                               const int& K ) //the number of superpixels desired by the user
{
    const int dx4[4] = { -1, 0, 1, 0 };
    const int dy4[4] = { 0, -1, 0, 1 };

    const int sz = width * height;
    const int SUPSZ = sz / K;

    for (int i = 0; i < sz; i++) nlabels[i] = -1;
    int label( 0 );
    int* xvec = new int[sz];
    int* yvec = new int[sz];
    int oindex( 0 );
    int adjlabel( 0 ); //adjacent label
    for (int j = 0; j < height; j++)
    {
        for (int k = 0; k < width; k++)
        {
            if (0 > nlabels[oindex])
            {
                nlabels[oindex] = label;
                //--------------------
                // Start a new segment
                //--------------------
                xvec[0] = k;
                yvec[0] = j;
                //-------------------------------------------------------
                // Quickly find an adjacent label for use later if needed
                //-------------------------------------------------------
                {
                    for (int n = 0; n < 4; n++)
                    {
                        int x = xvec[0] + dx4[n];
                        int y = yvec[0] + dy4[n];
                        if (( x >= 0 && x < width ) && ( y >= 0 && y < height ))
                        {
                            int nindex = y * width + x;
                            if (nlabels[nindex] >= 0) adjlabel = nlabels[nindex];
                        }
                    }
                }

                int count( 1 );
                for (int c = 0; c < count; c++)
                {
                    for (int n = 0; n < 4; n++)
                    {
                        int x = xvec[c] + dx4[n];
                        int y = yvec[c] + dy4[n];

                        if (( x >= 0 && x < width ) && ( y >= 0 && y < height ))
                        {
                            int nindex = y * width + x;

                            if (0 > nlabels[nindex] && labels[oindex] == labels[nindex])
                            {
                                xvec[count] = x;
                                yvec[count] = y;
                                nlabels[nindex] = label;
                                count++;
                            }
                        }

                    }
                }
                //-------------------------------------------------------
                // If segment size is less then a limit, assign an
                // adjacent label found before, and decrement label count.
                //-------------------------------------------------------
                if (count <= SUPSZ >> 2)
                {
                    for (int c = 0; c < count; c++)
                    {
                        int ind = yvec[c] * width + xvec[c];
                        nlabels[ind] = adjlabel;
                    }
                    label--;
                }
                label++;
            }
            oindex++;
        }
    }
    numlabels = label;

    if (xvec) delete [] xvec;
    if (yvec) delete [] yvec;
}



void SuperPixels( Image& rgb, int k, double M )
{
    //FEITO: Converte a imagem para LAb.

	Image pixie = Image();
	pixie = convertImageFromRGB2Lab(rgb);

	int width = pixie.getW();
	int height = pixie.getH();
    //FEITO: Calcula o numero de pixels cada superpixel.
	//pixels por superpixel, ao início. DÚVIDA inútil?
	int S = width * height / k;
	//tamanho em pixels do lado da janela.
	int s = sqrt((double)S);

    //FEITO: Inicializa os clusters.
    Cluster* clusters;
	
    
    k = initializeClusters( clusters, pixie, k, s);


    //FEITO: aloca e inicializa labels.
	int* labels = new int[width*height];
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			labels[width*i + j] = -1;
		}
	}


    //FEITO: Executa o algoritmo.
	performSuperPixelsAlgorithm(pixie, clusters, labels, k, M, s);

    
        int* nlabels = new int[width*height];
        enforceLabelConnectivity( labels, width, height, nlabels, k, double(width*height) / double( s * s ) );
        for (int i = 0; i < width*height; i++)
            labels[i] = nlabels[i];

		if (nlabels)
        delete [] nlabels;

    

    //FEITO: define as novas cores dos pixels.

	/*for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			pixie.setPixel(x, y, clusters[labels[y*width + x]].getPixel());
		}
	}*/

	//FEITO: Converte a imagem de volta.rgb = convertImageFromLAB2RGB(pixie);

	if(clusters)
	{
		delete[] clusters;
	}
	


    //Desenha os contornos. Deve passar a imagem em rgb e o vetor de labels.
    drawContoursAroundSegments( rgb, labels );


	if(labels)
	{
		delete[] labels;
	}

}



/*
 * 
 */
int main( int argc, char** argv )
{

    Image l;
    if (l.readBMP( "mapa-mundi-ibge.bmp" ))
    {
        printf( "Leitura executada com sucesso\n" );
    }
    
    SuperPixels( l, 256, 10 );
    
    if (l.writeBMP( "mapa-mundi3.bmp" ))
    {
        printf( "Escrita executada com sucesso\n" );
    }

    return 0;
}

