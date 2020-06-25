#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <string>
#include <opencv2/opencv.hpp>
//#include "opencv2/highgui.hpp"
#include <opencv2/imgproc.hpp>


class FaceDetector
{
public:

    //! Load dlib face detection and pose estimation models.

    FaceDetector(std::string sPredictor);

    bool findFaceAndLandmarks(const cv::Mat & inputImg, cv::Rect &mouthBox);


private:
    dlib::shape_predictor pose_model;
    dlib::frontal_face_detector detector;
};

#endif // FACEDETECTOR_H
