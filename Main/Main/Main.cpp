// Sample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp> 
#include "BodyDataNode.h"
#include "MotionData.h"
#include <vector>
#include "KinectIO.h"
#include <string>

#include <fstream>

//int main(int argc, char* argv[])//
int _tmain(int argc, _TCHAR* argv[])
{
	cv::setUseOptimized( true );
	//
	//add
	std::wstring indata,outdata;
	std::string message_str="STOP";
	std::ofstream outfs;
	

	int key,id;
	int message=1;
	bool record_model = false;
	bool record_user = false;
	std::vector<MotionData> model, user, goal;
	MotionData data;
//	BodyDataNode user, goal;

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
//			outfs<<std::endl;
			// add data of this frame to model
			kio.rec(model,colorMat,depthMat,true);
			
		}else if(record_user){
			kio.rec(user,colorMat,depthMat,false);
			// user data of this frame convert to goal from model[idx]
			if(!user.empty()){
				BodyDataNode view_node;
				for(unsigned int i = 0; i < user.size(); i++){
					if(user[i].size()!=0){
						user[i].convert(model[0],view_node);
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
		if( key == VK_ESCAPE ){
			break;
		}else if( key == VK_SPACE){
			if(record_user){
				MessageBox(NULL, L"ユーザモード中のため、モデルデータの録画はできません。", L"error",0);
				continue;
			}
			if(!record_model){
				record_model = true;
				message_str = "REC";
				MessageBox(NULL, L"モデルの録画を開始します。", L"start",0);
				model.clear();
				for(int i = 0; i < kio.getUserNumber(); i++){
					model.push_back(MotionData());
				}
				//std::cout<<"record_model"<<std::endl;
			}else{
				record_model = false;
				message_str = "STOP";
				//std::cout<<"record stop"<<std::endl;
				id=MessageBox(NULL, L"保存しますか？", L"データの保存", MB_YESNO);
				if(id == IDYES){
					//DialogBox(hInstance, MAKEINTRESOURCE(IDD_MYDIALOG),NULL, DlgProc);
					if(model.empty())MessageBox(NULL, L"モデルは取れませんでした。", L"no human",0) ;
					else {
						outdata=window.filesave();
						outfs.open(outdata);
						outfs<<model[0];
						outfs.close();
						std::wcout<<L"m:"<<outdata<<std::endl;
						//二人目以降のデータを出力させる。*_2.datみたいなファイルで出力させておく
						//ToDo::出てきた人の人数を把握する関数、wstringの末尾に数字をくっつけられる関数の把握
						//std::cout<<model.size()<<std::endl;
					}
				}
			}
		}else if(key == VK_RETURN){
			if(record_model){
				MessageBox(NULL, L"モデルモード中のため、ユーザデータの録画はできません。", L"error",0);
				continue;
			}
			if(!record_user){
				if(model.empty()){
					record_user = false;
					message_str = "STOP";
					MessageBox(NULL, L"モデルデータが入力されていません", L"model error",0);
					continue;
				}
				record_user = true;
				message_str = "REC(U)";
				MessageBox(NULL, L"ユーザの録画を開始します。", L"start",0);
				user.clear();
				for(int i = 0; i < kio.getUserNumber(); i++){
					user.push_back(MotionData());
				}
				std::cout<<"myrec"<<std::endl;
			}else{
				record_user = false;
				message_str = "STOP";
				std::cout<<"myrec stop"<<std::endl;
				id=MessageBox(NULL, L"保存しますか？", L"ユーザデータの保存", MB_YESNO);
				if(id == IDYES){
					//DialogBox(hInstance, MAKEINTRESOURCE(IDD_MYDIALOG),NULL, DlgProc);
					if(user.empty())MessageBox(NULL, L"ユーザモデルは取れませんでした。", L"no human",0) ;
					else {
						outdata=window.filesave();
						outfs.open(outdata);
						outfs<<user[0];
						outfs.close();
						std::wcout<<L"u:"<<outdata<<std::endl;
						//二人目以降のデータを出力させる。*_2.datみたいなファイルで出力させておく
						//ToDo::出てきた人の人数を把握する関数、wstringの末尾に数字をくっつけられる関数の把握
						//std::cout<<user.size()<<std::endl;
					}
				}
			}
		}else if(key == 'A'){
			message=1-message;
		}else if(key == 'h'){
			
			MessageBox(NULL, L"スペースキーで録画開始と終了。\n 「A」で右上の文字の消去、生成", L"ヘルプ", MB_OK);
		}else if(key == 'o'){
			indata=window.fileopen();
			std::wcout<<L"open:"<<indata<<std::endl;
			
			data.input(indata,model);
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

