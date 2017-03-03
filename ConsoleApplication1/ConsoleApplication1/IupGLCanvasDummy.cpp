/* 
 * File:   IupGLCanvasDummy.cpp
 * Author: jeferson, André Mazal Krauss(modifier)
 * 
 * Created on August 31, 2014, 9:28 AM
 * Modified by André Mazal Krauss during November 2016
 */

#include "IupGLCanvasDummy.h"

#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iup/iup.h>
#include <iup/iupgl.h>
#include <iosfwd>
#include <string>
#include <fstream>
#include<iostream>




IupGLCanvasDummy::IupGLCanvasDummy( )
{
    //Cria janela e define suas configuracoes.
    createWindow( );
}



void IupGLCanvasDummy::createWindow( )
{
    //Cria botao de sair.
    Ihandle *exitButton = IupButton( "Sair", NULL );

	//Cria botão de mudança de shader
	Ihandle *changeButton = IupButton("Muda", NULL);

    //Cria canvas.
    Ihandle *canvas = IupGLCanvas( NULL );

    //Cria composicao para o botao.
    Ihandle *hboxButton = IupHbox( IupFill(), changeButton, IupFill( ), exitButton, NULL );

    //Cria composicao final.
    Ihandle *vboxFinal = IupVbox( canvas, hboxButton, NULL );

    //Cria dialogo.
    _dialog = IupDialog( vboxFinal );

    //Define os atributos do botao exit
    IupSetAttribute( exitButton, IUP_RASTERSIZE, "80x32" );
    IupSetAttribute( exitButton, IUP_TIP, "Fecha a janela." );

	//define os atributos do botão change
	IupSetAttribute(changeButton, IUP_RASTERSIZE, "80x32");
	IupSetAttribute(exitButton, IUP_TIP, "Muda de shader.");

    //Define os atributos do canvas.
    IupSetAttribute( canvas, IUP_RASTERSIZE, "600x600" );
    IupSetAttribute( canvas, IUP_BUFFER, IUP_DOUBLE );
    IupSetAttribute( canvas, IUP_EXPAND, IUP_YES );

    //Define propriedades do dialogo.
    IupSetAttribute( _dialog, IUP_MARGIN, "10x10" );
    IupSetAttribute( _dialog, IUP_TITLE, "OpenGL + Canvas Dummy" );
    IupSetAttribute( _dialog, "THIS", ( char* ) this );

    //Define callbacks do botao exit e change
    IupSetCallback( exitButton, IUP_ACTION, ( Icallback ) exitButtonCallback );
	IupSetCallback(changeButton, IUP_ACTION, (Icallback)changeButtonCallback);

    //Define as callbacks do canvas.
    IupSetCallback( canvas, IUP_ACTION, ( Icallback ) actionCanvasCallback );
    IupSetCallback( canvas, IUP_RESIZE_CB, ( Icallback ) resizeCanvasCallback );
    IupSetCallback( canvas, IUP_BUTTON_CB, ( Icallback ) buttonCanvasCallback );
    IupSetCallback( canvas, IUP_WHEEL_CB, ( Icallback ) wheelCanvasCallback );
	IupSetCallback(canvas, IUP_MOUSEMOTION_CB, (Icallback)mouseDragCallback);

	

	

    //Mapeia o dialogo.
    IupMap( _dialog );

    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Incialia propriedades dos canvas.
    initializeCanvas( );

	char arq[30];
	do
	{
		printf("Arquivo OFF a ser renderizado\n");
		scanf("%s", arq);

	} while (le_OFF(arq));
}



IupGLCanvasDummy::~IupGLCanvasDummy( )
{
    IupDestroy( _dialog );
}



void IupGLCanvasDummy::show( )
{
    IupShow( _dialog );
}



void IupGLCanvasDummy::hide( )
{
    IupHide( _dialog );
}



std::string IupGLCanvasDummy::readFile( const char* name )
{
    std::ifstream in( name );
    std::string shader;

    if (in.fail( ))
    {
        return "";
    }

    char a;
    while (in.get( a ) && a != EOF)
    {
        shader += a;
    }
    shader += '\0';
    return shader;
}



