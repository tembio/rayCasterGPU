#include "Vector.h"
#include <math.h>

#include "Point.h"

Vector::Vector(){}

Vector::Vector(float x, float y, float z){
	  	this->x=x;
	  	this->y=y;
	  	this->z=z;
}

Vector::Vector(Point p){
  this->x=p.getX();
  this->y=p.getY();
  this->z=p.getZ();
}

Vector::~Vector(){}

float Vector::getX(){
  return x;
}

float Vector::getY(){
	return y;
}

float Vector::getZ(){
  return z;
}

Vector Vector::operator+(const Vector& v) const {
  return Vector(x+v.x,y+v.y,z+v.z);
}

Vector Vector::operator-(const Vector& v) const {
  return Vector(x-v.x,y-v.y,z-v.z);
}

Vector Vector::operator-() const {
  return Vector(-x,-y,-z);
}

Vector& Vector::operator+=(const Vector& v) {
  x+=v.x; y+=v.y; z+=v.z;
  return *this;
}

Vector& Vector::operator-=(const Vector& v) {
  x-=v.x; y-=v.y; z-=v.z;
  return *this;
}

Vector Vector::operator*(const Vector& v) const {
  return Vector(x*v.x, y*v.y, z*v.z);
}

Vector Vector::operator*(double s) const {
  return Vector(x*s, y*s, z*s);
}

Vector& Vector::operator*=(const Vector& v) {
  x*=v.x; y*=v.y; z*=v.z;
  return *this;
}

Vector& Vector::operator*=(double s) {
  x*=s; y*=s; z*=s;
  return *this;
}

Vector Vector::operator/(double s) const {
  double inv_s = 1./s;
  return Vector(x*inv_s, y*inv_s, z*inv_s);
}

Vector& Vector::operator/=(double s) {
  double inv_s = 1./s;
  x*=inv_s; y*=inv_s; z*=inv_s;
  return *this;
}

Vector Vector::operator/(const Vector& v){
  return Vector(x/v.x, y/v.y, z/v.z);
}

double Vector::length() const {
  return sqrt(x*x+y*y+z*z);
}

double Vector::length2() const {
  return x*x+y*y+z*z;
}

double Vector::normalize() {
  double l = length();
  double scale = 1./l;
  *this *= scale;
  return l;
}

Vector Vector::inverse() const {
  return Vector(1./x, 1./y, 1./z);
}

Vector Vector::absoluteValue() const {
  return Vector(fabs(x),fabs(y), fabs(z));
}

Vector zero() {
  return Vector(0,0,0);
}

Vector one() {
  return Vector(1,1,1);
}

