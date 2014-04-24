#ifndef RAY_H
#define RAY_H

#include "Point.h"
#include "Vector.h"

class Ray{

  public:
  	Ray();
  	Ray(const Point& origin, const Vector& direction);
  	~Ray();
  	Ray(Ray& copy);

    Ray& operator=(Ray& r);
    Point& getOrigin();
    Vector& getDirection();

  private:
     Point origin;
     Vector direction;

};


#endif
