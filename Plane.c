#include "Plane.h"
#include "Primitive.h"
#include "Point.h"
#include "Vector.h"

Plane::Plane(){type=0;myId=id;id++;}

Plane::Plane(const Vector &normal,const Point &p){
  type=0;
	this->normal = normal;
	point = p;
  myId=id;id++;
}

Plane::Plane(const Vector &normal,const Point &p,const Material &m){
  type=0;
	this->normal = normal;
	point = p;
	material = m;
  myId=id;id++;
}

Plane::Plane(const Vector &normal,const Point &p,const Color &m){
  type=0;
	this->normal = normal;
	point = p;
	material.setColor(m.r,m.g,m.b);
  myId=id;id++;
}

Plane::~Plane(){}

bool Plane::intersect(Ray& ray,Scene& s,Color& col,float &distance){

		double denom = Dot(normal, ray.getDirection());
		Vector aux(point.getX(),point.getY(),point.getZ());
		Vector aux2(ray.getOrigin().getX(),ray.getOrigin().getY(),ray.getOrigin().getZ());
		double d = Dot(normal,aux);


			  if(fabs(denom) > 1.e-6){
    		    double t = (d-Dot(normal,aux2))/denom;

    		    if(t>0){//The ray intersects...
                   distance=t;

                  //------------SHADER----------------

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

                  //Blend material color and light color
                  material.getColor(col);
                  col = col*lightColor;

                  //-------------END SHADER

    		      return true;
    		    }
    		    return false;
  			  }
  			  return false;
		}




