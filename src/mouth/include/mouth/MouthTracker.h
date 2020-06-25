#ifndef MOUTHTRACKER_H
#define MOUTHTRACKER_H

#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
//#include <opencv2/opencv.hpp>


class MouthTracker
{
public:
    MouthTracker();

    void reinitializeTracekr(const cv::Mat &frame, cv::Rect2d bbox);

    bool updateTrack(const cv::Mat& frame, cv::Rect2d &bbox);


private:
    cv::Ptr<cv::Tracker> tracker;
};

#endif // MOUTHTRACKER_H
