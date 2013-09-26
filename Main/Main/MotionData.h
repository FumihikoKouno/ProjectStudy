#ifndef _MOTION_DATA_
#define _MOTION_DATA_

#include <string>
#include <vector>
#include <iostream>
#include "BodyDataNode.h"

class MotionData{
public:
	MotionData();
	~MotionData();
	
	std::ostream& dump(std::ostream& os) const;
	void output(std::string s);
	void input(std::string s);
	void reset();
	void add(BodyDataNode& bd);
	int size();
	BodyDataNode& operator[](int i);
	BodyDataNode& back();
	void convert(MotionData& model, BodyDataNode& ret);
	void convert_all(MotionData& model, MotionData& ret);
	
private:
	std::vector<BodyDataNode> data;
};

std::ostream& operator<<(std::ostream& os, const MotionData& m);

#endif