/**
 * �̂̊e�֐߂̎O�������W��ێ����A����̑�����s���N���X
 * BodyDataNode���Ė��O�͂ǂ��Ȃ̂��Ȃ��Ďv���n�߂Ă���
 * Node����Ȃ���
 */
#ifndef _BODY_DATA_NODE_
#define _BODY_DATA_NODE_

#include "ThreeDVector.h"
//#include "Quaternion.h"
//#include <vector>
#include <iostream>
class BodyDataNode{
public:
	BodyDataNode();
	~BodyDataNode();
// model�̃f�[�^���Q�l��ret�̊֐�to�̍��W��from�̍��W����v�Z���A�������B
  void make_next(BodyDataNode& ret, BodyDataNode& model, int from, int to);
// model�f�[�^�������ɂƂ�A���[�U�����ׂ��p���̊e�֐߂̍��W������BodyDataNode��Ԃ�
//	BodyDataNode& convert(BodyDataNode& bdnode);
/**
 * model�f�[�^�������ɂƂ�A���[�U�����ׂ��p���̊e�֐߂̍��W������BodyDataNode��Ԃ�
 */
	void convert(BodyDataNode& bdnode, ThreeDVector& diff, BodyDataNode& ret);
/**
 * ���Z�b�g
 */ 
  static void reset();
  
  // �e�֐߂̎O�������W
	ThreeDVector joints[15];

//	void add_children(double x,double y, double z);

private:
};

std::ostream& operator<<(std::ostream& os, const BodyDataNode& b);

#endif