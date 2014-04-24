#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Vector.h"
#include "Point.h"
#include "Material.h"
#include "Ray.h"
#include "Scene.h"
#include <stdio.h>

class Scene;

class Primitive{
  public:
    Primitive();
    ~Primitive();

    //Normal of the primitive in a point
    Vector getNormal(const Point& p) const;
    Vector& getNormal();
    //Material: color, diffuse component ,specular...
    Material getMaterial() const;
    //Calculates intersection ray-primitive, 
    virtual bool intersect(Ray& r,Scene& s,Color &c,float &distance){};    
    //Specifies color
    void setColor(const Color c);
    //Get id
    int getId();

    virtual Point& getVertex1(){};
    virtual Point& getVertex2(){};
    virtual Point& getVertex3(){};

  static int id;

  protected:
    Material material;
    Vector normal;
    int type;
    int myId;
    

};  







#endif