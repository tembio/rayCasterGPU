#ifndef PLANE_H
#define PLANE_H

#include "Primitive.h"
#include "Point.h"
#include "Vector.h"

class Plane : public Primitive{
	public:
		Plane();

		Plane(const Vector &normal,const Point &p);

		Plane(const Vector &normal,const Point &p,const Material &m);

		Plane(const Vector &normal,const Point &p,const Color &m);

		~Plane();

		virtual bool intersect(Ray& ray, Scene& s,Color& col,float &distance);

	private:
		Point point;


} ;

#endif