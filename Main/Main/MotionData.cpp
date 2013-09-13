#include <fstream>

#include "stdafx.h"
#include "NiteEnums.h"

#include "MotionData.h"


MotionData::MotionData(){}
MotionData::~MotionData(){}

void MotionData::reset(){ data.clear(); }
void MotionData::add(BodyDataNode& bd){ data.push_back(bd); }
BodyDataNode& MotionData::operator[](int i){ return data[i]; }
BodyDataNode& MotionData::back(){ return data.back(); }

void MotionData::output(std::string s){
	std::ofstream out(s);
	dump(s);
}

void MotionData::input(std::string s){
	reset();
	std::ifsteram in(s);
	if(!in){
		// cannot open file
		return;
	}
	while(!in.eof()){
		BodyDataNode tmp;
		for(int i = 0; i < 15; i++){
			in >> tmp.joints[i];
		}
		data.push_back(tmp);
	}
}

std::ostream& MotionData::dump(std::ostream& os) const{
	for(int i = 0; i < data.size(); i++){
		os << data[i];
	}
	return os;
}

void MotionData::convert(MotionData& model, BodyDataNode& ret){
	int size = data.size();
	ThreeDVector diff = data[0].joints[JOINT_TORSO]-model[0].joints[JOINT_TORSO];
	data.back().convert(model[size-1],diff,ret);
}

void MotionData::convert_all(MotionData& model, MotionData& ret){
	ret.reset();
	BodyDataNode tmp;
	ThreeDVector diff = data[0].joints[JOINT_TORSO]-model[0].joints[JOINT_TORSO];
	for(int i = 0; i < data.size(); i++){
		data[i].convert(model[i],diff,tmp);
		ret.add(tmp);
	}
}

std::ostream& operator<<(std::ostream& os, const MotionData& m){
	m.dump(os);
	return os;
}