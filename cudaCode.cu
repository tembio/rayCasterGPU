
#ifndef CUDACODE_H
#define CUDACODE_H

#include "cudaCode.h"
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>
#include <stdio.h>


__global__ void CUDAintersection(int width,int height,float *rays,int numTriangles,float *triangles, float *output){ 

  //CUDA parameters
  int chunkSize = width*height / gridDim.x;
  int rayNumber = chunkSize*blockIdx.x + threadIdx.x;

  float closestT=1000000;
  output[rayNumber*4] = -1;
  output[rayNumber*4 +1] = 0;
  output[rayNumber*4 +2] = 0;
  output[rayNumber*4 +3] = 0;

  for(int p=0;p<numTriangles;p++){

      float  a,b,c,d,e,f,g,h,i,j,k,l,beta,gamma,t;

      //V1 of triangle
      a = triangles[(p*10)+1] - triangles[(p*10)+4];
      b = triangles[(p*10)+2] - triangles[(p*10)+5];
      c = triangles[(p*10)+3] - triangles[(p*10)+6];
      //V2 of triangle
      d = triangles[(p*10)+1] - triangles[(p*10)+7];
      e = triangles[(p*10)+2] - triangles[(p*10)+8];
      f = triangles[(p*10)+3] - triangles[(p*10)+9];
      
      g = rays[(rayNumber*6)+3];
      h = rays[(rayNumber*6)+4];
      i = rays[(rayNumber*6)+5];

      j = triangles[(p*10)+1] - rays[(rayNumber*6)];
      k = triangles[(p*10)+2] - rays[(rayNumber*6)+1];
      l = triangles[(p*10)+3] - rays[(rayNumber*6)+2];

      //Using Kramer to solve system to find barycentric coordinates
      
      beta = (j*(e*i-h*f) + k*(g*f-d*i) + l*(d*h-e*g))/(a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g));

      gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/(a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g));

      t = -((f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/(a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g)));

      
      if(t>0){
         //Test if it is closets intersection
         if(t<closestT){
           //test if inside triangle
           if(beta+gamma <=1 && 0<=beta && 0<=gamma){


                     output[rayNumber*4]=2;
             output[(rayNumber*4)+1] =37;
             output[(rayNumber*4)+2] =7;
             output[(rayNumber*4)+3] =57;
             //Calculate intersection point 
             closestT=t;
             output[rayNumber*4]=triangles[(p*10)];
             output[(rayNumber*4)+1] = rays[(rayNumber*6)] + rays[(rayNumber*6)+3]*t;
             output[(rayNumber*4)+2] = rays[(rayNumber*6)+1] + rays[(rayNumber*6)+4]*t;
             output[(rayNumber*4)+3] = rays[(rayNumber*6)+2] + rays[(rayNumber*6)+5]*t;
            }
         }
       }

    }//End calculation per ray

  }

  __global__ void CUDAshading(int width,int height,float *rays,float *interPoints,
                              float *normals,float *materials,
                              float *lights,int numLights,
                              float bgColorR,float bgColorG,float bgColorB,
                              float *pixelColors){
  //---INITIALIZATIONS---

  //CUDA parameters
  int chunkSize = width*height / gridDim.x;
  int pixelNumber = chunkSize*blockIdx.x + threadIdx.x;

  //Direction of the intersecting ray
  float rayDirX = rays[pixelNumber*3],
        rayDirY = rays[pixelNumber*3 + 1],
        rayDirZ = rays[pixelNumber*3 + 2];

  //Find the ID of  the intersected triangle and the intersection point
  int idTriangle = (int)interPoints[pixelNumber*4];

  //If no intersection...assign background color
  if(idTriangle==-1){
      pixelColors[pixelNumber*3] = bgColorR; 
      pixelColors[pixelNumber*3 + 1] = bgColorG;
      pixelColors[pixelNumber*3 + 2] = bgColorB;

  }else{

     float interX = interPoints[pixelNumber*4 + 1],
           interY = interPoints[pixelNumber*4 + 2],
           interZ = interPoints[pixelNumber*4 + 3];
   
     //Material of the intersected triangle
     float colorMatR = materials[idTriangle*5],
           colorMatG = materials[idTriangle*5 + 1],
           colorMatB = materials[idTriangle*5 + 2],
           materialDiffuse = materials[idTriangle*5 + 3],
           materialAmbient = materials[idTriangle*5 + 4];
   
     //Normal of the intersected triangle
     float normalX = normals[idTriangle*3],
           normalY = normals[idTriangle*3 + 1],
           normalZ = normals[idTriangle*3 + 2];
   
     //
     float ambientR = lights[0];
     float ambientG = lights[1];
     float ambientB = lights[2];
   
     //---CALCULATIONS---
   
     //Get ambient component of light
     float lightR=0,lightG=0,lightB=0;
     lightR = ambientR * materialAmbient;
     lightG = ambientG * materialAmbient;
     lightB = ambientB * materialAmbient;
   
     //Normalize normals
     float size=sqrt(normalX*normalX+normalY*normalY+normalZ*normalZ);
     normalX /= size;
     normalY /= size;
     normalZ /= size;
   
     //Calculate normal-light angle
     float costheta = normalX*rayDirX + normalY*rayDirY + normalZ*rayDirZ;
     if(costheta > 0){
         normalX *= -1;
         normalY *= -1;
         normalZ *= -1;
     }
   
     //Lights
     for(int i=0;i<numLights;i++){
   
       float tmpLightR = lights[3 + i*6];
       float tmpLightG = lights[3 + i*6 + 1];
       float tmpLightB = lights[3 + i*6 + 2];
   
       float tmpLightX = lights[3 + i*6 + 3];
       float tmpLightY = lights[3 + i*6 + 4];
       float tmpLightZ = lights[3 + i*6 + 5];
   
       //Calculate intersection-lightsource vector
       float vectorLightX = tmpLightX - interX;
       float vectorLightY = tmpLightY - interY;
       float vectorLightZ = tmpLightZ - interZ;


       float length = sqrt(vectorLightX*vectorLightX+vectorLightY*vectorLightY+vectorLightZ*vectorLightZ);
       vectorLightX /= length;
       vectorLightY /= length;
       vectorLightZ /= length;
   
       float cosphi = normalX*vectorLightX + normalY*vectorLightY + normalZ*vectorLightZ;
   
       if(cosphi>0){
         lightR += tmpLightR*cosphi*materialDiffuse;
         lightG += tmpLightG*cosphi*materialDiffuse;
         lightB += tmpLightB*cosphi*materialDiffuse;
       }
   
     }
   
     pixelColors[pixelNumber*3] = colorMatR * lightR; 
     pixelColors[pixelNumber*3 + 1] = colorMatG * lightB;
     pixelColors[pixelNumber*3 + 2] = colorMatB * lightG;

  }

}



