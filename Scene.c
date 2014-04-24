#include "Scene.h"
#include <stdlib.h> 
#include <stdio.h>
#include "Plane.h"
#include "cudaCode.h"



Scene::Scene(){
  numPrimitives=0;
  numLights=0;
  
  lights = new Light[5];
  ambient = Color(0,0,0);
}

Scene::~Scene(){
  //free(lights);
}

int Scene::getNumPrimitives()const{
  return numPrimitives;
}

int Scene::getNumLights(){ return numLights;}

Light& Scene::getLight(int i){
  return lights[i];
}

void Scene::getPrimitives(std::vector<Primitive*> &v)const{
  v = primitives;
}

void Scene::setAmbient(const Color &c){
  ambient.r=c.r;
  ambient.g=c.g;
  ambient.b=c.b;
}

void Scene::getAmbient(Color& a){

  a = ambient;
}

Point Scene::getEyePosition()const{
  return eyePosition;
}

void Scene::setEyePosition(const Point &p){
  eyePosition = p;
}

Point Scene::getImageCenter()const{
  return imageCenterPosition;
}

void Scene::setImageCenter(const Point &p){
  imageCenterPosition = p;
}

void Scene::setBackground(const Color & c){
  background = c;
}



void Scene::addPrimitive(Primitive *t){
  primitives.push_back(t);
  numPrimitives++;
}

void Scene::rayTracer(Image &im){
  Point bottomLeftCorner(-im.getXresolution()/2,-im.getYresolution()/2,0);
  Color c(0,0,0),finalColor;
  bool intersected,bgColor;


	for(int i=0;i<im.getXresolution();i++)
		for(int j=0;j<im.getYresolution();j++){
      Point aux = bottomLeftCorner+Point((float)i,(float)j,0)-eyePosition;
      Vector dir(aux.getX(),aux.getY(),aux.getZ());
     
      Ray ray(eyePosition,dir);

      float distance,minDistance=10000;
      bool firstPrimitive=true;
      bgColor=true;

      ray.getDirection().normalize();

      for(int p=0;p<numPrimitives;p++){
        //Bigger t == closer

         intersected = primitives[p]->intersect(ray,*this,c,distance);

         if(intersected && (firstPrimitive||distance<=minDistance)){ 
          minDistance=distance;
          finalColor=c;
          bgColor=false;
          firstPrimitive=false;
        }

      }

			if(bgColor)
			 im.set(i,j,background);
      else
       im.set(i,j,finalColor);

		}

}

void Scene::addLight(Light &l){
  lights[numLights] = l;
  numLights++;
}

void Scene::readPrimitives(char* file){}


