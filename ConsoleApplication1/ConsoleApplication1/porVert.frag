#version 410

//fragment Shader

//implementa��o do modelo de Phong por v�rtices. Andr� Mazal Krauss 2016

out vec4 fragmentColor;

in vec4 fgColor;

void main(void)
{
    //na implementa��o por v�rtices, o fragment shader somente atribuir a cor interpolada dos v�rtices a cor final do fragmento
	fragmentColor =  fgColor;
}
