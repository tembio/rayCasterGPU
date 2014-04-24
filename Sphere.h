 #ifndef SPHERE_H
#define SPHERE_H

#include "Primitive.h"
#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Scene.h"
#include "Material.h"

class Point;

class Sphere : public Primitive{
  public:
    Sphere();
  	Sphere(const Point &center,float radius);
  	Sphere(const Point &center,float radius,const Material &m);
  	~Sphere();

    void getNormal(Point &p,Vector &normal) const;
    //Calculates intersection ray-primitive, 
    bool intersect(Ray& r,Scene& s,Color &c,float &distance); 

  private:
  	Point center;
    float radius;

};


#endif