
#ifndef Image_h
#define Image_h

#include "Material.h"
#include <string>
#include <cmath>

struct Pixel {
  unsigned char r, g, b;
};

class Image {
 public:
  Image(const std::string& name);
  Image(int xres, int yres);
  ~Image();
  void set(int x, int y, const Color& c) {
    Pixel p;
    p.r = c.r < 0.f? 0: c.r >= 1.f? 255 : (int)(c.r * 255.f);
    p.g = c.g < 0.f? 0: c.g >= 1.f? 255 : (int)(c.g * 255.f);
    p.b = c.b < 0.f? 0: c.b >= 1.f? 255 : (int)(c.b * 255.f);
    data[y][x] = p;
  }
  void write(const std::string& filename) const;
  double aspect_ratio() const {
    return double(xres)/double(yres);
  }
  int getXresolution() {
    return xres;
  }
  int getYresolution() {
    return yres;
  }
  Color interpolate(float x, float y) const {
    x *= xres; y *= yres;
    int ix = ((int)floor(x))%xres;
    if(ix<0)
      ix += xres;
    int ix1 = (ix+1)%xres;
    int iy = ((int)floor(y))%yres;
    if(iy<0)
      iy += yres;
    int iy1 = (iy+1)%yres;
    float fx = x-ix;
    float fy = y-iy;

    Color c00;
    c00.r = data[iy][ix].r;
    c00.g = data[iy][ix].g;
    c00.b = data[iy][ix].b;

    Color c01;
    c01.r = data[iy][ix1].r;
    c01.g = data[iy][ix1].g;
    c01.b = data[iy][ix1].b;

    Color c10;
    c10.r = data[iy1][ix].r;
    c10.g = data[iy1][ix].g;
    c10.b = data[iy1][ix].b;

    Color c11; 
    c11.r = data[iy1][ix1].r;
    c11.g = data[iy1][ix1].g;
    c11.b = data[iy1][ix1].b;

    Color c;
    c.r = c00.r*(1-fx)*(1-fy) + c01.r*fx*(1-fy) + c10.r*(1-fx)*fy + c11.r*fx*fy;
    c.g = c00.g*(1-fx)*(1-fy) + c01.g*fx*(1-fy) + c10.g*(1-fx)*fy + c11.g*fx*fy;
    c.b = c00.b*(1-fx)*(1-fy) + c01.b*fx*(1-fy) + c10.b*(1-fx)*fy + c11.b*fx*fy;

    c.r = c.r*(1./255);
    c.g = c.g*(1./255);
    c.b = c.b*(1./255);

    return c;

  }

 protected:
  Pixel** data;
  int xres, yres;
  Image(const Image&);
  Image& operator=(const Image&);
};

#endif