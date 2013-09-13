#include "stdafx.h"

#include "BodyDataNode.h"
#include "NiteEnums.h"
//#include "JointDataHandler.h"

//ThreeDVector BodyDataNode::diff = ThreeDVector();
//bool BodyDataNode::first = true;

BodyDataNode::BodyDataNode(){}
//BodyDataNode::BodyDataNode(double x,double y, double z):tv(x,y,z){}
//BodyDataNode::BodyDataNode(ThreeDVector t,Quaternion qu):tv(t),q(qu){}
BodyDataNode::~BodyDataNode(){}
/*
BodyDataNode::~BodyDataNode(){
	for(std::vector<BodyDataNode*>::iterator it = children.begin(); it != children.end(); it++)
		delete *it;
}

void BodyDataNode::add_children(double x,double y, double z)
{
	Quaternion tmpq;
	ThreeDVector tmpt(x,y,z);
	tmpq.getRotation(tv,tmpt);
	children.push_back(new BodyDataNode(tmpt,tmpq));
}*/

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

/*
// 以前のconvert関数、上に定義してあるのと入出力は同じ(はず)
// 変換効率が悪そうだったので一旦コメントアウトし、上に新たなconvert関数を定義した
BodyDataNode BodyDataNode::convert(BodyDataNode& bdnode){
	BodyDataNode bd;
	double abs;
	abs = (joints[JOINT_RIGHT_KNEE]-joints[JOINT_RIGHT_FOOT]).abs();
	Quaternion tmp;
	tmp.getRotation(joints[JOINT_RIGHT_KNEE]-joints[JOINT_RIGHT_FOOT],bdnode.joints[JOINT_RIGHT_KNEE]-bdnode.joints[JOINT_RIGHT_FOOT]);
	bd.joints[JOINT_RIGHT_FOOT] = joints[JOINT_RIGHT_FOOT];
	bd.joints[JOINT_RIGHT_KNEE] = tmp.rotate(joints[JOINT_RIGHT_KNEE]-joints[JOINT_RIGHT_FOOT])+joints[JOINT_RIGHT_FOOT];


	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_RIGHT_FOOT],
		bdnode.joints[JOINT_RIGHT_KNEE],
		bdnode.joints[JOINT_RIGHT_HIP]
	);
	bd.joints[JOINT_RIGHT_HIP] = JointDataHandler::getNext(
		bd.joints[JOINT_RIGHT_FOOT],
		bd.joints[JOINT_RIGHT_KNEE],
		tmp,
		(joints[JOINT_RIGHT_HIP]-joints[JOINT_RIGHT_KNEE]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_RIGHT_KNEE],
		bdnode.joints[JOINT_RIGHT_HIP],
		bdnode.joints[JOINT_TORSO]
	);
	bd.joints[JOINT_TORSO] = JointDataHandler::getNext(
		bd.joints[JOINT_RIGHT_KNEE],
		bd.joints[JOINT_RIGHT_HIP],
		tmp,
		(joints[JOINT_TORSO]-joints[JOINT_RIGHT_HIP]).abs()
	);
	

	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_RIGHT_HIP],
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_LEFT_HIP]
	);
	bd.joints[JOINT_LEFT_HIP] = JointDataHandler::getNext(
		bd.joints[JOINT_RIGHT_HIP],
		bd.joints[JOINT_TORSO],
		tmp,
		(joints[JOINT_LEFT_HIP]-joints[JOINT_TORSO]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_LEFT_HIP],
		bdnode.joints[JOINT_LEFT_KNEE]
	);
	bd.joints[JOINT_LEFT_KNEE] = JointDataHandler::getNext(
		bd.joints[JOINT_TORSO],
		bd.joints[JOINT_LEFT_HIP],
		tmp,
		(joints[JOINT_LEFT_KNEE]-joints[JOINT_LEFT_HIP]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_LEFT_HIP],
		bdnode.joints[JOINT_LEFT_KNEE],
		bdnode.joints[JOINT_LEFT_FOOT]
	);
	bd.joints[JOINT_LEFT_FOOT] = JointDataHandler::getNext(
		bd.joints[JOINT_LEFT_HIP],
		bd.joints[JOINT_LEFT_KNEE],
		tmp,
		(joints[JOINT_LEFT_FOOT]-joints[JOINT_LEFT_KNEE]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_RIGHT_HIP],
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_RIGHT_SHOULDER]
	);
	bd.joints[JOINT_RIGHT_SHOULDER] = JointDataHandler::getNext(
		bd.joints[JOINT_RIGHT_HIP],
		bd.joints[JOINT_TORSO],
		tmp,
		(joints[JOINT_RIGHT_SHOULDER]-joints[JOINT_TORSO]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_RIGHT_SHOULDER],
		bdnode.joints[JOINT_RIGHT_ELBOW]
	);
	bd.joints[JOINT_RIGHT_ELBOW] = JointDataHandler::getNext(
		bd.joints[JOINT_TORSO],
		bd.joints[JOINT_RIGHT_SHOULDER],
		tmp,
		(joints[JOINT_RIGHT_ELBOW]-joints[JOINT_RIGHT_SHOULDER]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_RIGHT_SHOULDER],
		bdnode.joints[JOINT_RIGHT_ELBOW],
		bdnode.joints[JOINT_RIGHT_HAND]
	);
	bd.joints[JOINT_RIGHT_HAND] = JointDataHandler::getNext(
		bd.joints[JOINT_RIGHT_SHOULDER],
		bd.joints[JOINT_RIGHT_ELBOW],
		tmp,
		(joints[JOINT_RIGHT_HAND]-joints[JOINT_RIGHT_ELBOW]).abs()
	);	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_LEFT_HIP],
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_LEFT_SHOULDER]
	);
	bd.joints[JOINT_LEFT_SHOULDER] = JointDataHandler::getNext(
		bd.joints[JOINT_LEFT_HIP],
		bd.joints[JOINT_TORSO],
		tmp,
		(joints[JOINT_LEFT_SHOULDER]-joints[JOINT_TORSO]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_LEFT_SHOULDER],
		bdnode.joints[JOINT_LEFT_ELBOW]
	);
	bd.joints[JOINT_LEFT_ELBOW] = JointDataHandler::getNext(
		bd.joints[JOINT_TORSO],
		bd.joints[JOINT_LEFT_SHOULDER],
		tmp,
		(joints[JOINT_LEFT_ELBOW]-joints[JOINT_LEFT_SHOULDER]).abs()
	);
	
	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_LEFT_SHOULDER],
		bdnode.joints[JOINT_LEFT_ELBOW],
		bdnode.joints[JOINT_LEFT_HAND]
	);
	bd.joints[JOINT_LEFT_HAND] = JointDataHandler::getNext(
		bd.joints[JOINT_LEFT_SHOULDER],
		bd.joints[JOINT_LEFT_ELBOW],
		tmp,
		(joints[JOINT_LEFT_HAND]-joints[JOINT_LEFT_ELBOW]).abs()
	);

	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_RIGHT_HIP],
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_NECK]
	);
	bd.joints[JOINT_NECK] = JointDataHandler::getNext(
		bd.joints[JOINT_RIGHT_HIP],
		bd.joints[JOINT_TORSO],
		tmp,
		(joints[JOINT_NECK]-joints[JOINT_TORSO]).abs()
	);

	tmp = JointDataHandler::getQuaternion(
		bdnode.joints[JOINT_TORSO],
		bdnode.joints[JOINT_NECK],
		bdnode.joints[JOINT_HEAD]
	);
	bd.joints[JOINT_HEAD] = JointDataHandler::getNext(
		bd.joints[JOINT_TORSO],
		bd.joints[JOINT_NECK],
		tmp,
		(joints[JOINT_HEAD]-joints[JOINT_NECK]).abs()
	);

	ThreeDVector diff = joints[JOINT_TORSO] - bd.joints[JOINT_TORSO];
	for(int i = 0; i < 15; i++){
		bd.joints[i] += diff;
	}

	return bd;
}
*/

