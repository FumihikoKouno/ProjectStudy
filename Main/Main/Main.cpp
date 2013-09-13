// Sample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <OpenNI.h> // OpenNI2 Header file
#include <NiTE.h> // NITE2 Header file
#include <opencv2/opencv.hpp>
#include "BodyDataNode.h"
#include "MotionData.h"
#include <vector>

#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
	cv::setUseOptimized( true );

	//add
	std::ofstream outfs("test1.dat");
	int key;
	int idx;
	bool record_model = false;
	bool record_user = false;
	MotionData model, user, goal;
//	BodyDataNode user, goal;

//	BodyDataNode model,mydata,goal;

	// Initialize OpenNI2
	openni::OpenNI::initialize();

	// Initialize NITE2 
	nite::NiTE::initialize();
	
	// Device Open
	openni::Status statusOpenNI = openni::STATUS_OK;
	openni::Device device;
	statusOpenNI = device.open( openni::ANY_DEVICE );
	if( statusOpenNI != openni::STATUS_OK ){
		std::cerr << "Error : openni::Device::open" << std::endl;
		return -1;
	}

	// Color Stream Create and Open
	openni::VideoStream colorStream;
	colorStream.create( device, openni::SENSOR_COLOR );
	statusOpenNI = colorStream.start();
	if( statusOpenNI != openni::STATUS_OK ){
		std::cerr << "Error : openni::VideoStream::start( COLOR )" << std::endl;
		return -1;
	}

	// Depth Stream Create and Open
	openni::VideoStream depthStream;
	depthStream.create( device, openni::SENSOR_DEPTH );
	statusOpenNI = depthStream.start();
	if( statusOpenNI != openni::STATUS_OK ){
		std::cerr << "Error : openni::VideoStream::start( DEPTH )" << std::endl;
		return -1;
	}

	// User Tracker Create and Open
	nite::Status statusNITE = nite::STATUS_OK;
	nite::UserTracker userTracker;
	statusNITE = userTracker.create( &device );
	if( statusNITE != nite::STATUS_OK ){
		std::cerr << "Error : nite::UserTracker::create" << std::endl;
		return -1;
	}

	// Setting enable Synchronize
	device.setDepthColorSyncEnabled(true);

	// Set Registration Mode Depth to Color
	// But Kinect doesn't support this Function!
	device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );

	cv::Mat colorMat;
	cv::Mat colorMat1;
	cv::Mat depthMat;
	cv::Mat userMat;
	cv::Mat skeletonMat;

	cv::namedWindow( "Color", cv::WINDOW_AUTOSIZE );
	cv::namedWindow( "Picture", cv::WINDOW_AUTOSIZE );
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
		// Retrieve Frame from Color Stream (8bit 3channel)
		openni::VideoFrameRef colorFrame;
		colorStream.readFrame( &colorFrame ); // Retrieve a Frame from Stream
		if( colorFrame.isValid() ){
			colorMat = cv::Mat( colorStream.getVideoMode().getResolutionY(), colorStream.getVideoMode().getResolutionX(), CV_8UC3, reinterpret_cast<uchar*>( const_cast<void*>( colorFrame.getData() ) ) ); // Retrieve a Data from Frame 
			cv::cvtColor( colorMat, colorMat, CV_RGB2BGR ); // Change the order of the pixel RGB to BGR
			if(record_user)colorMat1=colorMat.clone();
		}

		// Retrieve Frame from Depth Stream (16bit 1channel)
		openni::VideoFrameRef depthFrame;
		depthStream.readFrame( &depthFrame ); // Retrieve a Frame from Stream
		if( depthFrame.isValid() ){
			depthMat = cv::Mat( depthStream.getVideoMode().getResolutionY(), depthStream.getVideoMode().getResolutionX(), CV_16UC1, reinterpret_cast<ushort*>( const_cast<void*>( depthFrame.getData() ) ) ); // Retrieve a Data from Frame
			depthMat.convertTo( depthMat, CV_8UC1, -255.0f / 10000, 255.0 ); // Convert the pixel 0~10000 to 0~255
		}

		// Retrieve User Frame from UserTracker
		nite::UserTrackerFrameRef userFrame;
		userTracker.readFrame( &userFrame ); // Retrive a Frame form Tracker
		const nite::UserId* pUserId = userFrame.getUserMap().getPixels(); // Retrive UserId from Frame
		int width = userFrame.getUserMap().getWidth();
		int height = userFrame.getUserMap().getHeight();
		userMat = cv::Mat( height, width, CV_8UC3, cv::Scalar( 255, 255, 255 ) );
		if( userFrame.isValid() ){
			for(int y = 0; y < height; y++ ){
				for(int x = 0; x < width; x++ ){
					userMat.at<cv::Vec3b>( y, x ) = color[*pUserId];
					pUserId++;
				}
			}
		}

		// Retrieve Skeleton Frame from UserTracker
		skeletonMat = cv::Mat( height, width, CV_8UC3, cv::Scalar( 255, 255, 255 ) );
		const nite::Array<nite::UserData>& users = userFrame.getUsers(); // Retrieve User from User Frame
	  /**
		 * TODO : If we assume only a model, this for loop is unnecessary
		 */
		for( int count = 0; count < users.getSize(); count++ ){
			// Start Skeleton Tracking a new User
			if( users[count].isNew() ){
				userTracker.startSkeletonTracking( users[count].getId() );
			}
			// Retrieve Skeleton from Tracking User ( who is Not Lost and Visible User )
			else if( !users[count].isLost() && users[count].isVisible() ){
				const nite::Skeleton& skeleton = users[count].getSkeleton(); // Retrieve Skeleton form User
				if( skeleton.getState() == nite::SkeletonState::SKELETON_TRACKED ){
					BodyDataNode tmp_body_data;
					for( int position = 0; position < 15; position++ ){//20?
						const nite::SkeletonJoint& joint = skeleton.getJoint((nite::JointType)position); // Retrieve Joint from Skeleton ( Total 14 joint )
						const nite::Point3f& point = joint.getPosition(); // Retrieve three-dimensional position of the Joint
						if(record_model){
							outfs<<point.x
							<<" "<<point.y
							<<" "<<point.z<<std::endl;
							std::cout<<point.x
							<<" "<<point.y
							<<" "<<point.z<<std::endl;
						}
						tmp_body_data[position] = ThreeDVector(point.x,point.y,point.z);
						/*
						if(record_model){
							model.joints[position] = ThreeDVector(point.x,point.y,point.z);
						}else if(record_user){
							mydata.joints[position] = ThreeDVector(point.x,point.y,point.z);
						}
						*/
						cv::Point2f registPoint;
						userTracker.convertJointCoordinatesToDepth( point.x, point.y, point.z, &registPoint.x, &registPoint.y ); // Registration Joint Position to Depth
						if(record_model) cv::circle( colorMat, registPoint, 10, static_cast<cv::Scalar>( color[count + 1] ), -1, CV_AA );
					}
					if(record_model){
						outfs<<std::endl;
						// add data of this frame to model
						model.add(tmp_body_data);
						record_model = false;
					}else if(record_user){
						user.add(tmp_body_data);
						if(idx < model.size()){
							// user data of this frame convert to goal from model[idx]
							BodyDataNode view_node;
							user.convert(model,view_node);
							for( int position = 0; position < 15; position++ ){
								cv::Point2f registPoint2;
								int z_diff = (view_node.joints[position].getZ()-user.joints[position].getZ());
								if(z_diff < -255) z_diff = -255;
								if(z_diff > 255) z_diff = 255;
								int z_diff_abs;
								if(z_diff < 0) z_diff_abs = -z_diff;
								else z_diff_abs = z_diff;
								userTracker.convertJointCoordinatesToDepth( view_node.joints[position].getX(), view_node.joints[position].getY(), view_node.joints[position].getZ(), &registPoint2.x, &registPoint2.y ); // Registration Joint Position to Depth
								cv::circle( colorMat1, registPoint2, 16+z_diff/16, static_cast<cv::Scalar>( cv::Vec3b(z_diff_abs,0,255-z_diff_abs) ), -1, CV_AA );
							}
						}else{
							record_user = false;
							idx = 0;
						}
					}
				}
			}
		}

		// Draw Images retrieved from Kinect
		cv::imshow( "Color", colorMat );
		if(record_user){cv::imshow( "Picture", colorMat1 );}
		//cv::imshow( "User", userMat );
		//cv::imshow( "Skeleton", skeletonMat );
	    if(record_model){ 
			std::cout<<"There is no person."<<std::endl;
			rec_model = false;
		}
		key=cv::waitKey( 30 );
		// Press the Escape key to Exit
		if( key == VK_ESCAPE ){
			break;
		}else if( key == VK_SPACE){
			if(!record_model){
				record_model = true;
				model.reset();
				std::cout<<"record_model"<<std::endl;
			}else{
				record_model = false;
				std::cout<<"myrec stop"<<std::endl;
			}
		}else if(key == VK_RETURN){
			if(!record_user){
				record_user = true;
				idx = 0;
				user.reset();
				std::cout<<"myrec"<<std::endl;
			}else{
				record_user = false;
				std::cout<<"myrec stop"<<std::endl;
			}
		}
	}

	// Shutdown Application
	cv::destroyAllWindows();
	colorStream.stop();
	depthStream.stop();
	colorStream.destroy();
	depthStream.destroy();
	userTracker.destroy();
	device.close();
	openni::OpenNI::shutdown();
	nite::NiTE::shutdown();
	std::cout<<"finish"<<std::endl;
	return 0;
}

