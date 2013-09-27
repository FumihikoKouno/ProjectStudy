// Sample.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include "BodyDataNode.h"
#include "MotionData.h"
#include <vector>
#include "KinectIO.h"

#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	cv::setUseOptimized( true );
	//
	//add
	std::ofstream outfs("test1.dat");
	int key;
	bool record_model = false;
	bool record_user = false;
	std::vector<MotionData> model, user, goal;
//	BodyDataNode user, goal;

//	BodyDataNode model,mydata,goal;
	KinectIO kio;
	kio.init();

	cv::Mat colorMat;
	cv::Mat depthMat;

	cv::namedWindow( "Color", cv::WINDOW_AUTOSIZE );
//	cv::namedWindow( "Picture", cv::WINDOW_AUTOSIZE );
	//cv::namedWindow( "User", cv::WINDOW_AUTOSIZE );
	//cv::namedWindow( "Skeleton", cv::WINDOW_AUTOSIZE );

	// Table of Colors
	cv::Vec3b color[7];
	color[0]  = cv::Vec3b( 255, 255, 255 );
	color[1]  = cv::Vec3b( 255,   0,   0 );
	color[2]  = cv::Vec3b(   0, 255,   0 );
	color[3]  = cv::Vec3b(   0,   0, 255 );
	color[4]  = cv::Vec3b( 255, 255,   0 );
	color[5]  = cv::Vec3b( 255,   0, 255 );
	color[6]  = cv::Vec3b(   0, 255, 255 );

	std::cout<<"start"<<std::endl;
	while(true){
		if(record_model){
//			outfs<<std::endl;
			// add data of this frame to model
			kio.rec(model,colorMat,depthMat,true);
		}else if(record_user){
			kio.rec(user,colorMat,depthMat,false);
			if(user.size() == model.size()){
				// user data of this frame convert to goal from model[idx]
				BodyDataNode view_node;
				for(unsigned int i = 0; i < user.size(); i++){
					if(user[i].size() > model[i].size()){
						record_user = false;
						break;
					}
					user[i].convert(model[i],view_node);
					for( int position = 0; position < 15; position++ ){
						cv::Point2f registPoint2;
						kio.getUserTracker().convertJointCoordinatesToDepth( (float)view_node.joints[position].getX(), (float)view_node.joints[position].getY(), (float)view_node.joints[position].getZ(), &registPoint2.x, &registPoint2.y ); // Registration Joint Position to Depth
						cv::circle( colorMat, registPoint2, 10, static_cast<cv::Scalar>( color[i] ), -1, CV_AA );
					}
				}
			}else{
				record_user = false;
			}
		}else{
			kio.setVideo(colorMat);
		}

		// Draw Images retrieved from Kinect
		cv::imshow( "Color", colorMat );
//		if(record_user){cv::imshow( "Picture", colorMat1 );}
		//cv::imshow( "User", userMat );
		//cv::imshow( "Skeleton", skeletonMat );
		key=cv::waitKey( 30 );
		// Press the Escape key to Exit
		if( key == VK_ESCAPE ){
			break;
		}else if( key == VK_SPACE){
			if(!record_model){
				record_model = true;
				model.clear();
				for(int i = 0; i < kio.getUserNumber(); i++){
					model.push_back(MotionData());
				}
				std::cout<<"record_model"<<std::endl;
			}else{
				record_model = false;
				std::cout<<"record stop"<<std::endl;
			}
		}else if(key == VK_RETURN){
			if(!record_user){
				record_user = true;
				user.clear();
				for(int i = 0; i < kio.getUserNumber(); i++){
					user.push_back(MotionData());
				}
				std::cout<<"myrec"<<std::endl;
			}else{
				record_user = false;
				std::cout<<"myrec stop"<<std::endl;
			}
		}
	}

	// Shutdown Application
	cv::destroyAllWindows();
	openni::OpenNI::shutdown();
	nite::NiTE::shutdown();
	std::cout<<"finish"<<std::endl;
	return 0;
}

