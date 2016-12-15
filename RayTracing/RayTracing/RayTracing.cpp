#include "RayTracing.h"

#include <iostream>
#include <fstream>

RayTracing::RayTracing(char * path)
{
	_path = path;
	readFile(path, this);
	this->setup();
}

std::string readLine(std::ifstream& stream)
{
	std::string line;
	char a;

	while (stream.get(a) && a != EOF && a != '\n')
	{
		line += a;
	}

	return line;
}

int readFile(std::string path, RayTracing *RTpointer)
{
	std::ifstream rt5(path);
	std::string line;
	std::string temp;
	int lineNumber = 0;

	
	if (rt5.fail())
	{
		std::cout << "Erro ao abrir arquivo " << path << ". Arquivo existe?" << std::endl;
		return -1;
	}

	std::cout << "Lendo arquivo: " << path << std::endl;

	line = readLine(rt5); //linha RT5
	if (!(line == "RT 5"))
	{
		std::cout << "Arquivo n�o � um arquivo RT5" << std::endl;
		return -2;
	}
	lineNumber++;

	

	while (rt5) {
		rt5 >> temp;

			//Ajeita casos em que \n � o �ltimo caractere
			if (rt5.peek() == -1) break;
		

		if (temp[0] == '!')
		{
			//Linha com coment�rio, pula para a pr�xima
			readLine(rt5);
			lineNumber++;
			continue;
		}

		if (temp == "SCENE")
		{
			//trata cena

			float r_fundo, g_fundo, b_fundo;
			float r_luz, g_luz, b_luz;
			std::string temp2;
			rt5 >> r_fundo;
			rt5 >> g_fundo;
			rt5 >> b_fundo;
			rt5 >> r_luz;
			rt5 >> g_luz;
			rt5 >> b_luz;
			rt5 >> temp2;

			RTpointer->bgColor = Pixel(r_fundo, g_fundo, b_fundo);
			RTpointer->bgLightIntensity = Pixel(r_luz, g_luz, b_luz);
			RTpointer->bgTexture = temp2;


		}

		else if (temp == "CAMERA")
		{
			//trata c�mera

			float eyeIn[3], refIn[3], upIn[3], a, n, f, w, h;
			rt5 >> eyeIn[0];
			rt5 >> eyeIn[1];
			rt5 >> eyeIn[2];
			rt5 >> refIn[0];
			rt5 >> refIn[1];
			rt5 >> refIn[2];
			rt5 >> upIn[0];
			rt5 >> upIn[1];
			rt5 >> upIn[2];
			rt5 >> a;
			rt5 >> n;
			rt5 >> f;
			rt5 >> w;
			rt5 >> h;

			RTpointer->eye = Point(eyeIn[0], eyeIn[1], eyeIn[2]);
			RTpointer->ref = Point(refIn[0], refIn[1], refIn[2]);
			RTpointer->up = Point(upIn[0], upIn[1], upIn[2]);
			RTpointer->fovy = a;
			RTpointer->near = n;
			RTpointer->far = f;
			RTpointer->w = w;
			RTpointer->h = h;
		}

		else if (temp == "MATERIAL")
		{
			//trata material
			std::string nome, textura;
			int n;
			float kd[3], ks[3], k, ref, o;
			rt5 >> nome;
			if (isdigit(nome[0])) kd[0] = std::stof(nome);
			else rt5 >> kd[0];
			rt5 >> kd[1];
			rt5 >> kd[2];
			rt5 >> ks[0];
			rt5 >> ks[1];
			rt5 >> ks[2];
			rt5 >> n;
			rt5 >> k;
			rt5 >> ref;
			rt5 >> o;
			rt5 >> textura;

			Pixel Kd = Pixel(kd[0], kd[1], kd[2]);
			Pixel Ks = Pixel(ks[0], ks[1], ks[2]);

			//???
			//Material m = Material(nome.c_str(), Kd, Ks, n, k, ref, o, textura.c_str());
			Material m = Material();
			m.k = k;
			m.Kd = Kd;
			m.Ks = Ks;
			m.name = nome;
			m.n_especular = n;
			m.n_refracao = ref;
			m.o = o;
			m.texture = textura;

			RTpointer->materials.push_back(m);
		}

		else if (temp == "LIGHT")
		{
			//trata luz
			float posicao[3], intensidade[3];
			rt5 >> posicao[0];
			rt5 >> posicao[1];
			rt5 >> posicao[2];
			rt5 >> intensidade[0];
			rt5 >> intensidade[1];
			rt5 >> intensidade[2];

			Point point = Point(posicao[0], posicao[1], posicao[2]);
			Pixel pixel = Pixel(intensidade[0], intensidade[1], intensidade[2]);
			RTpointer->lightsPositions.push_back(point);
			RTpointer->lightsIntensities.push_back(pixel);

		}

		else if (temp == "SPHERE")
		{
			//trata esfera
			Point center;
			std::string materialName;
			float r, x, y, z;
			rt5 >> materialName;
			rt5 >> r;
			rt5 >> x;
			rt5 >> y;
			rt5 >> z;

			center = Point(x, y, z);
			Sphere *sphere = new Sphere(center, r, RTpointer->searchMaterial(materialName));
			RTpointer->objects.push_back(sphere);
		}

		else if (temp == "BOX")
		{
			//trata caixa
			std::string material;
			Point upperRight, lowerLeft;
			float pts[2][3];

			rt5 >> material;
			rt5 >> pts[0][0];
			rt5 >> pts[0][1];
			rt5 >> pts[0][2];
			rt5 >> pts[1][0];
			rt5 >> pts[1][1];
			rt5 >> pts[1][2];

			lowerLeft = Point(pts[0][0], pts[0][1], pts[0][2]);
			upperRight = Point(pts[1][0], pts[1][1], pts[1][2]);

			Box *box = new Box(lowerLeft, upperRight, RTpointer->searchMaterial(material));
			RTpointer->objects.push_back(box);

		}

		else if (temp == "TRIANGLE")
		{
			//trata tri�ngulo
			std::string material;
			Point a, b, c;
			float vertex[3][3], texture[3][2];

			rt5 >> material;
			rt5 >> vertex[0][0];
			rt5 >> vertex[0][1];
			rt5 >> vertex[0][2];
			rt5 >> vertex[1][0];
			rt5 >> vertex[1][1];
			rt5 >> vertex[1][2];
			rt5 >> vertex[2][0];
			rt5 >> vertex[2][1];
			rt5 >> vertex[2][2];
			rt5 >> texture[0][0];
			rt5 >> texture[0][1];
			rt5 >> texture[1][0];
			rt5 >> texture[1][1];
			rt5 >> texture[2][0];
			rt5 >> texture[2][1];

			a = Point(vertex[0][0], vertex[0][1], vertex[0][2]);
			b = Point(vertex[1][0], vertex[1][1], vertex[1][2]);
			c = Point(vertex[2][0], vertex[2][1], vertex[2][2]);

			//TODO: e o material?
			Material *mat = RTpointer->searchMaterial(material);
			Triangle *triangle = new Triangle(a, b, c, mat);
			
			RTpointer->objects.push_back(triangle);

		}

		else
		{
			std::cout << "Erro ao ler arquivo: " << temp << " inesperado no inicio da linha " << lineNumber << std::endl;
			return -1;
		}

		if (rt5.peek() == '\n') lineNumber++;
	}

	rt5.close();
	return 0;
}

void RayTracing::Render(Image & l)
{
	l = Image(w, h);
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			Pixel p;
			p[0] = 0.7;
			p[1] = 0.3;
			p[2] = 0.9;
			l.setPixel(i, j, p);
		}
	}

	return;
}

Material* RayTracing::searchMaterial(std::string name)
{
	for (int i = 0; i < materials.size(); i++)
	{
		if (materials[i].name == name)
		{
			return &materials[i];
		}
	}
	return new Material();
}

int RayTracing::setup()
{
	f = near;
	a = 2 * f*tan(fovy / 2.0);
	b = ((float)w / (float)h)*a;

	//calculo do sistemas de coordenadas
	//Zeye
	Zeye = eye - ref;
	Zeye.normalize();
	//Xeye
	Xeye = up.produto_vetorial(Zeye);
	Xeye.normalize();
	//Yeye
	Yeye = Zeye.produto_vetorial(Xeye);
	Yeye.normalize();

	return 0;
	
}
