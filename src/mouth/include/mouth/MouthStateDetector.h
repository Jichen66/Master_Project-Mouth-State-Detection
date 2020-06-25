#ifndef MOUTHSTATEDETECTOR_H
#define MOUTHSTATEDETECTOR_H

#include <string>

#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>

class MouthStateDetector
{
public:
    MouthStateDetector();

    MouthStateDetector(std::string cascadeName);

    void detectMouths(const cv::Mat &greyImg, cv::Size minSize, cv::Size maxSize);

    std::vector<cv::Rect> getMouthRects();

private:
    cv::CascadeClassifier mouth_cascade;


    std::vector<cv::Rect> mouthRects;
    std::vector<int> reject_levels;
    std::vector<double> level_weights;



};

#endif // MOUTHSTATEDETECTOR_H
