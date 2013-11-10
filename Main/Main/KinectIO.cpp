#include "stdafx.h"
#include "KinectIO.h"


KinectIO::KinectIO(void)
{
}

KinectIO::~KinectIO(void)
{
	colorStream.stop();
	depthStream.stop();
	userTracker.destroy();
	device.close();
	colorStream.destroy();
	depthStream.destroy();
}

void KinectIO::setVideo(cv::Mat& mat){	
		// Table of Colors
	cv::Vec3b color[7];
	color[0]  = cv::Vec3b( 255, 255, 255 );
	color[1]  = cv::Vec3b( 255,   0,   0 );
	color[2]  = cv::Vec3b(   0, 255,   0 );
	color[3]  = cv::Vec3b(   0,   0, 255 );
	color[4]  = cv::Vec3b( 255, 255,   0 );
	color[5]  = cv::Vec3b( 255,   0, 255 );
	color[6]  = cv::Vec3b(   0, 255, 255 );

	openni::VideoFrameRef colorFrame;
	colorStream.readFrame( &colorFrame ); // Retrieve a Frame from Stream
	if( colorFrame.isValid() ){
		mat = cv::Mat( colorStream.getVideoMode().getResolutionY(), colorStream.getVideoMode().getResolutionX(), CV_8UC3, reinterpret_cast<uchar*>( const_cast<void*>( colorFrame.getData() ) ) ); // Retrieve a Data from Frame 
		cv::cvtColor( mat, mat, CV_RGB2BGR ); // Change the order of the pixel RGB to BGR
	}
	//////
		// Retrieve User Frame from UserTracker
	nite::UserTrackerFrameRef userFrame;
	userTracker.readFrame( &userFrame ); // Retrive a Frame form Tracker
	const nite::UserId* pUserId = userFrame.getUserMap().getPixels(); // Retrive UserId from Frame
	int width = userFrame.getUserMap().getWidth();
	int height = userFrame.getUserMap().getHeight();
		// Retrieve Skeleton Frame from UserTracker
//		skeletonMat = cv::Mat( height, width, CV_8UC3, cv::Scalar( 255, 255, 255 ) );
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
					tmp_body_data.joints[position] = ThreeDVector(point.x,point.y,point.z);
					cv::Point2f registPoint;
					userTracker.convertJointCoordinatesToDepth( point.x, point.y, point.z, &registPoint.x, &registPoint.y ); // Registration Joint Position to Depth
					cv::circle( mat, registPoint, 10, static_cast<cv::Scalar>( color[count + 1] ), -1, CV_AA );
				}
				
			}
		}
	}

	//////
}