// bが持っている各関節の三次元座標を出力する
std::ostream& operator<<(std::ostream& os, const BodyDataNode& b){
	ThreeDVector tmp;
	for(int i = 0; i < 15; i++){
		os << b.joints[i] << std::endl;
	}
	/*
	os << "HEAD : " << b.joints[JOINT_HEAD] << std::endl;
	os << "NECK : " << b.joints[JOINT_NECK] << std::endl;
	os << "TORSO : " << b.joints[JOINT_TORSO] << std::endl;
	os << "LEFT SHOULDER : " << b.joints[JOINT_LEFT_SHOULDER] << std::endl;
	os << "LEFT ELBOW : " << b.joints[JOINT_LEFT_ELBOW] << std::endl;
	os << "LEFT HAND : " << b.joints[JOINT_LEFT_HAND] << std::endl;
	os << "LEFT HIP : " << b.joints[JOINT_LEFT_HIP] << std::endl;
	os << "LEFT KNEE : " << b.joints[JOINT_LEFT_KNEE] << std::endl;
	os << "LEFT FOOT : " << b.joints[JOINT_LEFT_FOOT] << std::endl;
	os << "RIGHT SHOULDER : " << b.joints[JOINT_RIGHT_SHOULDER] << std::endl;
	os << "RIGHT ELBOW : " << b.joints[JOINT_RIGHT_ELBOW] << std::endl;
	os << "RIGHT HAND : " << b.joints[JOINT_RIGHT_HAND] << std::endl;
	os << "RIGHT HIP : " << b.joints[JOINT_RIGHT_HIP] << std::endl;
	os << "RIGHT KNEE : " << b.joints[JOINT_RIGHT_KNEE] << std::endl;
	os << "RIGHT FOOT : " << b.joints[JOINT_RIGHT_FOOT] << std::endl;
	*/
	return os;
}

	/*
	typedef enum
{
	JOINT_HEAD,
	JOINT_NECK,

	JOINT_LEFT_SHOULDER,
	JOINT_RIGHT_SHOULDER,
	JOINT_LEFT_ELBOW,
	JOINT_RIGHT_ELBOW,
	JOINT_LEFT_HAND,
	JOINT_RIGHT_HAND,

	JOINT_TORSO,

	JOINT_LEFT_HIP,
	JOINT_RIGHT_HIP,
	JOINT_LEFT_KNEE,
	JOINT_RIGHT_KNEE,
	JOINT_LEFT_FOOT,
	JOINT_RIGHT_FOOT,
} JointType;
	*/
