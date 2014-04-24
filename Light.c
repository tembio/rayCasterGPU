#include "Light.h"
#include <stdio.h>

  	Light::Light(){}

  	Light::Light(const Point &source,const Color &c){
  		color = c;
  		position = source;
  	}

  	Light::~Light(){}
 
    Point Light::getPosition(){
      return position;
    }

    void Light::setPosition(Point &p){
      position = p;
    }

    void Light::getColor(Color &c){
    	c=color;
    }