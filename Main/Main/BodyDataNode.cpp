#include "stdafx.h"

#include "BodyDataNode.h"
#include "NiteEnums.h"

BodyDataNode::BodyDataNode(){}
BodyDataNode::~BodyDataNode(){}

// model�̃f�[�^���Q�l��ret�̊֐�to�̍��W��from�̍��W����v�Z���A�������B
void BodyDataNode::make_next(BodyDataNode& ret, BodyDataNode& model, int from, int to){
	// model�f�[�^����from->to�̒P�ʃx�N�g�������߂�
	ThreeDVector dir = model.joints[to]-model.joints[from];
	dir /= dir.abs();
	// ���[�U��from�Ato�Ԃ̐߂̒��������߂�
	double abs = (joints[to]-joints[from]).abs();
	// model�f�[�^���瓾���P�ʃx�N�g���ƃ��[�U�̐߂̒�������V���ȓ_(to)�̍��W���Z�o����
	ret.joints[to] = ret.joints[from] + (dir * abs);
	return;
}

/**
 * ���Z�b�g�p�֐�
 */
void BodyDataNode::reset(){
//	first = true;
}

/**
 * model�f�[�^�������ɂƂ�A���[�U�����ׂ��p���̊e�֐߂̍��W������BodyDataNode��Ԃ�
 * �����Ȃ�������o�O����邩����convert�֐����g�������Ă�������
 */
void BodyDataNode::convert(BodyDataNode& model, ThreeDVector& diff, BodyDataNode& ret){
	// ��_�����߂�
	ret.joints[JOINT_TORSO] = model.joints[JOINT_TORSO] + diff;
	// ���X�ɐV���ȓ_���߂�
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


// b�������Ă���e�֐߂̎O�������W���o�͂���
std::ostream& operator<<(std::ostream& os, const BodyDataNode& b){
	ThreeDVector tmp;
	for(int i = 0; i < 15; i++){
		os << b.joints[i] << std::endl;
	}
	return os;
}
