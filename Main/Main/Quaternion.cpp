#include "stdafx.h"

#include "Quaternion.h"
#include<math.h>

Quaternion::Quaternion():t(0),v(ThreeDVector()){}
Quaternion::Quaternion(double tt, double tx, double ty, double tz):t(tt),v(ThreeDVector(tx,ty,tz)){}
Quaternion::Quaternion(double tt, ThreeDVector tv):t(tt),v(ThreeDVector(tv)){}
Quaternion::~Quaternion(){}

Quaternion Quaternion::operator*(const Quaternion& q){
  Quaternion tmp;
  tmp.t = t*q.t-v.dot(q.v);
  ThreeDVector tv = v.cross(q.v);
  tv += v*q.t;
  tv += q.v*t;
  tmp.v = tv;
  return tmp;
}

void Quaternion::getRotation(const ThreeDVector& from, const ThreeDVector& to){
  ThreeDVector axis = to.cross(from);
  axis /= axis.abs();
  double cosTheta = to.dot(from)/(to.abs()*from.abs());
  double c = sqrt((1+cosTheta)/2);
  double s = sqrt((1-cosTheta)/2);
  axis *= s;
  t = c;
  v = axis;
}

Quaternion Quaternion::conjugation() const{
  Quaternion tmp(t,v);
  ThreeDVector zero;
  tmp.v = zero-tmp.v;
  return tmp;
}

ThreeDVector Quaternion::rotate(const ThreeDVector& from){
  Quaternion tmp(0,from);
  Quaternion ans = this->conjugation() * tmp * (*this);
  return ans.v;
}

double Quaternion::getT() const{
  return t;
}
ThreeDVector Quaternion::getV() const{
  return v;
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q){
  os << "(" << q.getT() << "," << q.getV() << ")";
  return os;
}

