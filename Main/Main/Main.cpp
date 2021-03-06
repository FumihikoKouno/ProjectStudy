// Sample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp> 
#include "BodyDataNode.h"
#include "MotionData.h"
#include <vector>
#include "KinectIO.h"
#include <string>

#include <sstream>
#include <fstream>

//int main(int argc, char* argv[])//
int _tmain(int argc, _TCHAR* argv[])
{
	cv::setUseOptimized( true );
	//
	//add
	std::string indata,outdata;
	std::string message_str="STOP";
	std::ofstream outfs;
	

	int key,id;
	int message=1;
	int countdown=0,countfirst=150;
	bool record_model = false;
	bool record_user = false;
	std::vector<MotionData> model, user, preuser, goal;
	MotionData data;
//	BodyDataNode user, goal;
	//std::cout<<"start"<<std::endl;
//	BodyDataNode model,mydata,goal;
	KinectIO kio;
	kio.init();

	cv::Mat colorMat;
	cv::Mat depthMat;

	mywindow window;

	cv::namedWindow( "Main", cv::WINDOW_AUTOSIZE );
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

	//std::cout<<"start"<<std::endl;
	while(true){
		if(record_model){
			if(!countdown){
				// add data of this frame to model
				kio.rec(model,colorMat,depthMat,true);
				message_str = "REC";
			}else{		
				kio.setVideo(colorMat);
				std::stringstream ss;
				ss << 1+countdown/30;
				message_str = ss.str();
				countdown--;
			}
			
		}else if(record_user&&!countdown){
			kio.rec(user,colorMat,depthMat,false);
			message_str = "REC(U)";
			// user data of this frame convert to goal from model[idx]
			if(!user.empty()){
				while(user.size()>goal.size())goal.push_back(MotionData());
				BodyDataNode view_node;
				for(unsigned int i = 0; i < user.size(); i++){
					if(user[i].size()!=0){
						if(model[0].size()>user[i].size()){
							user[i].convert(model[0],view_node);//model[0]の動きをまねるようにする
							for( int position = 0; position < 15; position++ ){
								cv::Point2f registPoint2;
								kio.getUserTracker().convertJointCoordinatesToDepth( (float)view_node.joints[position].getX(), (float)view_node.joints[position].getY(), (float)view_node.joints[position].getZ(), &registPoint2.x, &registPoint2.y ); // Registration Joint Position to Depth
								cv::circle( colorMat, registPoint2, 10, static_cast<cv::Scalar>( color[i] ), -1, CV_AA );
							}
							goal[i].add(view_node);
						}else{
							for( int position = 0; position < 15; position++ ){
								cv::Point2f registPoint2;
								kio.getUserTracker().convertJointCoordinatesToDepth( (float)user[i].back().joints[position].getX(), (float)user[i].back().joints[position].getY(), (float)user[i].back().joints[position].getZ(), &registPoint2.x, &registPoint2.y ); 
								cv::circle( colorMat, registPoint2, 10, static_cast<cv::Scalar>( color[i] ), -1, CV_AA );
							}
							
						}
					}
				}
			}

		}else if(record_user){
			kio.rec(preuser,colorMat,depthMat,false);
			std::stringstream ss;
			ss << 1+countdown/30;
			message_str = ss.str();
			countdown--;		
			// user data of this frame convert to goal from model[idx]
			if(!preuser.empty()){
				while(preuser.size()>goal.size()){
					user.push_back(MotionData());
					goal.push_back(MotionData());
				}
				BodyDataNode view_node;
				for(unsigned int i = 0; i < preuser.size(); i++){
					if(preuser[i].size()!=0){
							preuser[i].preconvert(model[0],view_node);//model[0]の動きをまねるようにする
							for( int position = 0; position < 15; position++ ){
								cv::Point2f registPoint2;
								kio.getUserTracker().convertJointCoordinatesToDepth( (float)view_node.joints[position].getX(), (float)view_node.joints[position].getY(), (float)view_node.joints[position].getZ(), &registPoint2.x, &registPoint2.y ); // Registration Joint Position to Depth
								cv::circle( colorMat, registPoint2, 10, static_cast<cv::Scalar>( color[i] ), -1, CV_AA );
							}

					}
				}
			}

		}else{
			kio.setVideo(colorMat);
		}

		// Draw Images retrieved from Kinect
		
//		if(record_user){cv::imshow( "Picture", colorMat1 );}
		//cv::imshow( "User", userMat );
		//cv::imshow( "Skeleton", skeletonMat );
		key=cv::waitKey( 30 );
		// Press the Escape key to Exit
		if( key == VK_ESCAPE || key == 'q' ){
			break;
		}else if( key == VK_SPACE){
			if(record_user){
				MessageBox(NULL, "ユーザモード中のため、モデルデータの録画はできません。", "error",0);
				continue;
			}
			if(!record_model){
				record_model = true;
				message_str = "REC";
				countdown=countfirst;
				MessageBox(NULL, "モデルの録画を開始します。", "start",0);
				model.clear();
				for(int i = 0; i < kio.getUserNumber(); i++){
					model.push_back(MotionData());
				}
				//std::cout<<"record_mode"<<std::endl;
			}else{
				record_model = false;
				message_str = "STOP";
				//std::cout<<"record stop"<<std::endl;
				id=MessageBox(NULL, "保存しますか？", "データの保存", MB_YESNO);
				if(id == IDYES){
					//DialogBox(hInstance, MAKEINTRESOURCE(IDD_MYDIALOG),NULL, DlgProc);
					if(model.empty())MessageBox(NULL, "モデルは取れませんでした。", "no human",0) ;
					else {
						outdata=window.filesave();
						outfs.open(outdata);
						outfs<<model[0];
						outfs.close();
						std::cout<<"m:"<<outdata<<std::endl;
						//二人目以降のデータを出力させる。*_2.datみたいなファイルで出力させておく
						//ToDo::出てきた人の人数を把握する関数、stringの末尾に数字をくっつけられる関数の把握
						//std::cout<<model.size()<<std::endl;
						if(model.size()>1){
							id=MessageBox(NULL, "二人目以降のモデルデータも保存しますか？", "データの保存", MB_YESNO);
							if(id == IDYES){
								for(int i=1;i<model.size();i++){
									outdata=window.filesave();
									outfs.open(outdata);
									outfs<<model[i];
									outfs.close();
								}
							}
						}
					}
				}
			}
		}else if(key == VK_RETURN){
			if(record_model){
				MessageBox(NULL, "モデルモード中のため、ユーザデータの録画はできません。", "error",0);
				continue;
			}
			if(!record_user){
				if(model.empty()){
					record_user = false;
					message_str = "STOP";
					MessageBox(NULL, "モデルデータが入力されていません", "model error",0);
					continue;
				}
				record_user = true;
				countdown=countfirst;
				message_str = "REC(U)";
				MessageBox(NULL, "ユーザの録画を開始します。", "start",0);
				user.clear();
				goal.clear();
				preuser.clear();
				for(int i = 0; i < kio.getUserNumber(); i++){
					user.push_back(MotionData());
				}
				//std::cout<<"myrec"<<std::endl;
			}else{
				record_user = false;
				message_str = "STOP";
				//std::cout<<"myrec stop"<<std::endl;
				id=MessageBox(NULL, "今の動きを保存しますか？", "ユーザデータの保存", MB_YESNO);
				if(id == IDYES){
					//DialogBox(hInstance, MAKEINTRESOURCE(IDD_MYDIALOG),NULL, DlgProc);
					if(user.empty()){
						MessageBox(NULL, "ユーザモデルは取れませんでした。", "no human",0) ;
						continue;
					}
					else {
						outdata=window.filesave();
						outfs.open(outdata);
						outfs<<user[0];
						outfs.close();
						std::cout<<"u:"<<outdata<<std::endl;
						if(user.size()>1){
							id=MessageBox(NULL, "二人目以降のモデルデータも保存しますか？", "データの保存", MB_YESNO);
							if(id == IDYES){
								for(int i=1;i<user.size();i++){
									outdata=window.filesave();
									outfs.open(outdata);
									outfs<<user[i];
									outfs.close();
								}
							}
						}
					}
				}
				id=MessageBox(NULL, "あなたの体に合ったモデルデータの動きを保存しますか？", "ユーザに合ったモデルデータの保存", MB_YESNO);
				if(id == IDYES){
					//DialogBox(hInstance, MAKEINTRESOURCE(IDD_MYDIALOG),NULL, DlgProc);
					if(user.empty())MessageBox(NULL, "ユーザモデルは取れませんでした。", "no human",0) ;
					else {
						outdata=window.filesave();
						outfs.open(outdata);
						outfs<<goal[0];
						outfs.close();
						std::cout<<"c:"<<outdata<<std::endl;
						if(goal.size()>1){
							id=MessageBox(NULL, "二人目以降のモデルデータも保存しますか？", "データの保存", MB_YESNO);
							if(id= IDYES){
								for(int i=1;i<goal.size();i++){
									outdata=window.filesave();
									outfs.open(outdata);
									outfs<<goal[i];
									outfs.close();
								}
							}
						}
					}
				}
			}
		}else if(key == 'A'){
			message=1-message;
		}else if(key == 'h'){
			
			MessageBox(NULL, "「o」でモデルデータのファイルを開く\n「space」でモデルデータの録画開始と終了\n「Enter」でユーザデータの録画開始と終了\n「shift+a」で右上の文字の消去、生成\n「Esc」でアプリケーション終了", "ヘルプ", MB_OK);
		}else if(key == 'o'){
			indata=window.fileopen();
			//std::cout<<"open:"<<indata<<std::endl;
			if(indata!=".mywindow.dat"){
				std::cout<<"m:"<<indata<<std::endl;
				data.input(indata,model);
			}
		}else if(key == 'c'){
			countfirst=window.getint(countfirst);
		}
		if(message)
				cv::putText(colorMat, message_str, cvPoint(colorMat.cols*4/5,colorMat.rows/8), CV_FONT_HERSHEY_SIMPLEX, 1.0f, CV_RGB(0,255,0),2,CV_AA);
		cv::imshow( "Main", colorMat );
	}

	// Shutdown Application
	cv::destroyAllWindows();
	openni::OpenNI::shutdown();
	nite::NiTE::shutdown();
	//std::cout<<"finish"<<std::endl;
	return 0;
}