void IupGLCanvasDummy::initializeCanvas( )
{
    glClearColor( 1.0, 1.0, 1.0, 1.0 );

	scale = 1.0;
	alpha = 1.0;
	beta = 1.0;
	alphaLuz = 1.0;
	betaLuz = 1.0;
	light_distance = 10.0;

    //Aloca shader por vértice
    _shaderpVert = new GraphicsShader( );

    std::string vertexShader = readFile( "porVert.vert" );
	_shaderpVert->setVertexProgram( vertexShader.c_str( ), vertexShader.size( ) );

    std::string fragmentShader = readFile( "porVert.frag" );
	_shaderpVert->setFragmentProgram( fragmentShader.c_str( ), fragmentShader.size( ) );

	//Aloca shader por fragmento
	_shaderpFrag = new GraphicsShader();

	vertexShader = readFile("porFrag.vert");
	_shaderpFrag->setVertexProgram(vertexShader.c_str(), vertexShader.size());

	fragmentShader = readFile("porFrag.frag");
	_shaderpFrag->setFragmentProgram(fragmentShader.c_str(), fragmentShader.size());

	perVertex = true;

	glEnable(GL_DEPTH_TEST);
}

std::vector<float> IupGLCanvasDummy::produto_vetorial(std::vector<float> u, std::vector<float> v)
{
	std::vector<float> vetp;
	if (u.size() != 3 || v.size() != 3)
	{
		return vetp;
	}
	
	vetp.resize(3);
	vetp[0] = u[1] * v[2] - u[2] * v[1];
	vetp[1] = u[2] * v[0] - u[0] * v[2];
	vetp[2] = u[0] * v[1] - u[1] * v[0];

	return vetp;
}



void IupGLCanvasDummy::calcula_normais()
{
	if(coords.empty() || triangleMesh.empty())
	{
		return;
	}
	
	normais.resize(n_vertices);
	//zera normais dos vértices
	for (int i = 0; i < n_vertices; i++)
	{
		normais[i].x = 0;
		normais[i].y = 0;
		normais[i].z = 0;
	}

	//para cada triangulo, obtem o produto vetorial de dois de seus lados e o soma e cada vértice
	vertex u;
	vertex v;
	//normal do triangulo
	vertex n;
	for (int i = 0; i < n_triangulos; i++)
	{
		//u = v2 - v1
		u.x = coords[triangleMesh[i].v2].x - coords[triangleMesh[i].v1].x;
		u.y = coords[triangleMesh[i].v2].y - coords[triangleMesh[i].v1].y;
		u.z = coords[triangleMesh[i].v2].z - coords[triangleMesh[i].v1].z;

		//v = v3 - v2
		v.x = coords[triangleMesh[i].v3].x - coords[triangleMesh[i].v2].x;
		v.y = coords[triangleMesh[i].v3].y - coords[triangleMesh[i].v2].y;
		v.z = coords[triangleMesh[i].v3].z - coords[triangleMesh[i].v2].z;

		//normal do triangulo
		n.x = u.y * v.z - u.z * v.y;
		n.y = u.z * v.x - u.x * v.z;
		n.z = u.x * v.y - u.y * v.x;

		//soma normal do triangulo nos três vértices

		normais[triangleMesh[i].v1].x += n.x;
		normais[triangleMesh[i].v1].y += n.y;
		normais[triangleMesh[i].v1].z += n.z;

		normais[triangleMesh[i].v2].x += n.x;
		normais[triangleMesh[i].v2].y += n.y;
		normais[triangleMesh[i].v2].z += n.z;

		normais[triangleMesh[i].v3].x += n.x;
		normais[triangleMesh[i].v3].y += n.y;
		normais[triangleMesh[i].v3].z += n.z;

	}
	//normaliza para cada vértice: creio que não é necessário. Tentar provar pela inversa transposta da model view.
	float norma;
	for (int i = 0; i < n_vertices; i++)
	{
		norma = sqrt(normais[i].x * normais[i].x + normais[i].y * normais[i].y + normais[i].z*normais[i].z);
		normais[i].x /= norma;
		normais[i].y /= norma;
		normais[i].z /= norma;
	}

}



int IupGLCanvasDummy::le_OFF(char* path)
{
	int i, t;
	std::string ts;
	std::ifstream offFile;
	
	offFile.open(path, std::fstream::in);
	if(!(offFile.is_open()))
	{
		printf("Erro de abertura OFF. Digite o nome de um arquivo existente, incluíndo a extensão .off\n");
		return 1;
	}

	//lê "OFF", numero de vertices, triangulos, e 0
	
	offFile >> ts;
	offFile >> n_vertices;
	offFile >> n_triangulos;
	offFile >> i;

	//armazena vertices no vetor de vertice
	coords.resize(n_vertices);
	for (i = 0; i < n_vertices; i++)
	{
		offFile >> coords[i].x >> coords[i].y >> coords[i].z;
	}

	cores.resize(4 * n_vertices);
	for (i = 0; i < n_vertices; i++)
	{
		//cor roxa		
		/*cores[4 * i] = 0.5;
		cores[4 * i + 1] = 0.1;
		cores[4 * i + 2] = 0.7;
		cores[4 * i + 3] = 1.0;*/

		//srand(time(NULL));

		//vértices de cores distintas. bem colorido
		cores[4 * i] = (static_cast <float> (rand()/2.0) / static_cast <float> (RAND_MAX));
		cores[4 * i + 1] = (static_cast <float> (rand()/2.0) / static_cast <float> (RAND_MAX));
		cores[4 * i + 2] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		cores[4 * i + 3] = 1.0;
	}

	//armazena triangulos no vetor de triangulos
	triangleMesh.resize(n_triangulos);
	for (i = 0; i < n_triangulos; i++)
	{
		offFile >> t >> triangleMesh[i].v1 >> triangleMesh[i].v2 >> triangleMesh[i].v3;
	}

	calcula_normais();

	return 0;

}



