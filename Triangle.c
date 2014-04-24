#include "Triangle.h"
#include <stdio.h>

Triangle::Triangle(){type=1;myId=id;id++;}

Triangle::Triangle(const Point &a,const Point &b,const Point &c){
  type=1;
  vertices[0]=a;
  vertices[1]=b;
  vertices[2]=c;
  calculateNormal(normal);
  myId=id;id++;
}

Triangle::Triangle(const Point &a,const Point &b,const Point &c,const Material &m){
  type=1;
  vertices[0]=a;
  vertices[1]=b;
  vertices[2]=c;
  material = m;
  calculateNormal(normal);
  myId=id;id++;
}

Triangle::~Triangle(){

}

void Triangle::getNormal(const Point &p,Vector &normal) const{
  normal = this-> normal;
}

Point& Triangle::getVertex1(){return vertices[0];}
Point& Triangle::getVertex2(){return vertices[1];}
Point& Triangle::getVertex3(){return vertices[2];}

Vector& Triangle::getNormal(){
  return this-> normal;
}

bool Triangle::intersect(Ray& ray, Scene& s,Color& col,float &distance){
  double a,b,c,d,e,f,g,h,i,j,k,l,M,beta,gamma,t;

  //V1 of triangle
  a = vertices[0].getX() - vertices[1].getX();
  b = vertices[0].getY() - vertices[1].getY();
  c = vertices[0].getZ() - vertices[1].getZ();
  //V2 of triangle
  d = vertices[0].getX() - vertices[2].getX();
  e = vertices[0].getY() - vertices[2].getY();
  f = vertices[0].getZ() - vertices[2].getZ();
  
  g = ray.getDirection().getX();
  h = ray.getDirection().getY();
  i = ray.getDirection().getZ();

  j = vertices[0].getX() - ray.getOrigin().getX();
  k = vertices[0].getY() - ray.getOrigin().getY();
  l = vertices[0].getZ() - ray.getOrigin().getZ();

  //Using Kramer to solve system to find barycentric coordinates
 
  M = a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g);   
  
  beta = (j*(e*i-h*f) + k*(g*f-d*i) + l*(d*h-e*g))/M;

  gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;

  t = (f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/M;
  t= -t;



  if(t<0)
    return false;

   //test if hot
  if(beta+gamma <=1 && 0<=beta && 0<=gamma){
    distance=t;

    //Calculate intersection point
    Vector inter = Vector(ray.getOrigin())+(ray.getDirection()*t);
    Point interPoint(inter.getX(),inter.getY(),inter.getZ());

    //Ambient component of color light
    Color amb,lightColor;
    s.getAmbient(amb);
    lightColor  = amb*material.getAmbient();

    normal.normalize();

    float costheta = Dot(normal,ray.getDirection());
    if(costheta > 0)
      normal = -normal;
    

    for(int j=0;j<s.getNumLights();j++){
      Light lightAux = s.getLight(j);
      Vector light_direction(lightAux.getPosition()-interPoint);

      light_direction.normalize();

      float cosphi = Dot(light_direction,normal);

      if(cosphi > 0){
          // Cast shadow rays...
          //Ray shadowray(hitpos, light_direction);
          //world->intersect(shadowhit, context, shadowray);
          //if(!shadowhit.getPrimitive())
            // No shadows...
            Color aux;
            lightAux.getColor(aux);  
            lightColor = lightColor + aux*(cosphi*material.getDiffuse());
      }

    }

    // Blend material color and lighting    
    material.getColor(col);
    col = col*lightColor;

    return true;
  }

  return false;
}


void Triangle::calculateNormal(Vector &normal){
  double a,b,c,d,e,f;
  //V1 of triangle
  a = vertices[0].getX() - vertices[1].getX();
  b = vertices[0].getY() - vertices[1].getY();
  c = vertices[0].getZ() - vertices[1].getZ();
  Vector v1(a,b,c);
  //V2 of triangle
  d = vertices[0].getX() - vertices[2].getX();
  e = vertices[0].getY() - vertices[2].getY();
  f = vertices[0].getZ() - vertices[2].getZ();
  Vector v2(d,e,f);

  normal = Cross(v2,v1);
}
