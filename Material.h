#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>

class Color{
  public:
  	float r,g,b;
    Color(){}
    Color(const Color &c){this->r=c.r;this->g=c.g;this->b=c.b;}
    Color(float r, float g, float b){this->r=r;this->g=g;this->b=b;}
    Color& operator+(Color &c){r+=c.r;g+=c.g;b+=c.b;return *this;}
    Color& operator*(float f){r = r*f; g = g*f;b = b*f; return *this;}
    Color& operator*(Color &c){r = r*c.r; g = g*c.g;b = b*c.b; return *this;}
    Color& operator=(const Color &c){this->r=c.r;this->g=c.g;this->b=c.b;return *this;}
};

class Material{
  public:
  	Material();
  	Material(const Color &c,float specular,float diffuse,float ambient);
  	float getSpecular()const;
  	float getDiffuse()const;
    float getAmbient()const;
  	void getColor(Color &c)const;
    void setColor(int r, int g, int b);


  protected:
  	Color color;
  	float diffuse;
  	float specular;
    float ambient;

};


#endif