void intersection(int numThreads,int width,int height,float *rays,int numTriangles,float *triangles, float *output){ 

   float *d_rays, *d_triangles, *d_out;

   //Allocate memory in device
   cudaMalloc((void **)&d_rays,      6 *height*width*sizeof(float));
   cudaMalloc((void **)&d_triangles, 10*numTriangles*sizeof(float));
   cudaMalloc((void **)&d_out,       4 *height*width*sizeof(float));

   // copy host matrix to device matrix
   cudaMemcpy(d_rays,rays, 6*height*width*sizeof(float), cudaMemcpyHostToDevice);
   cudaMemcpy(d_triangles,triangles, 10*numTriangles*sizeof(float), cudaMemcpyHostToDevice);

   // check for error
  cudaError_t error = cudaGetLastError();
  if(error != cudaSuccess)
  {
    // print the CUDA error message and exit
    printf("CUDA error: %s\n", cudaGetErrorString(error));
    exit(-1);
  }


   int numBlocks=height*width/numThreads;

   dim3 dimBlock(numThreads,1,1); // block size
   dim3 dimGrid(numBlocks,1); // nblocks

   // invoke the kernel
   CUDAintersection<<<dimGrid, dimBlock>>>(width,height,d_rays,numTriangles,d_triangles,d_out);

   cudaThreadSynchronize();

   cudaMemcpy(output, d_out, width*height*4*sizeof(float), cudaMemcpyDeviceToHost);
 
   cudaFree(d_rays);
   cudaFree(d_triangles);
   cudaFree(d_out);

}



