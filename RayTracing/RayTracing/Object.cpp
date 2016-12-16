#include "Object.h"

#include <iostream>

// Classe Material

Material::Material(std::string name, Pixel Kd, Pixel Ks, int n_especular, float k, float n_refracao, float o, std::string texture)
{
	this->name = name;
	this->Kd = Kd;
	this->Ks = Ks;
	this->n_especular = n_especular;
	this->k = k;
	this->n_refracao = n_refracao;
	this->o = o;
	this->texture = texture;
}

Material::Material(std::string name, Pixel Kd, Pixel Ks, int n_especular)
{
	this->name = name;
	this->Kd = Kd;
	this->Ks = Ks;
	this->n_especular = n_especular;
	// valores default
	this->k = 0;
	this->n_refracao = 0;
	this->o = 1;
	this->texture = nullptr;

}

Material::Material()
{
	// valores default
	this->name = "NAN";
	this->Kd = Pixel(0,0,0);
	this->Ks = Pixel(0, 0, 0);
	this->n_especular = 0;	
	this->k = 0;
	this->n_refracao = 0;
	this->o = 1;
	this->texture = "null";
}



// FIM. Classe Material


// Classe abstrata Object
Object::Object()
{
	material = new Material();
}

Material* Object::getMaterial()
{
	return material;
}

// FIM. Classe abstrata Object

// sub-classe de Object, Sphere
Sphere::Sphere(Point center, float radius, Material *material)
{
	this->center = center;
	this->radius = radius;
	this->material = material;
}

float Sphere::intercept(Ray ray)
{
	/*std::cout << "center: " << center.getX() << " ," << center.getY() << " ," << center.getZ() << std::endl;
	std::cout << "radius" << radius << std::endl;

	std::cout << "material:" << material->name << std::endl;*/

	/*float Sphere::intercept(float *o, float *d)
{
	float t1, t2, delta, a, b, c;
	float temp[3];

	//calcula os coeficientes da equação de 2° grau
	a = scalarProduct(d, d);
	temp[0] = o[0] - center.getX();
	temp[1] = o[1] - center.getY();
	temp[2] = o[2] - center.getZ();
	b = 2 * scalarProduct(d, temp);
	c = scalarProduct(temp, temp) - (radius*radius);

	//calcula o delta
	delta = (b*b) - (4*a*c);

	//checa se t1 e t2 são reais
	if (delta < 0) 
		return -1;
	
	//calcula t1 e t2
	t1 = (-b + sqrt(delta)) / (2.0 * a);
	t2 = (-b - sqrt(delta)) / (2.0 * a);

	//retorna o menor valor
	return t1 < t2 ? t1 : t2;
} 
	*/ 

	float t0, t1, delta, a, b, c;
	//monta equação de segundo grau a*ti^2 + b*ti + c
	
	a = ray.d * ray.d;

	Point OminusC = Point();
	OminusC = (ray.o - center);
	
	b = 2 * (ray.d * OminusC);
	c = (OminusC * OminusC) - (radius * radius);

	//resolve a equação de segundo grau
	delta = (b*b) - (4 * a*c);
	if (delta < 0)
	//raio não intercepta esfera
	{
		return 0;
	}
	//calcula t1 e t2
	t0 = (-b + sqrt(delta)) / (2.0 * a);
	t1 = (-b - sqrt(delta)) / (2.0 * a);

	return t0 < t1 ? t0 : t1;
}

Point Sphere::normal(Point p)
{
	return Point();
}

// FIM. sub-classe de Object, Sphere

// sub-classe de Object, box

Box::Box(Point lowerLeft, Point topRight, Material *material)
{
	this->lowerLeft = lowerLeft;
	this->topRight = topRight;
	this->material = material;
}

