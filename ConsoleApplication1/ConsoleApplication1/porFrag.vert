#version 410

//Vertex Shader

//implementa��o do modelo de ilumina��o de Phong por fragmentos. Andr� Mazal Krauss, 2016


in vec4 vtx;
in vec4 vtColor;
in vec3 vtNormal;

out vec3 fgNormal;
out vec4 fgColor;
out vec4 fgx;

//matriz model-view-projection
uniform mat4 mvp;

//matriz model-view
uniform mat4 mv;

uniform mat4 v;

//matriz inversa transposta da model-view. Usada para as normais
uniform mat3 normalMv;

//eye
uniform vec3 eye;

//lightPosition
uniform int nLights;
uniform vec3 lightPosition[16];

//lightPosition
//uniform vec3 lightPosition;



void main(void)
{
    //na implementa��o por fragmentos, o vertexShader somente atribui aos v�rtices algumas informa��es (n�o uniformes) que ser�o interpoladas para cada fragmento
	gl_Position = mvp * vtx;

	fgNormal = vtNormal;
	fgColor = vtColor;
	fgx = vtx;
}
