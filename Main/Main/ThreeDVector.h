#ifndef _ThreeD_VECTOR_H_
#define _ThreeD_VECTOR_H_

#include <iostream>
class ThreeDVector{
public:
  ThreeDVector();
  ThreeDVector(double, double, double);
  ThreeDVector(const ThreeDVector&);
  ~ThreeDVector();
  double getX() const;
  double getY() const;
  double getZ() const;
  double dot(const ThreeDVector& v) const;
  double abs() const;
  ThreeDVector operator*(const double& d) const;
  ThreeDVector& operator*=(const double& d);
  ThreeDVector operator/(const double& d) const;
  ThreeDVector& operator/=(const double& d);
  ThreeDVector cross(const ThreeDVector& v) const;
  ThreeDVector operator+(const ThreeDVector& v) const;
  ThreeDVector& operator+=(const ThreeDVector& v);
  ThreeDVector operator-(const ThreeDVector& v) const;
  ThreeDVector& operator-=(const ThreeDVector& v);
  double getThetaXY(const ThreeDVector& v);
  double getThetaYZ(const ThreeDVector& v);
  double getThetaZX(const ThreeDVector& v);
private:
  double x,y,z;
};

std::ostream& operator<<(std::ostream& os, const ThreeDVector& p);

#endif
