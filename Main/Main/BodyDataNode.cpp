#include "stdafx.h"

#include "BodyDataNode.h"
#include "NiteEnums.h"

BodyDataNode::BodyDataNode(){}
BodyDataNode::~BodyDataNode(){}

// modelのデータを参考にretの関節toの座標をfromの座標から計算し、代入する。
void BodyDataNode::make_next(BodyDataNode& ret, BodyDataNode& model, int from, int to){
	// modelデータからfrom->toの単位ベクトルを求める
	ThreeDVector dir = model.joints[to]-model.joints[from];
	dir /= dir.abs();
	// ユーザのfrom、to間の節の長さを求める
	double abs = (joints[to]-joints[from]).abs();
	// modelデータから得た単位ベクトルとユーザの節の長さから新たな点(to)の座標を算出する
	ret.joints[to] = ret.joints[from] + (dir * abs);
	return;
}

/**
 * リセット用関数
 */
void BodyDataNode::reset(){
//	first = true;
}

/**
 * modelデータを引数にとり、ユーザが取るべき姿勢の各関節の座標を持つBodyDataNodeを返す
 * 動かなかったらバグを取るか下のconvert関数を使うかしてください
 */
void BodyDataNode::convert(BodyDataNode& model, ThreeDVector& diff, BodyDataNode& ret){
	// 基準点を決める
	ret.joints[JOINT_TORSO] = model.joints[JOINT_TORSO] + diff;
	// 徐々に新たな点を定める
	make_next(ret,model,JOINT_TORSO,          JOINT_NECK);
	make_next(ret,model,JOINT_NECK,           JOINT_HEAD);
	make_next(ret,model,JOINT_TORSO,          JOINT_RIGHT_SHOULDER);
	make_next(ret,model,JOINT_RIGHT_SHOULDER, JOINT_RIGHT_ELBOW);
	make_next(ret,model,JOINT_RIGHT_ELBOW,    JOINT_RIGHT_HAND);
	make_next(ret,model,JOINT_TORSO,          JOINT_LEFT_SHOULDER);
	make_next(ret,model,JOINT_LEFT_SHOULDER,  JOINT_LEFT_ELBOW);
	make_next(ret,model,JOINT_LEFT_ELBOW,     JOINT_LEFT_HAND);
	make_next(ret,model,JOINT_TORSO,          JOINT_RIGHT_HIP);
	make_next(ret,model,JOINT_RIGHT_HIP,      JOINT_RIGHT_KNEE);
	make_next(ret,model,JOINT_RIGHT_KNEE,     JOINT_RIGHT_FOOT);
	make_next(ret,model,JOINT_TORSO,          JOINT_LEFT_HIP);
	make_next(ret,model,JOINT_LEFT_HIP,       JOINT_LEFT_KNEE);
	make_next(ret,model,JOINT_LEFT_KNEE,      JOINT_LEFT_FOOT);
}


// bが持っている各関節の三次元座標を出力する
std::ostream& operator<<(std::ostream& os, const BodyDataNode& b){
	ThreeDVector tmp;
	for(int i = 0; i < 15; i++){
		os << b.joints[i] << std::endl;
	}
	return os;
}
