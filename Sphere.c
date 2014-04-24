#include "Sphere.h"
#include "Point.h"
// #include "Primitive.h"
// #include "Ray.h"
// #include "Point.h"
// #include "Vector.h"
// #include "Scene.h"
// #include "Material.h"

// class Point;

Sphere::Sphere(){type=2;myId=id;id++;}

Sphere::Sphere(const Point &center,float radius){
  type=2;
  this->radius=radius;
  this->center=center;
  myId=id;id++;
}

Sphere::Sphere(const Point &center,float radius,const Material &m){
  type=2;
  this->radius=radius;
  this->center=center;
  material = m;
  myId=id;id++;
}

Sphere::~Sphere(){}

void Sphere::getNormal(Point &p,Vector &n) const{
  n = Vector(p-center);
}

//Calculates intersection ray-primitive, 
bool Sphere::intersect(Ray& r,Scene& s,Color &col,float &distance){
  Vector O(r.getOrigin().getX()-center.getX(),r.getOrigin().getY()-center.getY(),r.getOrigin().getZ()-center.getZ());
  Vector d(r.getDirection());
  Vector aux = O*2.0;

  double a = Dot(d,d);
  double b = Dot(aux,d);
  double c = Dot(O, O)-radius*radius;
  double disc = b*b-(4*a*c);

  if(disc > 0){
    double sdisc = sqrt(disc);
    double root1 = (-b - sdisc)/(2.0*a);
    double root2 = (-b + sdisc)/(2.0*a);

    root1<root2?distance=root1:distance=root2;
    float t = distance;

    //Calculate intersection point
    Vector inter = Vector(r.getOrigin())+(d*t);
    Point pInter(inter.getX(),inter.getY(),inter.getZ());

    getNormal(pInter,normal);

    //material.getColor(col);
    //---------SHADER----------
    
    Color amb,lightColor;
    s.getAmbient(amb);

    //material.getColor(col);
    Point auxPoint(r.getDirection().getX()*t,r.getDirection().getY()*t,r.getDirection().getZ()*t);
    Point interPoint(r.getOrigin()+auxPoint);

    //Ambient component of color light
    lightColor  = amb*material.getAmbient();

    normal.normalize();

    float costheta = normal.getX()*r.getDirection().getX() + normal.getY()*r.getDirection().getY() + normal.getZ()*r.getDirection().getZ();
    if(costheta > 0)
      normal = -normal;
    

    for(int j=0;j<s.getNumLights();j++){
      Light lightAux = s.getLight(j);
      Vector light_direction(lightAux.getPosition().getX()-interPoint.getX(),
                   lightAux.getPosition().getY()-interPoint.getY(),
                   lightAux.getPosition().getZ()-interPoint.getZ());


      light_direction.normalize();

      float cosphi = light_direction.getX()*normal.getX()+light_direction.getY()*normal.getY()+light_direction.getZ()*normal.getZ();//Dot(normal, light_direction);

      if(true){
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
    
    //Blend material color and light color
    material.getColor(col);
    col = col*lightColor;

    return true;
  }

  return false;

}
