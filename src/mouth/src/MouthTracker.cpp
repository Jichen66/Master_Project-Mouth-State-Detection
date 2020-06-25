#include "mouth/MouthTracker.h"

MouthTracker::MouthTracker()
{
    tracker = cv::TrackerMedianFlow::create();
}

void MouthTracker::reinitializeTracekr(const cv::Mat &frame, cv::Rect2d bbox)
{
    tracker = cv::TrackerMedianFlow::create();

    tracker->init(frame, bbox);
}

bool MouthTracker::updateTrack(const cv::Mat &frame, cv::Rect2d& bbox)
{
    bool ok = tracker->update(frame, bbox);
    return ok;
}

