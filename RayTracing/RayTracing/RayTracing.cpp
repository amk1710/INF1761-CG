#include "RayTracing.h"

#include <iostream>
#include <fstream>

//classe RayTracing
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
		std::cout << "Arquivo não é um arquivo RT5" << std::endl;
		return -2;
	}
	lineNumber++;

	

	while (rt5) {
		rt5 >> temp;

			//Ajeita casos em que \n é o último caractere
			if (rt5.peek() == -1) break;
		

		if (temp[0] == '!')
		{
			//Linha com comentário, pula para a próxima
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
			//trata câmera

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
			//se o material não tiver nome, kd[0] é o primeiro argumento lido
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
			Light l = Light(point, pixel);
			RTpointer->lights.push_back(l);
			
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
			//trata triângulo
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
			Ray ray = getRay(i, j);
			Pixel p = trace(ray);
			l.setPixel(i, j, p);
			float print = (i*h + j) / (w*h);
			std::cout << '\r'
				<< std::setw(2) << std::setfill('0') << i << ','
				<< std::setw(2) << j << ':'
				<< std::setw(2) << print << std::flush;
		}
	}

	return;
}

Material* RayTracing::searchMaterial(std::string name)
{
	//é número?
	if (isdigit(name[0]))
	{
		return &materials[name[0] - '0'];
	}
	
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
	float radians = fovy * (3.14159265 / 180.0);
	a = 2.0 * f * tan(radians / 2.0);
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
	//Yeye.normalize();

	return 0;
	
}

Ray RayTracing::getRay(int i, int j)
{
	/*
	for (int k = 0; k < 3; k++) d[k] = -near*ze[k] + altura*(j/h - 0.5)*ye[k] + base*(i/w - 0.5)*xe[k];
	tempnorm = norm(d);
	for (int k = 0; k < 3; k++) d[k] /= tempnorm
	*/
	
	Point d = Point();
	d = Zeye * -f;
	d = d + (Yeye * (a*(((float)j / (float)h) - 0.5 )));
	d = d + (Xeye * (b*(((float)i / (float)w) - 0.5 )));

	//normaliza?
	d.normalize();

	return Ray(eye, d);
}

Pixel RayTracing::trace(Ray ray)
{
	float t = FLT_MAX, temp;
	int index = -1;
	for (int i = 0; i < objects.size(); i++)
	{
		temp = objects[i]->intercept(ray);
		if (temp != 0 && temp < t)
		//interceptação mais próxima que anterior encontrado
		{
			t = temp;
			index = i;
		}
	}
	if (t > 0 && t != FLT_MAX && index != -1)
	{
		//ponto de interceptação
		Point p1 = ray.o + (ray.d * t);		
		
		return shade(index, p1);

	}
	else
	{
		return bgColor;
	}
	
}

Pixel RayTracing::shade(int index, Point p1)
{
	//MODELO DE PHONG
	//componente de luz ambiente
	Pixel ambient = bgLightIntensity * objects[index]->getMaterial()->Kd;
	//componente de luz difusa
	Pixel diffuse = Pixel();
	//componente de luz especular
	Pixel specular = Pixel();
	
	//normal no ponto
	Point normal = objects[index]->normal(p1);
	//vetor v aponta do interceptante para o centro de projeção
	Point v = (eye - p1);
	v.normalize();
	//Podemos simplificar o algoritmo refletindo o vetor v em relação à normal n uma única vez e, para cada fonte luminosa, fazer o produto interno entre esse vetor e o vetor L correspondente
	Point Rr = (normal * 2 *(v * normal)) - v;
	for (int i = 0; i < lights.size(); i++)
	{

		//L é o vetor unitário que aponta do intercepto para a fonte de luz em questão
		Point L = (lights[i].position - p1);
		L.normalize();
		//cosseno entre a normal e L = entre . Calculado com o produto escalar
		float cos = Rr * L;
		if (cos > 0)
		//se o cosseno for menos que 0, a luz não ilumina o objeto porque é obstruída por ele próprio(a luz está "do outro lado" do objeto)
		{
			diffuse = diffuse + ((objects[index]->getMaterial()->Kd * lights[i].intensity) * cos);
			specular = specular + (objects[index]->getMaterial()->Ks * lights[i].intensity * powf((Rr * L), objects[index]->getMaterial()->n_especular));
		}
		
	}

	return diffuse + ambient + specular;
}
