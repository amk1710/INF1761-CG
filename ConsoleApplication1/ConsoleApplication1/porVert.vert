#version 410

//vertex Shader

//implementação do modelo de iluminação de Phong por vértices. André Mazal Krauss 2016


in vec4 vtx;
in vec4 vtColor;
in vec3 vtNormal;

out vec4 fgColor;

//matriz model-view-projection
uniform mat4 mvp;

//matriz model-view
uniform mat4 mv;

//matriz de view
uniform mat4 v;

//matriz inversa transposta da model-view. Usada para as normais
uniform mat3 normalMv;

//eye
uniform vec3 eye;

//vetor de lightPosition, em coordenadas da camera e a quantidade de luzes
uniform int nLights;
uniform vec3 lightPosition[16];

struct lightSource
{
  vec4 position;
  vec4 diffuse;
  vec4 specular;
};

lightSource light0[16];

vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);

struct material
{
  vec4 diffuse;
  vec4 specular;
};

material mymaterial = material(
  vec4(vtColor),
  vec4(1.0, 1.0, 1.0, 1.0)
);

void main(void)
{
    gl_Position = mvp * vtx;
		

	//vec4 usados para multiplicar pelas matrizes 4x4. depois, seu w será usado para dividir as demais componentes as serem usadas nos vec3
	vec4 lpos4;
	vec4 vtpos4;
	vec3 normal;
	vec3 lpos;
	vec3 vtpos;
	vec3 L;
	vec3 ambientColor = vec3(0.0, 0.0, 0.0);
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	int i;
	float dotp;
	vec4 eye4;
	vec3 eyeU;

	//intensidade da iluminação de cada fonte
	float intensity = 1.0;

	//calcula transformações sobre o eye. Efetivamente iguala o eyeU a vec(0,0,0). 
	//eye4 = vec4(eye, 1);
	//eye4 = v * eye4;
	//eyeU = vec3(eye4 / eye4[3]);
	eyeU = vec3(0,0,0);

	//zera a cor do vertice
	fgColor = vec4(0.0, 0.0, 0.0, 1.0);

	//para cada luz, soma a componente especular e difusa
	for(i = 0; i < nLights; i++)
	{
		 light0[i] = lightSource(
			vec4(lightPosition[i], 1.0),
			vec4(intensity,  intensity,  intensity, 1.0),
			vec4(intensity,  intensity,  intensity, 1.0)
		);
	

		//transforma o que for necessário:
		lpos4 = v * light0[i].position;
		lpos4 = lpos4 / lpos4[3];
		vtpos4 = mv * vtx;
		vtpos4 = vtpos4 / vtpos4[3];


		//normais, posições de luz, posições do vertice e vetor L usados na fórmula
		normal = normalize(normalMv * vtNormal);
		lpos = vec3(lpos4);
		vtpos = vec3(vtpos4);
		//	L = vetor unitário apontando do vértice para a fonte de luz
		L = normalize(lpos - vtpos);	
	
		dotp = dot(L, normal);
		if(dotp > 0.0)
		//se o cosseno entre o vertice e a luz for maior que 0, ou seja, se o a Luz ilumina esse vertice:
		{
			//cálculos do modelo de Phong
			//cor difusa
			diffuseColor += vec3(mymaterial.diffuse * dotp * light0[i].diffuse);

			//cor especular
			vec3 R = normalize(reflect(L, normal));
			vec3 V = normalize(eyeU - vtpos);

			specularColor += vec3(mymaterial.specular * pow(dot(R, V), 128) * light0[i].specular);
	
		}
	}

	//calcula cor ambiente
	ambientColor = vec3(scene_ambient) * vec3(mymaterial.diffuse);
	
	//soma tres componentes de cor na cor final do vertice
	fgColor = vec4(diffuseColor + specularColor + ambientColor, 1.0);

}