void IupGLCanvasDummy::drawScene( )
{
    //Limpa a janela.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum err = glGetError( );

    if (err != GL_NO_ERROR)
    {
        printf( "Error : %s\n", gluErrorString( err ) );
    }

	//shader a ser usado
	GraphicsShader* _shader;
	if (perVertex == true)
	{
		_shader = _shaderpVert;
	}
	else
	{
		_shader = _shaderpFrag;
	}

	//compila o shader se este nao tiver sido compilado ainda
	if (!_shader->isAllocated())
		_shader->compileShader();

	//Carrega o programa na placa.
	_shader->load();

	unsigned int glShader = _shader->getShaderIndex();


    //Aplica uma transformacao de lookat, escala e rotação
    _modelViewMatrix.push( );

	//eye em coordenadas do mundo
	float eyeX = 10.0, eyeY = 2, eyeZ = 0;
	
	//posições das luzes, em coordenadas do olho. Máximo de 16 fontes de luz
	float lights[3*16] = { 10, 0, 0,   0, 10, 0, };

	int nLights = 2;

	//float lightPositionX = 10.0, lightPositionY = 10.0, lightPositionZ = 10.0;

	_modelViewMatrix.lookAt(eyeX, eyeY, eyeZ, 0, 0, 0, 1, 1, 0);

	//passa view pra placa
	int ViewParam = glGetUniformLocation(glShader, "v");
	glUniformMatrix4fv(ViewParam, 1, GL_FALSE, (float*)_modelViewMatrix);



	_modelViewMatrix.scale(scale, scale, scale);

	//rotaciona por alpha em torno do eixo z
	_modelViewMatrix.rotate(alpha, 0, 0, 1);
	//rotaciona por beta em torno do eixo y
	_modelViewMatrix.rotate(beta, 0, 1, 0);


	

    //Transfere os vertices para a placa.
    int vertexParam = glGetAttribLocation( glShader, "vtx" );
    glVertexAttribPointer( vertexParam, 3, GL_FLOAT, GL_FALSE, 0, &coords[0] );
    glEnableVertexAttribArray( vertexParam );

	//transfere a cor dos vértices para a placa
	int colorParam = glGetAttribLocation( glShader, "vtColor");
	glVertexAttribPointer(colorParam, 4, GL_FLOAT, GL_FALSE, 0, &cores[0]);
	glEnableVertexAttribArray(colorParam);

//	transfere as normais para a placa
	int normalParam = glGetAttribLocation(glShader, "vtNormal");
	glVertexAttribPointer(normalParam, 3, GL_FLOAT, GL_FALSE, 0, &normais[0]);
	glEnableVertexAttribArray(normalParam);

	
    //Obtem a modelview projection (mvp)
    {
        _projectionMatrix.push( );
		_modelViewMatrix.push( );


        //Multiplica a modelview pela projection.
        _projectionMatrix.multMatrix( ( float* ) _modelViewMatrix );

        //Transfere a matriz para a placa.
        int matrixParam = glGetUniformLocation( glShader, "mvp" );
        glUniformMatrix4fv( matrixParam, 1, GL_FALSE, ( float* ) _projectionMatrix );

        _projectionMatrix.pop( );
		_modelViewMatrix.pop();

		//passa eye, lightPosition e nLights

		int eyeParam = glGetUniformLocation(glShader, "eye");
		glUniform3f(eyeParam, eyeX, eyeY, eyeZ);

		int nLightsParam = glGetUniformLocation(glShader, "nLights");
		glUniform1i(nLightsParam, nLights);

		int lightParam = glGetUniformLocation(glShader, "lightPosition");
		glUniform3fv(lightParam, 16, lights);

		
		////transfere a view e model-view para a placa
		_modelViewMatrix.push();

		int modelViewParam = glGetUniformLocation(glShader, "mv");
		glUniformMatrix4fv(modelViewParam, 1, GL_FALSE, (float*)_modelViewMatrix);


		//obtem transposta inversa e transfere para a placa
		float *matInvTrans = new float[3 * 3];
		_modelViewMatrix.getMatrixInverseTransposed(matInvTrans);


		int modelViewInvTransParam = glGetUniformLocation(glShader, "normalMv");
		glUniformMatrix3fv(modelViewInvTransParam, 1, GL_FALSE, (float*)matInvTrans);
		delete[] matInvTrans;

		_modelViewMatrix.pop();
        
    }	
    
    //Desempilha a matriz que foi empilhada para fazer a transformacao de escala.
    _modelViewMatrix.pop( );

    //Desenha os elementos.
    glDrawElements( GL_TRIANGLES, 3*n_triangulos, GL_UNSIGNED_INT, &triangleMesh[0] );

    //Descarrega o programa da placa.
    _shader->unload( );
}



