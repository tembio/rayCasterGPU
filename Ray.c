#include "Ray.h"

Ray::Ray(){ }

Ray::Ray(const Point& o, const Vector& d){
	origin = o;
	direction = d;
}
  
Ray::~Ray() {}

Ray::Ray(Ray& copy){
  origin = copy.getOrigin();
  direction = copy.getDirection();  
}

Ray& Ray::operator=(Ray& copy) {
  origin = copy.getOrigin();
  direction = copy.getDirection();  
  return *this;
}

Point& Ray::getOrigin(){
  return origin;
}
Vector& Ray::getDirection(){
  return direction;
}