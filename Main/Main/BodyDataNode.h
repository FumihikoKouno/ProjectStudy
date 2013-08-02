/**
 * 体の各関節の三次元座標を保持し、それの操作を行うクラス
 * BodyDataNodeって名前はどうなのかなって思い始めている
 * Nodeじゃないし
 */
#ifndef _BODY_DATA_NODE_
#define _BODY_DATA_NODE_

#include "ThreeDVector.h"
#include "Quaternion.h"
#include <vector>
#include <iostream>
class BodyDataNode{
public:
	BodyDataNode();
//	BodyDataNode(double x,double y, double z);
//	BodyDataNode(ThreeDVector tv, Quaternion q);
	~BodyDataNode();
// modelのデータを参考にretの関節toの座標をfromの座標から計算し、代入する。
  void makeNext(BodyDataNode& ret, BodyDataNode& model, int from, int to);
// modelデータを引数にとり、ユーザが取るべき姿勢の各関節の座標を持つBodyDataNodeを返す
	BodyDataNode convert(BodyDataNode& bdnode);

  // 各関節の三次元座標
	ThreeDVector joints[15];

//	void add_children(double x,double y, double z);

private:
	ThreeDVector tv;
	std::vector<BodyDataNode*> children;
	Quaternion q;
};


std::ostream& operator<<(std::ostream& os, const BodyDataNode& b);

#endif