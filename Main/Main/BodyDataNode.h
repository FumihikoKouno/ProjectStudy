#include "ThreeDVector.h"
#include "Quaternion.h"
#include <vector>
#include <iostream>
class BodyDataNode{
public:
	BodyDataNode();
	BodyDataNode(double x,double y, double z);
	BodyDataNode(ThreeDVector tv, Quaternion q);
	~BodyDataNode();

	BodyDataNode convert(BodyDataNode& bdnode);

	ThreeDVector joints[15];
/*
	ThreeDVector HEAD;
	ThreeDVector NECK;

	ThreeDVector LEFT_SHOULDER;
	ThreeDVector RIGHT_SHOULDER;
	ThreeDVector LEFT_ELBOW;
	ThreeDVector RIGHT_ELBOW;
	ThreeDVector LEFT_HAND;
	ThreeDVector RIGHT_HAND;

	ThreeDVector TORSO;

	ThreeDVector LEFT_HIP;
	ThreeDVector RIGHT_HIP;
	ThreeDVector LEFT_KNEE;
	ThreeDVector RIGHT_KNEE;
	ThreeDVector LEFT_FOOT;
	ThreeDVector RIGHT_FOOT;
	*/

	void add_children(double x,double y, double z);

private:
	ThreeDVector tv;
	std::vector<BodyDataNode*> children;
	Quaternion q;
};


std::ostream& operator<<(std::ostream& os, const BodyDataNode& b);