#include "mouth/FaceDetector.h"

FaceDetector::FaceDetector(std::string sPredictor)
{
    // Load face detection and pose estimation models.
    detector = dlib::get_frontal_face_detector();

    dlib::deserialize(sPredictor) >> pose_model;

    //s predictor should be ended with "shape_predictor_68_face_landmarks.dat"
}

bool FaceDetector::findFaceAndLandmarks(const cv::Mat &inputImg,  cv::Rect& mouthBox)
{
    dlib::cv_image<dlib::bgr_pixel> cimg(inputImg);
    // Detect faces
    std::vector<dlib::rectangle> faces = detector(cimg);
    dlib::full_object_detection faceShape;

    if(faces.size()!=1)
        return false;
    else
    {
        faceShape = pose_model(cimg,faces[0]);
    }

    DLIB_CASSERT
            (
                faceShape.num_parts() == 68,
                "\n\t Invalid inputs were given to this function. "
                << "\n\t d.num_parts():  " << faceShape.num_parts()
                );

    std::vector<cv::Point> mouth;
    for (int i = 48; i <= 67; ++i)
    {
        cv::Point pt = cv::Point(faceShape.part(i).x(), faceShape.part(i).y());
        mouth.push_back(pt);
    }
    mouthBox = cv::boundingRect(mouth);

    return true;
}






