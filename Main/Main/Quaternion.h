#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "ThreeDVector.h"
class Quaternion{
 public:
  Quaternion();
  Quaternion(double, double, double, double);
  Quaternion(double, ThreeDVector);
  ~Quaternion();
  Quaternion operator*(const Quaternion& q);
  Quaternion& operator*=(const Quaternion& q);
  void getRotation(const ThreeDVector& from, const ThreeDVector& to);
  ThreeDVector rotate(const ThreeDVector& from);
  Quaternion conjugation() const;
  double getT() const;
  ThreeDVector getV() const;
 private:
  double t;
  ThreeDVector v;
};

std::ostream& operator<<(std::ostream& os, const Quaternion& q);

#endif