float Box::intercept(Ray ray)
{

//	std::cout << "material:" << material->name << std::endl;
	//não sei se o input sempre estará na ordem menor para maior... ver tratar isso depois
	float t;
	if (ray.d.getX() > 0)
	//raio pode entrar pela face x = xmin
	{
		t = (lowerLeft.getX() - ray.o.getX()) / ray.d.getX();
		float y, z;
		y = ray.o.getY() + ray.d.getY() * t;
		z = ray.o.getZ() + ray.d.getZ() * t;
		if (y >= lowerLeft.getY() && y <= topRight.getY() && z >= lowerLeft.getZ() && z <= topRight.getZ())
		{
			return t;
		}
	}
	else if (ray.d.getX() < 0)
	//raio pode entrar pela face x = xmax
	{
		t = (topRight.getX() - ray.o.getX()) / ray.d.getX();
		float y, z;
		y = ray.o.getY() + ray.d.getY() * t;
		z = ray.o.getZ() + ray.d.getZ() * t;
		if (y >= lowerLeft.getY() && y <= topRight.getY() && z >= lowerLeft.getZ() && z <= topRight.getZ())
		{
			return t;
		}
	}
	
	//não entra pelas faces X, testa para Y e Z

	if (ray.d.getY() > 0)
	//raio pode entrar pela face y = ymin
	{
		t = (lowerLeft.getY() - ray.o.getY()) / ray.d.getY();
		float x, z;
		x = ray.o.getX() + ray.d.getX() * t;
		z = ray.o.getZ() + ray.d.getZ() * t;
		if (x >= lowerLeft.getX() && x <= topRight.getX() && z >= lowerLeft.getZ() && z <= topRight.getZ())
		{
			return t;
		}
	}
	else if (ray.d.getY() < 0)
	//raio pode entrar pela face y = ymax
	{
		t = (topRight.getY() - ray.o.getY()) / ray.d.getY();
		float x, z;
		x = ray.o.getX() + ray.d.getX() * t;
		z = ray.o.getZ() + ray.d.getZ() * t;
		if (x >= lowerLeft.getX() && x <= topRight.getX() && z >= lowerLeft.getZ() && z <= topRight.getZ())
		{
			return t;
		}
	}
	
	if (ray.d.getZ() > 0)
	//raio pode entrar pela face z = zmin
	{
		t = (lowerLeft.getZ() - ray.o.getZ()) / ray.d.getZ();
		float x, y;
		x = ray.o.getX() + ray.d.getX() * t;
		y = ray.o.getY() + ray.d.getY() * t;
		if (x >= lowerLeft.getX() && x <= topRight.getX() && y >= lowerLeft.getY() && y <= topRight.getY())
		{
			return t;
		}
	}
	else if (ray.d.getZ() < 0)
		//raio pode entrar pela face z = zmax
	{
		t = (topRight.getZ() - ray.o.getZ()) / ray.d.getZ();
		float x, y;
		x = ray.o.getX() + ray.d.getX() * t;
		y = ray.o.getY() + ray.d.getY() * t;
		if (x >= lowerLeft.getX() && x <= topRight.getX() && y >= lowerLeft.getY() && y <= topRight.getY())
		{
			return t;
		}
	}

	//raio não entra por nenhuma face. Não há interceptação
	return 0;
}

Point Box::normal(Point p)
{
	return Point();
}

// FIM. sub-classe de Object, Sphere


Triangle::Triangle(Point v1, Point v2, Point v3, Material *material)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->material = material;
}

float Triangle::intercept(Ray ray)
{
//	std::cout << "material:" << material->name << std::endl;
	
	//normal
	Point n = Point();
	n = (v2 - v1).produto_vetorial((v3 - v2));
	n.normalize();

	float den = ray.d * n;
	if (den == 0)
	//raio é pararelo ao plano do triangulo. Não há interceptação
	{
		return 0;
	}

	float ti = ((v1 - ray.o) * n) / den;

	//ponto em que o raio intercepta o plano do triangulo
	Point pi = Point();
	pi = ray.o + (ray.d * ti);

	//teste por coordenadas baricêntricas
	float A1 = n*((v3 - v2).produto_vetorial(pi - v2)) / 2.0;
	float A2 = n*((v1 - v3).produto_vetorial(pi - v3)) / 2.0;
	float A3 = n*((v2 - v1).produto_vetorial(pi - v1)) / 2.0;
	float Ar = A1 + A2 + A3;

	float L1 = A1 / Ar;
	float L2 = A2 / Ar;
	float L3 = A3 / Ar;

	if ((L1 >= 0 && L2 >= 0 && L3 >= 0) && (L1 <= 1.0 && L2 <= 1.0 && L3 <= 1.0))
	//se todas as coordenadas baricêntricas são maiores ou iguais a 0 e menores ou iguais a 1, o ponto pi está dentro do triangulo
	{
		return ti;
	}

	return 0;
}

Point Triangle::normal(Point p)
{
	return Point();
}
