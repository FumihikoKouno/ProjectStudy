#include "stdafx.h"

#include <math.h>
#include "ThreeDVector.h"

ThreeDVector::ThreeDVector():x(0),y(0),z(0){}
ThreeDVector::ThreeDVector(double ix, double iy, double iz):x(ix),y(iy),z(iz){}
ThreeDVector::ThreeDVector(const ThreeDVector& v):x(v.x),y(v.y),z(v.z){};
ThreeDVector::~ThreeDVector(){}
double ThreeDVector::getX() const {return x;}
double ThreeDVector::getY() const {return y;}
double ThreeDVector::getZ() const {return z;}
double ThreeDVector::dot(const ThreeDVector& v) const {
  return x*v.x+y*v.y+z*v.z;
}
double ThreeDVector::abs() const {
  return sqrt(x*x+y*y+z*z);
}
ThreeDVector ThreeDVector::cross(const ThreeDVector& v) const {
  ThreeDVector tmp;
  tmp.x = y*v.z-z*v.y;
  tmp.y = z*v.x-x*v.z;
  tmp.z = x*v.y-y*v.x;
  return tmp;
}
ThreeDVector& ThreeDVector::operator*=(const double& d){
  x *= d;
  y *= d;
  z *= d;
  return *this;
}
ThreeDVector ThreeDVector::operator*(const double& d) const {
  ThreeDVector tmp;
  tmp.x = x*d;
  tmp.y = y*d;
  tmp.z = z*d;
  return tmp;
}
ThreeDVector& ThreeDVector::operator/=(const double& d){
  x /= d;
  y /= d;
  z /= d;
  return *this;
}
ThreeDVector ThreeDVector::operator/(const double& d) const {
  ThreeDVector tmp;
  tmp.x = x/d;
  tmp.y = y/d;
  tmp.z = z/d;
  return tmp;
}

ThreeDVector ThreeDVector::operator+(const ThreeDVector& v) const {
  ThreeDVector tmp;
  tmp.x = x + v.x;
  tmp.y = y + v.y;
  tmp.z = z + v.z;
  return tmp;
}
ThreeDVector& ThreeDVector::operator+=(const ThreeDVector& v){
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}
ThreeDVector ThreeDVector::operator-(const ThreeDVector& v) const {
  ThreeDVector tmp;
  tmp.x = x - v.x;
  tmp.y = y - v.y;
  tmp.z = z - v.z;
  return tmp;
}
ThreeDVector& ThreeDVector::operator-=(const ThreeDVector& v){
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}
double ThreeDVector::getThetaXY(const ThreeDVector& v){
  double vx = v.x;
  double vy = v.y;
  double theta = acos((x*vx+y*vy)/(sqrt(x*x+y*y)*sqrt(vx*vx+vy*vy)));
  if(x*vy-y*vx > 0) return theta;
  else return -theta;
}
double ThreeDVector::getThetaYZ(const ThreeDVector& v){
  double vy = v.y;
  double vz = v.z;
  double theta = acos((y*vy+z*vz)/(sqrt(y*y+z*z)*sqrt(vy*vy+vz*vz)));
  if(y*vz-z*vy > 0) return theta;
  else return -theta;
}
double ThreeDVector::getThetaZX(const ThreeDVector& v){
  double vz = v.z;
  double vx = v.x;
  double theta = acos((z*vz+x*vx)/(sqrt(z*z+x*x)*sqrt(vz*vz+vx*vx)));
  if(z*vx-x*vz > 0) return theta;
  else return -theta;
}


std::ostream& operator<<(std::ostream& os, const ThreeDVector& v){
  os << v.getX() << " " << v.getY() << " " << v.getZ();
  return os;
}
