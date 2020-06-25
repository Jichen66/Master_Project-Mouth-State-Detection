#ifndef MOUTHSTATECLASSIFIER_H
#define MOUTHSTATECLASSIFIER_H


#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "MouthStateDetector.h"
#include <algorithm>

enum class MouthState
{
    OPEN,
    CLOSE,
    UNKNOWN
};


struct MouthCandidate
{
    MouthState state;
    cv::Rect boundingBox;
};


class MouthStateClassifier
{
public:

    MouthStateClassifier(std::string openCascadeName,std::string closeCascadeName);

    void classifyMouths(const cv::Mat &greyImg, const cv::Rect& mouthBbox, cv::Rect &newMouthBox);

    MouthState getCurrentState();



    void detectCandidates(MouthStateDetector &detector, const cv::Rect &mouthBbox,
                          const MouthState state, const cv::Mat &img,const cv::Rect& roi);


    void selectNearestCand(const cv::Rect& mouthBbox, MouthCandidate &selected_cand);

private:

    std::vector<MouthCandidate> mouthCandidates;

    MouthCandidate selectedMouth;

    MouthStateDetector openDetector;
    MouthStateDetector closeDetector;
    MouthState currentMouthState;


};

#endif // MOUTHSTATECLASSIFIER_H
