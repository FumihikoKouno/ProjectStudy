#include "stdafx.h"

#include "JointDataHandler.h"

JointDataHandler::JointDataHandler(){}
JointDataHandler::~JointDataHandler(){}

Quaternion JointDataHandler::getQuaternion(ThreeDVector from, ThreeDVector joint, ThreeDVector to){
  Quaternion tmp;
  tmp.getRotation(joint-from, to-joint);
  return tmp;
}

ThreeDVector JointDataHandler::getNext(ThreeDVector from, ThreeDVector joint, Quaternion q, double length){
  ThreeDVector tmp;
  tmp = q.rotate(joint-from);
  tmp /= tmp.abs();
  tmp *= length;
  tmp += joint;
  return tmp;
}