void shading(int numThreads,int width,int height,int numTriangles,int numLights,
             float *rays,float *interPoints,float *normals,
             float *materials,float *lights,
             float bgColorR,float bgColorG,float bgColorB,
             float *pixelColors){

   float *d_rays, *d_inter, *d_normals, *d_materials,*d_lights, *d_pixelColors;

   cudaMalloc((void **)&d_rays,       3 *height*width*sizeof(float));
   cudaMalloc((void **)&d_inter,4 *height*width*sizeof(float));
   cudaMalloc((void **)&d_normals,    3 *numTriangles*sizeof(float));
   cudaMalloc((void **)&d_materials,  5 *numTriangles*sizeof(float));
   cudaMalloc((void **)&d_lights,    (6 *numLights +3)*sizeof(float));
   cudaMalloc((void **)&d_pixelColors,3 *height*width*sizeof(float));
 
   // copy host matrix to device matrix
   cudaMemcpy(d_rays, rays, 3*height*width*sizeof(float), cudaMemcpyHostToDevice);
   cudaMemcpy(d_inter, interPoints, 4*height*width*sizeof(float), cudaMemcpyHostToDevice);
   cudaMemcpy(d_normals, normals, 3*numTriangles*sizeof(float), cudaMemcpyHostToDevice);
   cudaMemcpy(d_materials, materials, 5*numTriangles*sizeof(float), cudaMemcpyHostToDevice);
   cudaMemcpy(d_lights, lights, (6 *numLights +3)*sizeof(float), cudaMemcpyHostToDevice);

   int numBlocks=height*width/numThreads;

   dim3 dimBlock(numThreads,1,1); // block size
   dim3 dimGrid(numBlocks,1); // nblocks

   // invoke the kernel
   CUDAshading<<<dimGrid, dimBlock>>>(width,height,d_rays,d_inter,d_normals,d_materials,d_lights,numLights,bgColorR,bgColorG,bgColorB,d_pixelColors);

   cudaThreadSynchronize();

   cudaMemcpy(pixelColors, d_pixelColors, width*height*3*sizeof(float), cudaMemcpyDeviceToHost);

       // check for error
    cudaError_t error = cudaGetLastError();
    if(error != cudaSuccess)
    {
      // print the CUDA error message and exit
      printf("CUDA error: %s\n", cudaGetErrorString(error));
      exit(-1);
    }

   cudaFree(d_rays);
   cudaFree(d_inter);
   cudaFree(d_normals);
   cudaFree(d_materials);
   cudaFree(d_materials);
   cudaFree(d_lights);
   cudaFree(d_pixelColors);

}























 void intersectionSerie(int width,int height,float *rays,int numTriangles,float *triangles, float *output){ 

  int numRays = width*height;

  for(int it=0;it<numRays;it++){

  int rayNumber=it;

  float rayOriginX= rays[(rayNumber*6)],
        rayOriginY= rays[(rayNumber*6)+1], 
        rayOriginZ= rays[(rayNumber*6)+2],
        rayDirectionX= rays[(rayNumber*6)+3],
        rayDirectionY= rays[(rayNumber*6)+4],
        rayDirectionZ= rays[(rayNumber*6)+5];

  float idIntersect=-1,closestT=1000000,interX=0,interY=0,interZ=0;

  for(int p=0;p<numTriangles;p++){

      float id=  triangles[(p*10)],
            v0x= triangles[(p*10)+1],
            v0y= triangles[(p*10)+2],
            v0z= triangles[(p*10)+3],
            v1x= triangles[(p*10)+4],
            v1y= triangles[(p*10)+5],
            v1z= triangles[(p*10)+6],
            v2x= triangles[(p*10)+7],
            v2y= triangles[(p*10)+8],
            v2z= triangles[(p*10)+9];

      double a,b,c,d,e,f,g,h,i,j,k,l,M,beta,gamma,t;

      //V1 of triangle
      a = v0x - v1x;
      b = v0y - v1y;
      c = v0z - v1z;
      //V2 of triangle
      d = v0x - v2x;
      e = v0y - v2y;
      f = v0z - v2z;
      
      g = rayDirectionX;
      h = rayDirectionY;
      i = rayDirectionZ;

      j = v0x - rayOriginX;
      k = v0y - rayOriginY;
      l = v0z - rayOriginZ;

      //Using Kramer to solve system to find barycentric coordinates
      
      M = a*(e*i-h*f)+b*(g*f-d*i)+c*(d*h-e*g);

      beta = (j*(e*i-h*f) + k*(g*f-d*i) + l*(d*h-e*g))/M;

      gamma = (i*(a*k-j*b)+h*(j*c-a*l)+g*(b*l-k*c))/M;

      t = (f*(a*k-j*b)+e*(j*c-a*l)+d*(b*l-k*c))/M;
      t= -t;

      if(t>0){
        //Test if it is closets intersection
        if(t<closestT){
          //test if inside triangle
          if(beta+gamma <=1 && 0<=beta && 0<=gamma){
            //Calculate intersection point 
            closestT=t;
            idIntersect=id;
            interX = rayOriginX + rayDirectionX*t;
            interY = rayOriginY + rayDirectionY*t;
            interZ = rayOriginZ + rayDirectionZ*t;
           }
        }
      }

    }//End calculation per ray

    output[rayNumber*4] = idIntersect;
    output[(rayNumber*4)+1] = interX;
    output[(rayNumber*4)+2] = interY;
    output[(rayNumber*4)+3] = interZ;

  }

  }