void KinectIO::rec(std::vector<MotionData>& data, cv::Mat& colorMat, cv::Mat& depthMat, bool show){

	// Table of Colors
	cv::Vec3b color[7];
	color[0]  = cv::Vec3b( 255, 255, 255 );
	color[1]  = cv::Vec3b( 255,   0,   0 );
	color[2]  = cv::Vec3b(   0, 255,   0 );
	color[3]  = cv::Vec3b(   0,   0, 255 );
	color[4]  = cv::Vec3b( 255, 255,   0 );
	color[5]  = cv::Vec3b( 255,   0, 255 );
	color[6]  = cv::Vec3b(   0, 255, 255 );

	// Retrieve Frame from Color Stream (8bit 3channel)
	openni::VideoFrameRef colorFrame;
	colorStream.readFrame( &colorFrame ); // Retrieve a Frame from Stream
	if( colorFrame.isValid() ){
		colorMat = cv::Mat( colorStream.getVideoMode().getResolutionY(), colorStream.getVideoMode().getResolutionX(), CV_8UC3, reinterpret_cast<uchar*>( const_cast<void*>( colorFrame.getData() ) ) ); // Retrieve a Data from Frame 
		cv::cvtColor( colorMat, colorMat, CV_RGB2BGR ); // Change the order of the pixel RGB to BGR
//			if(record_user)colorMat1=colorMat.clone();
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
//		userMat = cv::Mat( height, width, CV_8UC3, cv::Scalar( 255, 255, 255 ) );
	if( userFrame.isValid() ){
		for(int y = 0; y < height; y++ ){
			for(int x = 0; x < width; x++ ){
//					colorMat.at<cv::Vec3b>( y, x ) = color[*pUserId];
				pUserId++;
			}
		}
	}

	// Retrieve Skeleton Frame from UserTracker
//		skeletonMat = cv::Mat( height, width, CV_8UC3, cv::Scalar( 255, 255, 255 ) );
	const nite::Array<nite::UserData>& users = userFrame.getUsers(); // Retrieve User from User Frame
//	if(users.getSize()!=data.size())return;
	/**
		* TODO : If we assume only a model, this for loop is unnecessary
		*/
	std::cout << "before for : " << users.getSize() << std::endl;
	for( int count = 0; count < users.getSize(); count++ ){
		std::cout << "in for : " << count << "," << users.getSize() << std::endl;
		// Start Skeleton Tracking a new User
		if( users[count].isNew() ){
			userTracker.startSkeletonTracking( users[count].getId() );
			std::cout << "test" << std::endl;
		}
		// Retrieve Skeleton from Tracking User ( who is Not Lost and Visible User )
		else if( !users[count].isLost() && users[count].isVisible() ){
			const nite::Skeleton& skeleton = users[count].getSkeleton(); // Retrieve Skeleton form User
			if( skeleton.getState() == nite::SkeletonState::SKELETON_TRACKED ){
				BodyDataNode tmp_body_data;
				for( int position = 0; position < 15; position++ ){//20?
					const nite::SkeletonJoint& joint = skeleton.getJoint((nite::JointType)position); // Retrieve Joint from Skeleton ( Total 14 joint )
					const nite::Point3f& point = joint.getPosition(); // Retrieve three-dimensional position of the Joint
					tmp_body_data.joints[position] = ThreeDVector(point.x,point.y,point.z);
					cv::Point2f registPoint;
					userTracker.convertJointCoordinatesToDepth( point.x, point.y, point.z, &registPoint.x, &registPoint.y ); // Registration Joint Position to Depth
					if(show) cv::circle( colorMat, registPoint, 10, static_cast<cv::Scalar>( color[count + 1] ), -1, CV_AA );
				}
				std::cout << "tmp : " << tmp_body_data << std::endl;
				data[count].add(tmp_body_data);
				std::cout << "data : " << data[count].back() << std::endl;
			}
		}
	}

}

nite::UserTracker& KinectIO::getUserTracker(){ return userTracker; }

void KinectIO::update(){
	nite::UserTrackerFrameRef userFrame;
	userTracker.readFrame( &userFrame ); // Retrive a Frame form Tracker
	const nite::Array<nite::UserData>& users = userFrame.getUsers();
	for(int count = 0; count < users.getSize(); count++){
		if( users[count].isNew() ){
			std::cout << "test" << std::endl;
			userTracker.startSkeletonTracking( users[count].getId() );
		}
	}
}

int KinectIO::getUserNumber(){
	nite::UserTrackerFrameRef userFrame;
	userTracker.readFrame( &userFrame ); // Retrive a Frame form Tracker
	const nite::Array<nite::UserData>& users = userFrame.getUsers();
	for(int count = 0; count < users.getSize(); count++){
		if( users[count].isNew() ){
			std::cout << "test" << std::endl;
			userTracker.startSkeletonTracking( users[count].getId() );
		}
	}
	return users.getSize();
}

bool KinectIO::init(){
	// Initialize OpenNI2
	openni::OpenNI::initialize();

	// Initialize NITE2 
	nite::NiTE::initialize();
	
	// Device Open
	statusOpenNI = openni::STATUS_OK;
	statusOpenNI = device.open( openni::ANY_DEVICE );
	if( statusOpenNI != openni::STATUS_OK ){
		std::cerr << "Error : openni::Device::open" << std::endl;
		return false;
	}

	// Color Stream Create and Open
	colorStream.create( device, openni::SENSOR_COLOR );
	statusOpenNI = colorStream.start();
	if( statusOpenNI != openni::STATUS_OK ){
		std::cerr << "Error : openni::VideoStream::start( COLOR )" << std::endl;
		return false;
	}

	// Depth Stream Create and Open
	depthStream.create( device, openni::SENSOR_DEPTH );
	statusOpenNI = depthStream.start();
	if( statusOpenNI != openni::STATUS_OK ){
		std::cerr << "Error : openni::VideoStream::start( DEPTH )" << std::endl;
		return false;
	}

	// User Tracker Create and Open
	statusNITE = nite::STATUS_OK;
	statusNITE = userTracker.create( &device );
	if( statusNITE != nite::STATUS_OK ){
		std::cerr << "Error : nite::UserTracker::create" << std::endl;
		return false;
	}

	// Setting enable Synchronize
	device.setDepthColorSyncEnabled(true);

	// Set Registration Mode Depth to Color
	// But Kinect doesn't support this Function!
	device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );
	return true;
}
