#include "Object.h"

#include <iostream>

// Classe Material

Material::Material(char * name, Point Kd, Point Ks, int n_especular, double k, double n_refracao, double o, char *texture)
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

Material::Material(char * name, Point Kd, Point Ks, int n_especular)
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
	this->name = "NAN";
	this->Kd = Point(0,0,0);
	this->Ks = Point(0, 0, 0);
	this->n_especular = 0;
	// valores default
	this->k = 0;
	this->n_refracao = 0;
	this->o = 1;
	this->texture = nullptr;
}



// FIM. Classe Material


// Classe abstrata Object
Object::Object()
{
	material = Material("NAN", Point(0, 0, 0), Point(0, 0, 0), 1);
}

// FIM. Classe abstrata Object

// sub-classe de Object, Sphere
Sphere::Sphere(Point center, float radius, Material material)
{
	this->center = center;
	this->radius = radius;
	this->material = material;
}

Point Sphere::intercept()
{
	std::cout << "center: " << center.getX() << " ," << center.getY() << " ," << center.getZ() << std::endl;
	std::cout << "radius" << radius << std::endl;

	std::cout << "material:" << material.name << std::endl;

	return Point(0.0, 0.0, 0.0);
}

// FIM. sub-classe de Object, Sphere