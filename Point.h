#ifndef POINT_H
#define POINT_H


class Point{
	public:
	  Point();

	  Point(float x, float y, float z);

	  Point(const Point& p);

	  ~Point();

	  Point & operator=(const Point& p);

	  Point operator-(const Point& p);
	  
	  Point operator+(const Point& p);

	  bool operator==(const Point& p);

	  float getX();

	  float getY();

	  float getZ();

	  void setX(float x);

	  void setY(float y);

	  void setZ(float z);

	private:
	  float x;
	  float y;
	  float z;


};


#endif