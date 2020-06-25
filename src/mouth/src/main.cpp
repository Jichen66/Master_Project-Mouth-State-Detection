#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Image.h"
#include <opencv2/opencv.hpp>

#include "std_msgs/Int8.h"

#include "sensor_msgs/image_encodings.h"

#include <iostream>
#include <stdio.h>

#include "mouth/FaceDetector.h"
#include "mouth/MouthStateClassifier.h"
#include "mouth/MouthStateDetector.h"
#include "mouth/MouthTracker.h"

cv::CascadeClassifier test;

ros::Publisher pub;

std_msgs::String stateMsg ;


MouthStateClassifier mouthClassifier("/home/jichen/Documents/open.xml",
                                   "/home/jichen/Documents/close.xml");
                                  
FaceDetector faceDetector("/home/jichen/Documents/shape_predictor_68_face_landmarks.dat");
cv::Rect2d bbox(0, 0, 0, 0);
MouthTracker tracker;
bool foundFace = false;
bool isTracking = false;
bool faceHadBeenDetected = false;

void image_rawCallback(const sensor_msgs::Image& img)
{

    cv::Mat imgMat2(cv::Size(img.width,img.height), CV_8UC3, (void*)img.data.data());
//*******************************************************************
    cv::Mat imgMat2_gray;
    cv::cvtColor(imgMat2, imgMat2_gray, cv::COLOR_RGB2BGR);
    cv::Rect mouthBox;
    cv::Rect newMouthBox;
    foundFace = faceDetector.findFaceAndLandmarks(imgMat2,mouthBox);
    if(foundFace)
    {
        isTracking=false;

        bbox = mouthBox;
	faceHadBeenDetected = true;

    }

    else if(!isTracking && faceHadBeenDetected)
    {
        tracker.reinitializeTracekr(imgMat2,bbox);
        isTracking = true;
	faceHadBeenDetected= false;
    }
    bool isTracked;
    if(isTracking)
    {
         isTracked = tracker.updateTrack(imgMat2,bbox);
	 if (!isTracked){ //Tracker has lost target
		isTracking = false;		
	}
    }

    if(foundFace || isTracked)
    {
      //  std::cout<<bbox.x<<" "<<bbox.y<<" "<<bbox.width<<" "<<bbox.height<<std::endl;
        cv::rectangle(imgMat2,bbox,cv::Scalar(255,255,0),2,8,0); //检测不到嘴发0,检测到嘴识别不出状态发1,open发2，close和random发3
        stateMsg.data = "Face Detected, Mouth State Unknown";

        mouthClassifier.classifyMouths(imgMat2_gray,bbox,newMouthBox);

        MouthState currentState = mouthClassifier.getCurrentState();
   //     std::cout<<"bbox "<<bbox<<std::endl;

        if(currentState == MouthState::OPEN)
        {
            cv::rectangle(imgMat2,newMouthBox,cv::Scalar(0,255,0),2,8,0);
            cv::putText(imgMat2, std::string("open"),cv::Point(240,80),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,255,0));
            stateMsg.data = "Mouth Open";
        }
        else if(currentState == MouthState::CLOSE)
        {
            cv::rectangle(imgMat2,newMouthBox,cv::Scalar(0,0,255),2,8,0);
            cv::putText(imgMat2, std::string("closed"),cv::Point(240,80),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255));
            stateMsg.data = "Mouth Closed";
        }

        
    }
    else
    {
        cv::putText(imgMat2, std::string("no mouth or face detected..."),cv::Point(20,20),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,255));
        stateMsg.data = "No face Found";
    }


//*******************************************************************
    cv::imshow("image",imgMat2);


    cv::waitKey(1);
}



int main(int argc, char **argv)
{

    ros::init(argc, argv, "mouthstate");
    ros::NodeHandle nh;
    ros::Rate loop_rate = 5;  // HZ

    pub = nh.advertise<std_msgs::String>("/mouth/state",1000);

    ros::Subscriber imgSub = nh.subscribe("/camera/image", 1, image_rawCallback);

    while (ros::ok())
    {
 //      ROS_INFO("%s", stateMsg.data);

        pub.publish(stateMsg);
        ros::spinOnce();
        loop_rate.sleep();

        char c=(char)cv::waitKey(1);

        if(c==27)
            break;
    }
//    ros::spin();


    return 0;
}

