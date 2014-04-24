#ifndef SCENE_H 
#define SCENE_H

#include "Material.h"
#include "Primitive.h"
#include "Point.h"
#include "Image.h"
#include "Light.h"
#include <vector>

class Primitive;
class Triangle;
class Plane;
  
class Scene{
  public:
  	Scene();
  	~Scene();
  	int getNumPrimitives()const;
  	void getPrimitives(std::vector<Primitive*> &v)const;
    int getNumLights();
    Light& getLight(int i);
  	void setAmbient(const Color &c);
  	void getAmbient(Color& a);
  	void readPrimitives(char* file);
    Point getEyePosition()const;
    void setEyePosition(const Point &p);
    Point getImageCenter()const;
    void setImageCenter(const Point &p);
    void setBackground(const Color & c);
    void rayTracer(Image &i);
    void addLight(Light &l);

    void addPrimitive(Primitive* t);
    //void addPlane(Plane* p);

  private:
  	int numPrimitives;
  	std::vector<Primitive*> primitives;

    int numLights;
    Light* lights; 

  	Color ambient;
    Color background;
    Point eyePosition;
    Point imageCenterPosition;

};

#endif