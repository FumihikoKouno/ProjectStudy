/**
 * �̂̊e�֐߂̎O�������W��ێ����A����̑�����s���N���X
 * BodyDataNode���Ė��O�͂ǂ��Ȃ̂��Ȃ��Ďv���n�߂Ă���
 * Node����Ȃ���
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
// model�̃f�[�^���Q�l��ret�̊֐�to�̍��W��from�̍��W����v�Z���A�������B
  void makeNext(BodyDataNode& ret, BodyDataNode& model, int from, int to);
// model�f�[�^�������ɂƂ�A���[�U�����ׂ��p���̊e�֐߂̍��W������BodyDataNode��Ԃ�
	BodyDataNode convert(BodyDataNode& bdnode);

  // �e�֐߂̎O�������W
	ThreeDVector joints[15];

//	void add_children(double x,double y, double z);

private:
	ThreeDVector tv;
	std::vector<BodyDataNode*> children;
	Quaternion q;
};


std::ostream& operator<<(std::ostream& os, const BodyDataNode& b);

#endif