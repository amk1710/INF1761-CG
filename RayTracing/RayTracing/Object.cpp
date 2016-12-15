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

// FIM. Classe abstrata Object

// sub-classe de Object, Sphere
Sphere::Sphere(Point center, float radius, Material *material)
{
	this->center = center;
	this->radius = radius;
	this->material = material;
}

Point Sphere::intercept()
{
	std::cout << "center: " << center.getX() << " ," << center.getY() << " ," << center.getZ() << std::endl;
	std::cout << "radius" << radius << std::endl;

	std::cout << "material:" << material->name << std::endl;

	return Point(0.0, 0.0, 0.0);
}

// FIM. sub-classe de Object, Sphere

// sub-classe de Object, box

Box::Box(Point lowerLeft, Point topRight, Material *material)
{
	this->lowerLeft = lowerLeft;
	this->topRight = topRight;
	this->material = material;
}

Point Box::intercept()
{

	std::cout << "material:" << material->name << std::endl;

	return Point(0.0, 0.0, 0.0);
}

// FIM. sub-classe de Object, Sphere


Triangle::Triangle(Point v1, Point v2, Point v3, Material *material)
{
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->material = material;
}

Point Triangle::intercept()
{
	std::cout << "material:" << material->name << std::endl;

	return Point(0.0, 0.0, 0.0);
}
