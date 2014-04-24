#ifndef LIGHT_H
#define LIGHT_H

#include "Point.h"
#include "Material.h"


class Light{
  public:
  	Light();
  	Light(const Point &source,const Color &c);
  	~Light();
 
    Point getPosition();
    void setPosition(Point &p);
    void getColor(Color &c);

  private:
  	Point position;
  	Color color;

};

#endif