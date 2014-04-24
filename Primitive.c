#include "Primitive.h"


Primitive::Primitive(){
	this->normal = zero();
}

Primitive::~Primitive(){}

//Normal of the primitive in a point
Vector Primitive::getNormal(const Point& p) const{
	return normal;
}

Vector& Primitive::getNormal(){
  return this->normal;
}

//Material: color, diffuse component ,specular...
Material Primitive::getMaterial() const{
	return material;
}

void Primitive::setColor(const Color c){
	material.setColor(c.r,c.g,c.b);
}

int Primitive::getId(){return myId;}

int Primitive::id=0;