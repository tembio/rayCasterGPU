#include "Point.h"

Point::Point(){}

Point::Point(float x, float y, float z){
  this->x=x;
  this->y=y;
  this->z=z;
}

Point::Point(const Point& p){
  x = p.x;
  y = p.y;
  z = p.z;
}

Point::~Point(){}

Point & Point::operator=(const Point& p){
  x = p.x;
  y = p.y;
  z = p.z; 

  return *this;
}

Point Point::operator-(const Point& p){
  return Point(x-p.x,y-p.y,z-p.z);
}

Point Point::operator+(const Point& p){
  return Point(x+p.x,y+p.y,z+p.z);
}

bool Point::operator==(const Point& p){
  if(x==p.x && y==p.y && z==p.z)
    return true;
  else
    return false;
}


float Point::getX(){
  return x;
}

float Point::getY(){
  return y;
}

float Point::getZ(){
   return z;
}

void Point::setX(float x){
  this->x=x;
}

void Point::setY(float y){
  this->y=y;
}

void Point::setZ(float z){
   this->z=z;
}