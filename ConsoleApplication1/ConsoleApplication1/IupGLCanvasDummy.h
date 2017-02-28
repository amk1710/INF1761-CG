/* 
 * File:   IupGLCanvasDummy.h
 * Author: jeferson, André Mazal Krauss(modifier)
 *
 * Created on August 31, 2014, 9:28 AM
 * Modified by André Mazal Krauss during November 2016
 */

#ifndef IUPGLCANVASDUMMY_H
#define IUPGLCANVASDUMMY_H

#include "OpenGLMatrixManager.h"
#include "GraphicsShader.h"
#include <string>
#include <vector>
#include <iup/iup.h>

struct vertex
{
	//decidir essa bagaça. Implementar adaptações. Verificar nessa ordem: calculo da normal, obtenção da inversa transposta e Phong. 
	float x, y, z;
};

struct offTriangle
{
	unsigned int v1, v2, v3;
};

class IupGLCanvasDummy
{
public:
    /**
     * Construtor default da classe.
     */
    IupGLCanvasDummy( );

    /**
     * Destrutor da classe.
     */
    virtual ~IupGLCanvasDummy( );

    /**
     * Exibe a janela.
     */
    void show( );

    /**
     * Oculta a janela.
     */
    void hide( );

private:

    /**
     * Ponteiro para o dialogo.
     */
    Ihandle *_dialog;

    /**
     * Matriz de projecao OpenGL
     */
    OpenGLMatrixManager _projectionMatrix;

    /**
     * Matriz de MODELVIEW OpenGL
     */
    OpenGLMatrixManager _modelViewMatrix;

	/**
	* Matriz de lookat OpenGL
	*/
	OpenGLMatrixManager _lookAtMatrix;


    /**
     * Shader pointer do shader por vértice.
     */
    GraphicsShader* _shaderpVert;

	/**
	* Shader pointer do shader por fragmento.
	*/
	GraphicsShader* _shaderpFrag;


private:

    /**
     * Cria janela da IUP e define suas configuracoes e callbacks.
     */
    void createWindow( );

    /**
     * Incializa algumas propriedades do canvas OpenGL.
     */
    void initializeCanvas( );

	///*******************
	//* Variáveis que armazenam as informações lidas de um arquivo off.
	//*/

	//numero de vértices e de triângulos
	int n_vertices, n_triangulos;
	
	//vetor de coordenadas dos vértices
	std::vector<vertex> coords;

	//vetor de cor dos vértices
	std::vector<float> cores;

	//vetor das normais dos vértices
	std::vector<vertex> normais;
	
	// vetor da definição dos triangulos
	std::vector<offTriangle> triangleMesh;

	//scale alterável por callbacks de botões
	float scale;

	//angulos de rotação alteráveis por callbacks de botões
	float alpha, beta, alphaLuz, betaLuz;

	//distancia da luz até o centro (0,0,0), que é o ponto de observação
	float light_distance;

	//*******************

	/**
	* retorna o produto vetorial de dois vetores 3x3 passados
	*/

	std::vector<float> produto_vetorial(std::vector<float> u, std::vector<float> v);


	/**
	* usa as informações dos triangulos para calcular as normais de cada vértice
	*/

	void calcula_normais();

	/**
	* lê um arquivo off e armazena suas informações nas variáveis acima. Retorna 0 se lido com sucesso, 1 caso contrário. 
	*/

	int le_OFF( char* path);


    /**
     * Realiza o redesenho da cena OpenGL.
     */
    void drawScene( );

    /**
     * Trata evento de redimensionar o canvas OpenGL.
     * @param width - nova larguda do canvas.
     * @param height - nova altura do canvas.
     */
    void resizeCanvas( int width, int height );

    /**
     * Le arquivo de shader.
     * @param name - path do arquivo.
     * @return - string com o arquivo.
     */
    std::string readFile( const char* name );

private:
    /**
     * Callback do botao de fechar a janela.
     * @param button - ihandle do botao de sair.
     * @return - retorna IUP_CLOSE para que a janela seja fechada.
     */
    static int exitButtonCallback( Ihandle *button );

    /**
     * Callback responsavel por receber evento de redesenho do canvas.
     * @param canvas - ponteiro para o canvas.
     * @return  - IUP_DEFAULT.
     */
    static int actionCanvasCallback( Ihandle *canvas );

    /**
     * Callback responsavel por receber eventos do whell do mouse no canvas para
     * realizar a operacao de Zoom.
     * @param canvas - ponteiro para o canvas.
     * @param delta - vale -1 ou 1 e indica a direcao da rotacao do botao whell.
     * @param x - posicao x do mouse na tela.
     * @param y - posicao y do mouse na tela.
     * @param status - status dos botoes do mouse e certas teclas do teclado no 
     * momento que o evento foi gerado.
     * @return - IUP_DEFAULT.
     */
    static int wheelCanvasCallback( Ihandle *canvas, float delta, int x,
                                    int y, char *status );

    /**
     * Callback responsavel por receber eventos de resize do canvas.
     * @param canvas - ponteiro para o canvas.
     * @param width - nova largura, em pixeis, da janela.
     * @param heigth - nova altura, em pixeis, da janela.
     * @return - IUP_DEFAULT.
     */
    static int resizeCanvasCallback( Ihandle *canvas, int width, int height );

    /**
     * Callback responsavel por receber eventos de teclado do canvas.
     * @param canvas - ponteiro para o canvas.
     * @param button - identificador do botao, podem ser BUTTON_1, BUTTON_2, ...
     * @param pressed - 1 para o caso do botao esta sendo pressionado e 0 caso
     * contrario.
     * @param x - posicao x do mouse na tela.
     * @param y - posiao y do mouse na tela.
     * @param status - status dos botoes do mouse e certas teclas do teclado no 
     * momento que o evento foi gerado.
     * @return - IUP_DEFAULT. 
     */
    static int buttonCanvasCallback( Ihandle* canvas, int button, int pressed,
                                     int x, int y, char* status );

	/**
	* Callback responsavel por receber eventos de drag do mouse
	* @param canvas - ponteiro para o canvas.
	* 
	* @return - IUP_DEFAULT.
	*/
	static int mouseDragCallback(Ihandle* canvas);

	
	//true se o shader atual é por vértices. falso caso contrário
	bool perVertex;
	//indica se o shader deverá ser mudado no próximo redraw
	bool change;
	/**
	* Callback responsavel por alterar o shader sendo usado
	* @param canvas - ponteiro para o canvas.
	*
	* @return - IUP_DEFAULT.
	*/
	static int changeButtonCallback(Ihandle* canvas);

};

#endif /* IUPGLCANVASDUMMY_H */

