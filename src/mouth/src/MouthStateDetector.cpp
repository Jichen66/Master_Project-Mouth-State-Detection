#include "mouth/MouthStateDetector.h"
#include "ros/ros.h"

MouthStateDetector::MouthStateDetector()
{
}

MouthStateDetector::MouthStateDetector(std::string cascadeName)
{
    mouth_cascade.load(cascadeName);
}

void MouthStateDetector::detectMouths(const cv::Mat& greyImg,cv::Size minSize, cv::Size maxSize)
{
    mouth_cascade.detectMultiScale(greyImg,mouthRects,reject_levels,level_weights,
                                   1.1, 3, 0,minSize,maxSize,true);

    ROS_DEBUG_STREAM("------------------------------");
    ROS_DEBUG_STREAM("size of mouths "<<mouthRects.size());

}

std::vector<cv::Rect> MouthStateDetector::getMouthRects()
{
    return mouthRects;
}
