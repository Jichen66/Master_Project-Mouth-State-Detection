#include "mouth/MouthStateClassifier.h"
#include "ros/ros.h"


MouthStateClassifier::MouthStateClassifier(std::string openCascadeName,
                                           std::string closeCascadeName)
    :openDetector(openCascadeName),closeDetector(closeCascadeName)
{

}

void MouthStateClassifier::classifyMouths(const cv::Mat &greyImg,
                                          const cv::Rect &mouthBbox,
                                          cv::Rect &newMouthBox)
{

    currentMouthState = MouthState::UNKNOWN;

  //  std::cout<<"=============================="<<std::endl;
  //  std::cout<<"mouth box "<<mouthBbox<<std::endl;

    mouthCandidates.clear();


    float roi_x = mouthBbox.x-0.5*mouthBbox.width>0?mouthBbox.x-0.5*mouthBbox.width:0;
    float roi_y =mouthBbox.y-0.5*mouthBbox.height>0?mouthBbox.y-0.5*mouthBbox.height:0;
    float roi_width = 2*mouthBbox.width<(greyImg.cols-roi_x)?2*mouthBbox.width:(greyImg.cols-roi_x);
    float roi_height = 2*mouthBbox.height<(greyImg.rows-roi_y)?2*mouthBbox.height:(greyImg.rows-roi_y);

    cv::Rect roi = cv::Rect(roi_x,roi_y,roi_width,roi_height);
    ROS_DEBUG("close Mouth state detector");
 //   std::cout<<"roi box "<<roi<<std::endl;

//    cv::Mat roiImg = greyImg(roi);


    detectCandidates(closeDetector,mouthBbox,MouthState::CLOSE,greyImg,roi);
    ROS_DEBUG("open Mouth state detector");
    detectCandidates(openDetector,mouthBbox,MouthState::OPEN,greyImg,roi);

    if(mouthCandidates.size()!=0)
    {
        selectNearestCand(mouthBbox,selectedMouth);
        currentMouthState = selectedMouth.state;
//        newMouthBox = cv::Rect(selectedMouth.boundingBox.x+mouthBbox.x-0.5*mouthBbox.width,
//                               selectedMouth.boundingBox.y+mouthBbox.y-0.5*mouthBbox.height,
//                               selectedMouth.boundingBox.width,selectedMouth.boundingBox.height);
        newMouthBox = selectedMouth.boundingBox;
    }

    switch (currentMouthState) {
    case MouthState::OPEN:
        ROS_INFO("OPEN");
        break;
    case MouthState::CLOSE:
        ROS_INFO("CLOSE");
        break;
    default:
        break;
    }


}

MouthState MouthStateClassifier::getCurrentState()
{
    return currentMouthState;
}


void MouthStateClassifier::detectCandidates(MouthStateDetector &detector, const cv::Rect &mouthBbox,
                                             const MouthState state, const cv::Mat &img, const cv::Rect &roi)
{

    cv::Mat roiImg = img(roi);

    cv::Size minSize(mouthBbox.width*0.7,mouthBbox.height*0.7);
    cv::Size maxSize(mouthBbox.width*1.3,mouthBbox.height*1.3);
    MouthCandidate candidate;


    detector.detectMouths(roiImg,minSize,maxSize);
    std::vector<cv::Rect> detectedRects = detector.getMouthRects();

    for(auto rect:detectedRects)
    {
        rect.x = rect.x+roi.x;
        rect.y = rect.y+roi.y;
        candidate.boundingBox = rect;
        candidate.state = state;
        mouthCandidates.push_back(candidate);
    }

}

void MouthStateClassifier::selectNearestCand(const cv::Rect &mouthBbox,MouthCandidate& selected_cand)
{
 //   std::cout<<"........................"<<std::endl;

    float minRatio = 100;

    float ratioMouth = 1.0*mouthBbox.width/(1.0*mouthBbox.height);
    ROS_DEBUG_STREAM("ratio mouth");
//    std::cout<<"ratio mouth "<<ratioMouth<<std::endl;

    for(auto cand:mouthCandidates)
    {

        float ratioDetected = 1.0*cand.boundingBox.width/(1.0*cand.boundingBox.height);
 //       std::cout<<"ratio detected "<<ratioDetected<<std::endl;
        ROS_DEBUG_STREAM("ratio detected");

        float dRatio = fabs(ratioDetected-ratioMouth);

        float delta = dRatio-minRatio;

        if(fabs(delta)<0.1)
        {
            int dSelectedStartPt = abs(mouthBbox.x-selected_cand.boundingBox.x)
                    +abs(mouthBbox.y-selected_cand.boundingBox.y);
            int dCandStartPt = abs(mouthBbox.x-cand.boundingBox.x)+abs(mouthBbox.y-cand.boundingBox.y);

      //      std::cout<<"delta "<<delta<<"  dSele "<<dSelectedStartPt<<"   dCand  "<<dCandStartPt<<std::endl;
            ROS_DEBUG_STREAM("delta "<<delta<<"  dSele "<<dSelectedStartPt<<"   dCand  "<<dCandStartPt);
            if(dSelectedStartPt>dCandStartPt)
            {
                minRatio = dRatio;
                selected_cand = cand;
            }


        }


        else if(delta<0)
        {
            minRatio = dRatio;
            selected_cand = cand;
        }

    }


}
