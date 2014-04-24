#include "Scene.h"
#include "Image.h"
#include <string>
#include "Triangle.h"
#include "Material.h"
#include "Plane.h"
#include "Vector.h"
#include "Sphere.h"
#include "cudaCode.h"
#include <vector>
#include <time.h>


void diamond(float x,float y,float z,Scene &s,Material &m1,Material &m2,float heightD=50,float widthX=50, float widthZ=50);

int main(int argc,char* argv[]){

  int numDiamonds;
  if(argc==1)
   numDiamonds = 1;
  else
   numDiamonds = atoi(argv[1]);

   
  int width=256,height=256;

  if(argc>2){
    width=atoi(argv[2]);
    height=atoi(argv[3]);
  }


  //Define number of threads per block
  int numThreadsPerBlock=256;

  if(argc==5)
    numThreadsPerBlock=atoi(argv[4]);

  bool parallel=true;

  if(argc==6)
    parallel=false;


  Image im(width,height);

  std::string filename = "image.ppm";

  //DECLARING SCENE...

  Scene s;

  //Background
  Color bg;
  bg.r=0.3;bg.g=0.3;bg.b=1;

  //Light
  Light light(Point(200,200,100),Color(150,150,150));
  s.addLight(light);

  //Materials
  Material m(Color(0,0.1,0),0,0.05,0.1);//Color,specular,diffuse,ambient
  Material m2(Color(0.5,0,0),0,0.05,0.05);
  Material m3(Color(0,0.1,0.2),0,0.05,0.3);
  Material m4(Color(0.2,0.1,0.0),0,0.05,0.3);

  s.setBackground(bg);
  s.setAmbient(Color(30,30,30));
  s.setEyePosition(Point(0,50,-150));

  float  x=100,y=50,z=50;


  for(int cont=0;cont<numDiamonds;cont++)
    diamond(x-30*cont,y+10*cont,z+50*cont,s,m2,m3);

  x=-100,y=0,z=-50;

  for(int cont=0;cont<numDiamonds;cont++)
    diamond(x+30*cont,y,z+50*cont,s,m2,m4);


/*

c3*----*c2
  |t10/|
  |  / |
  | /  |
  |/ t9|
c1*----*c4

*/

  Point c1,c2,c3,c4;
  c1=Point(-1000,0,-1000);
  c2=Point(1000,0,1000);
  c3=Point(-1000,0,1000);
  c4=Point(1000,0,-1000);
  Triangle* t9 = new Triangle(c1,c4,c2,m);
  Triangle* t10 = new Triangle(c1,c2,c3,m);

  s.addPrimitive(t9);
  s.addPrimitive(t10);


  //PREPARE DATA FOR GPU...

  
  float *h_raysInter,*h_raysShading,*h_triangles,*h_interPoints,*h_normals,*h_materials,*h_lights,*pixelColors;

   //Allocate memory for intersection values
   h_triangles  = (float*) malloc(10*s.getNumPrimitives()*sizeof(float));// new float[10*s.getNumPrimitives()];
   h_raysInter  = (float*) malloc(width*height*6*sizeof(float));// new float[width*height*6];
   h_interPoints= (float*) malloc(width*height*4*sizeof(float));//new float[width*height*4];

   //Allocate memory for shading values
   h_raysShading = (float*) malloc(width*height*3*sizeof(float));//new float[width*height*3];
   h_normals = (float*) malloc(s.getNumPrimitives()*3*sizeof(float));//new float[s.getNumPrimitives()*3];
   h_materials = (float*) malloc(s.getNumPrimitives()*5*sizeof(float));//new float[s.getNumPrimitives()*5];
   h_lights = (float*) malloc((3+6*s.getNumLights())*sizeof(float));//new float[3+6*s.getNumLights()];

   pixelColors = (float*) malloc(width*height*3*sizeof(float));//new float[width*height*3];

   Point bottomLeftCorner(-width/2,-height/2,0); 

  //Load rays
  for(int i=0;i<width;i++)
    for(int j=0;j<height;j++){
      Point aux = bottomLeftCorner+Point((float)i,(float)j,0)-s.getEyePosition();
      Vector dir(aux.getX(),aux.getY(),aux.getZ());
     
      Ray auxRay(s.getEyePosition(),dir);

      auxRay.getDirection().normalize();

      h_raysInter[j*6 + width*i*6] = auxRay.getOrigin().getX();
      h_raysInter[j*6 + width*i*6 +1] = auxRay.getOrigin().getY();
      h_raysInter[j*6 + width*i*6 +2] = auxRay.getOrigin().getZ();

      h_raysInter[j*6 + width*i*6 +3] = auxRay.getDirection().getX();
      h_raysInter[j*6 + width*i*6 +4] = auxRay.getDirection().getY();
      h_raysInter[j*6 + width*i*6 +5] = auxRay.getDirection().getZ();


      h_raysShading[j*3 + width*i*3] = auxRay.getDirection().getX();
      h_raysShading[j*3 + width*i*3 +1] = auxRay.getDirection().getY();
      h_raysShading[j*3 + width*i*3 +2] = auxRay.getDirection().getZ();
  }

  //Load info from triangles
  std::vector<Primitive*> auxVec;
  s.getPrimitives(auxVec);
  for(int i=0;i<auxVec.size();i++){

    auxVec[i]->getNormal().normalize();

    //Normals info
    h_normals[i*3] = auxVec[i]->getNormal().getX();
    h_normals[i*3 +1] = auxVec[i]->getNormal().getY();
    h_normals[i*3 +2] = auxVec[i]->getNormal().getZ();

    //Material info
    Color auxColor;
    auxVec[i]->getMaterial().getColor(auxColor);

    h_materials[i*5] = auxColor.r;
    h_materials[i*5+1] = auxColor.g;
    h_materials[i*5+2] = auxColor.b;
    h_materials[i*5+3] = auxVec[i]->getMaterial().getDiffuse();
    h_materials[i*5+4] = auxVec[i]->getMaterial().getAmbient();

    h_triangles[i*10]=(float)i;
    h_triangles[i*10+1]=(auxVec[i]->getVertex1()).getX();
    h_triangles[i*10+2]=(auxVec[i]->getVertex1()).getY();
    h_triangles[i*10+3]=(auxVec[i]->getVertex1()).getZ();
    h_triangles[i*10+4]=(auxVec[i]->getVertex2()).getX();
    h_triangles[i*10+5]=(auxVec[i]->getVertex2()).getY();
    h_triangles[i*10+6]=(auxVec[i]->getVertex2()).getZ();
    h_triangles[i*10+7]=(auxVec[i]->getVertex3()).getX();
    h_triangles[i*10+8]=(auxVec[i]->getVertex3()).getY();
    h_triangles[i*10+9]=(auxVec[i]->getVertex3()).getZ();

  }

  //Load info from lights: [(ambientR,ambientG,ambientB),(posX,posY,posZ,colorR,colorG,colorB),...]
  Color auxColor;
  s.getAmbient(auxColor);

  h_lights[0]=auxColor.r;
  h_lights[1]=auxColor.g;
  h_lights[2]=auxColor.b;

  for(int i=0;i<s.getNumLights();i++){
 
    s.getLight(i).getColor(auxColor);
 
    h_lights[3 + i*6] = s.getLight(i).getPosition().getX();
    h_lights[3 + i*6 + 1] = s.getLight(i).getPosition().getY();
    h_lights[3 + i*6 + 2] = s.getLight(i).getPosition().getZ();
    h_lights[3 + i*6 + 3] = auxColor.r;
    h_lights[3 + i*6 + 4] = auxColor.g;
    h_lights[3 + i*6 + 5] = auxColor.b;
  }

  clock_t t;

  if(parallel){

    t = clock();

    //Calculate intersection ray-triangles
    intersection(numThreadsPerBlock,width,height,h_raysInter,s.getNumPrimitives(),h_triangles,h_interPoints);



    //Calculate shading
    shading(numThreadsPerBlock,width,height,s.getNumPrimitives(),s.getNumLights(),
             h_raysShading,h_interPoints,h_normals,
             h_materials,h_lights,bg.r,bg.g,bg.b,pixelColors);

    t = clock()  - t;

  }else{//CPU Serial version

    t = clock();

    //Calculate intersection ray-triangles
    intersectionSerie(width,height,h_raysInter,s.getNumPrimitives(),h_triangles,h_interPoints);

    //Calculate shading
    shadingSerie(width,height,h_raysShading,h_interPoints,
                              h_normals,h_materials,
                              h_lights,s.getNumLights(),
                              bg.r,bg.g,bg.b,pixelColors);

    t = clock()  - t;
  }

  free(h_triangles);
  free(h_raysInter);
  free(h_raysShading);
  free(h_interPoints);
  free(h_normals);
  free(h_materials);
  free(h_lights);

  //s.rayTracer(i);  

  for(int i=0;i<width;i++)
    for(int j=0;j<height;j++){

       Color finalColor;
 
       finalColor.r = pixelColors[j*3 + width*i*3];      
       finalColor.g = pixelColors[j*3 + width*i*3 +1];
       finalColor.b = pixelColors[j*3 + width*i*3 +2];

       im.set(i,j,finalColor);

  }

  im.write(filename);
  free(pixelColors);

  printf("Resolution: %i %i\n",width,height);
  printf("Number of triangles: %i\n",s.getNumPrimitives());
  printf("Time (s):%f \n", ((float)t) / CLOCKS_PER_SEC);
  

}


void diamond(float x,float y,float z,Scene &s,Material &m1,Material &m2,float heightD,float widthX, float widthZ){
  
/*
       *e
      /|\ 
     / |d\
   a*--*b-*c
     \ | /
      \|/
       *f
*/

  Point a(x-widthX,y,z),b(x,y,z-widthZ),d(x,y,z+widthZ),c(x+widthX,y,z)
                     ,e(x,y+heightD,z),f(x,y-heightD,z);

  Triangle* t1 = new Triangle(a,b,e,m1);
  Triangle* t2 = new Triangle(a,b,f,m2);
  Triangle* t3 = new Triangle(b,c,e,m2);
  Triangle* t4 = new Triangle(b,c,f,m1);
  Triangle* t5 = new Triangle(a,d,e,m2);
  Triangle* t6 = new Triangle(a,d,f,m1);
  Triangle* t7 = new Triangle(d,c,e,m1);
  Triangle* t8 = new Triangle(d,c,f,m2);

  s.addPrimitive(t1);
  s.addPrimitive(t2);
  s.addPrimitive(t3);
  s.addPrimitive(t4);
  s.addPrimitive(t5);
  s.addPrimitive(t6);
  s.addPrimitive(t7);
  s.addPrimitive(t8);


}
