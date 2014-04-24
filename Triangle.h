#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Primitive.h"
#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Scene.h"
#include "Material.h"

class Point;

class Triangle : public Primitive{
  public:
    Triangle();
  	Triangle(const Point &a,const Point &b,const Point &c);
  	Triangle(const Point &a,const Point &b,const Point &c,const Material &m);
  	~Triangle();

    void getNormal(const Point &p,Vector &normal) const;
    Point& getVertex1();
    Point& getVertex2();
    Point& getVertex3();
    
    Vector& getNormal();
    //Calculates intersection ray-primitive, 
    bool intersect(Ray& r,Scene& s,Color &c,float &distance); 

  private:
  	Point vertices[3];

  	void calculateNormal(Vector &normal);


};


#endif