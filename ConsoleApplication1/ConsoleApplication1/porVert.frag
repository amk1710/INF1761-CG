#version 410

//fragment Shader

//implementação do modelo de Phong por vértices. André Mazal Krauss 2016

out vec4 fragmentColor;

in vec4 fgColor;

void main(void)
{
    //na implementação por vértices, o fragment shader somente atribuir a cor interpolada dos vértices a cor final do fragmento
	fragmentColor =  fgColor;
}