void IupGLCanvasDummy::resizeCanvas( int width, int height )
{
    //Define o viewport.
    glViewport( 0, 0, width, height );

    _projectionMatrix.loadIdentity( );
    //_projectionMatrix.ortho( -10, 10, -10, 10, -1, 1 );

	if (height == 0) height = 1;                        // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height; // Compute aspect ratio

	_projectionMatrix.perspective(60, aspect, 1.0, 20.0);
}



int IupGLCanvasDummy::exitButtonCallback( Ihandle* button )
{
	return IUP_CLOSE;
}



int IupGLCanvasDummy::actionCanvasCallback( Ihandle* canvas )
{
    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Obtem ponteiro para o this.
    IupGLCanvasDummy *window = ( IupGLCanvasDummy* ) IupGetAttribute( canvas, "THIS" );

    //Redesenha a janela.
    window->drawScene( );

    //Troca os buffers.
    IupGLSwapBuffers( canvas );

    return IUP_DEFAULT;
}



int IupGLCanvasDummy::resizeCanvasCallback( Ihandle *canvas, int width, int height )
{
    //Torna o canvas como corrente.
    IupGLMakeCurrent( canvas );

    //Obtem ponteiro para o this.
    IupGLCanvasDummy *window = ( IupGLCanvasDummy* ) IupGetAttribute( canvas, "THIS" );

    //Redesenha a janela.
    window->resizeCanvas( width, height );

    //Marca o canvas para ser redesenhado.
    IupUpdate( canvas );

    return IUP_DEFAULT;
}



int IupGLCanvasDummy::buttonCanvasCallback( Ihandle* canvas, int button, int pressed,
                                            int x, int y, char* status )
{
	//Obtem ponteiro para o this.
	IupGLCanvasDummy *window = (IupGLCanvasDummy*)IupGetAttribute(canvas, "THIS");

	//botões do mouse:
	if (iup_isbutton1(status))
	{
		window->alpha += 5.0;
		if (window->alpha > 360.0) window->alpha = 0.0;

	}
	else if (iup_isbutton3(status))
	{
		window->beta += 5.0;
		if (window->beta > 360.0) window->beta = 0.0;
	}

	//Redesenha a janela.
	window->drawScene();

	//Troca os buffers.
	IupGLSwapBuffers(canvas);


	return IUP_DEFAULT;
}



int IupGLCanvasDummy::wheelCanvasCallback( Ihandle* canvas, float delta, int x,
                                           int y, char* status )
{
	//Obtem ponteiro para o this.
	IupGLCanvasDummy *window = (IupGLCanvasDummy*)IupGetAttribute(canvas, "THIS");

	if (delta > 0)
	{
		window->scale *= 1.05;
	}
	else if (delta < 0)
	{
		window->scale *= 0.95;
	}

	//Redesenha a janela.
	window->drawScene();

	//Troca os buffers.
	IupGLSwapBuffers(canvas);

	return IUP_DEFAULT;
}

int IupGLCanvasDummy::mouseDragCallback(Ihandle *canvas)
{
	return IUP_DEFAULT;
}

int IupGLCanvasDummy::changeButtonCallback(Ihandle* canvas)
{
	//Obtem ponteiro para o this.
	IupGLCanvasDummy *window = (IupGLCanvasDummy*)IupGetAttribute(canvas, "THIS");

	if (window->perVertex == true)
	{
		window->perVertex = false;
	}
	else if (window->perVertex != true)
	{
		window->perVertex = true;
	}

	//Redesenha a janela.
	window->drawScene();

	//Troca os buffers.
	IupGLSwapBuffers(canvas);
	return IUP_DEFAULT;
}