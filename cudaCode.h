#ifndef CUDACODE_H
#define CUDACODE_H

#include <cuda.h> 

/*
  numThreads = number of threads per CUDA block
  width = image width
  height = image height
  rays = array with all the calculated rays 
  numTriangles = number of triangles in the scene
  triangles = array with the geometry of the triangles and its id
  output = for every ray, there is an id of the intersected object (-1 if no intersection) and the coords of the intersected point
*/
void intersection(int numThreads,int width,int height,float *rays,int numTriangles,float *triangles, float *output);


void shading(int numThreads,int width,int height,int numTriangles,int numLights,
             float *rays,float *interPoints,float *normals,
             float *materials,float *lights,
             float bgColorR,float bgColorG,float bgColorB,
             float *pixelColors);





void intersectionSerie(int width,int height,float *rays,int numTriangles,float *triangles, float *output);
void shadingSerie(int width,int height,float *rays,float *interPoints,
                              float *normals,float *materials,
                              float *lights,int numLights,
                              float bgColorR,float bgColorG,float bgColorB,
                              float *pixelColors);


#endif