void shadingSerie(int width,int height,float *rays,float *interPoints,
                              float *normals,float *materials,
                              float *lights,int numLights,
                              float bgColorR,float bgColorG,float bgColorB,
                              float *pixelColors){
  //---INITIALIZATIONS---

  int numPixels = width*height;

for(int it=0;it<numPixels;it++){

  int pixelNumber=it;

  //Direction of the intersecting ray
  float rayDirX = rays[pixelNumber*3],
        rayDirY = rays[pixelNumber*3 + 1],
        rayDirZ = rays[pixelNumber*3 + 2];

  //Find the ID of  the intersected triangle and the intersection point
  int idTriangle = (int)interPoints[pixelNumber*4];

  //If no intersection...assign background color
  if(idTriangle==-1){

      pixelColors[pixelNumber*3] = bgColorR; 
      pixelColors[pixelNumber*3 + 1] = bgColorG;
      pixelColors[pixelNumber*3 + 2] = bgColorB;

      continue;
  }

  float interX = interPoints[pixelNumber*4 + 1],
        interY = interPoints[pixelNumber*4 + 2],
        interZ = interPoints[pixelNumber*4 + 3];

  //Material of the intersected triangle
  float colorMatR = materials[idTriangle*5],
        colorMatG = materials[idTriangle*5 + 1],
        colorMatB = materials[idTriangle*5 + 2],
        materialDiffuse = materials[idTriangle*5 + 3],
        materialAmbient = materials[idTriangle*5 + 4];

  //Normal of the intersected triangle
  float normalX = normals[idTriangle*3],
        normalY = normals[idTriangle*3 + 1],
        normalZ = normals[idTriangle*3 + 2];

  //
  float ambientR = lights[0];
  float ambientG = lights[1];
  float ambientB = lights[2];

  //---CALCULATIONS---

  //Get ambient component of light
  float lightR=0,lightG=0,lightB=0;
  lightR = ambientR * materialAmbient;
  lightG = ambientG * materialAmbient;
  lightB = ambientB * materialAmbient;

  //Normalize normals
  float size=sqrt(normalX*normalX+normalY*normalY+normalZ*normalZ);
  normalX /= size;
  normalY /= size;
  normalZ /= size;

  //Calculate normal-light angle
  float costheta = normalX*rayDirX + normalY*rayDirY + normalZ*rayDirZ;
  if(costheta > 0){
      normalX *= -1;
      normalY *= -1;
      normalZ *= -1;
  }

  //Lights
  for(int i=0;i<numLights;i++){

    float tmpLightR = lights[3 + i*6];
    float tmpLightG = lights[3 + i*6 + 1];
    float tmpLightB = lights[3 + i*6 + 2];

    float tmpLightX = lights[3 + i*6 + 3];
    float tmpLightY = lights[3 + i*6 + 4];
    float tmpLightZ = lights[3 + i*6 + 5];

    //Calculate intersection-lightsource vector
    float vectorLightX = tmpLightX - interX;
    float vectorLightY = tmpLightY - interY;
    float vectorLightZ = tmpLightZ - interZ;

    float length = sqrt(vectorLightX*vectorLightX+vectorLightY*vectorLightY+vectorLightZ*vectorLightZ);
    vectorLightX /= length;
    vectorLightY /= length;
    vectorLightZ /= length;

    float cosphi = normalX*vectorLightX + normalY*vectorLightY + normalZ*vectorLightZ;

    if(cosphi>0){
      lightR += tmpLightR*cosphi*materialDiffuse;
      lightG += tmpLightG*cosphi*materialDiffuse;
      lightB += tmpLightB*cosphi*materialDiffuse;
    }

  }

  pixelColors[pixelNumber*3] = colorMatR * lightR; 
  pixelColors[pixelNumber*3 + 1] = colorMatG * lightB;
  pixelColors[pixelNumber*3 + 2] = colorMatB * lightG;
}

}

















#endif