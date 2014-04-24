#include "Material.h"
#include <stdio.h>

Material::Material(){}

Material::Material(const Color &c,float specular,float diffuse,float ambient ){
  this->specular=specular;
  this->diffuse=diffuse;
  this->ambient=ambient;

  color = c;
}

float Material::getSpecular()const{
  return specular;
}

float Material::getDiffuse()const{
  return diffuse;
}

float Material::getAmbient()const{
  return ambient;
}

void Material::getColor(Color & c)const{
  c=color;
}

void Material::setColor(int r, int g, int b){
  color.r=r;
  color.g=g;
  color.b=b;
}




