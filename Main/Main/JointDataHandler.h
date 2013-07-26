#ifndef _JOINT_DATA_HANDLER_
#define _JOINT_DATA_HANDLER_

#include "ThreeDVector.h"
#include "Quaternion.h"

class JointDataHandler{
 public:
  JointDataHandler();
  ~JointDataHandler();
  // return Quaternion that rotate from->joint vector to joint^>to vector
  static Quaternion getQuaternion(ThreeDVector from, ThreeDVector joint, ThreeDVector to);

  // return ThreeDVector that is rotated from from->joint vector by q
  static ThreeDVector getNext(ThreeDVector from, ThreeDVector joint, Quaternion q, double length);
 private:
};

#endif
