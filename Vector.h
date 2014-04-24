#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Point;

class Vector{
	public:
	  Vector();

	  Vector(float x, float y, float z);

	  ~Vector();

      float getX();

      float getY();

      float getZ();

      Vector(Point p);

      Vector operator+(const Vector& v) const;

      Vector operator-(const Vector& v) const;

      Vector operator-() const;

      Vector& operator+=(const Vector& v);

      Vector& operator-=(const Vector& v);

      Vector operator*(const Vector& v) const;

      Vector operator*(double s) const;

      Vector& operator*=(const Vector& v);

      Vector& operator*=(double s);

      Vector operator/(double s) const;

      Vector& operator/=(double s);

      Vector operator/(const Vector& v);

      double length() const;

      double length2() const;

      double normalize();

      Vector inverse() const;

      Vector absoluteValue() const;



	private:
	  float x;
	  float y;
	  float z;


};

Vector zero();

Vector one();

inline Vector Cross(Vector& v1,Vector& v2)
{
  return Vector(v1.getY()*v2.getZ() - v1.getZ()*v2.getY(),
                v1.getZ()*v2.getX() - v1.getX()*v2.getZ(),
                v1.getX()*v2.getY() - v1.getY()*v2.getX());
}

inline float Dot(Vector& v1,Vector& v2){
  return v1.getX()*v2.getX() + v1.getY()*v2.getY() + v1.getZ()*v2.getZ();
}   

#endif