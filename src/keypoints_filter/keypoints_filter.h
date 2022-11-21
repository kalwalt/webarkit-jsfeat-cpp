#ifndef KEYPOINTS_FILTER_H
#define KEYPOINTS_FILTER_H

#include <keypoint_t/keypoint_t.h>
#include <algorithm>
#include <vector>

namespace jsfeat{

struct KeypointResponseGreaterThanOrEqualToThreshold
{
    KeypointResponseGreaterThanOrEqualToThreshold(float _value) :
    value(_value)
    {
    }
    inline bool operator()(const KeyPoint_t& kpt) const
    {
        //return kpt.response >= value;
        return kpt.score >= value;
    }
    float value;
};

struct KeypointResponseGreater
{
    inline bool operator()(const KeyPoint_t& kp1, const KeyPoint_t& kp2) const
    {
        //return kp1.response > kp2.response;
        return kp1.score > kp2.score;
    }
};

class KeyPointsFilter {
    public:
    KeyPointsFilter(){}
     /*
     * Retain the specified number of the best keypoints (according to the response)
     */
    static void retainBest( std::vector<KeyPoint_t>& keypoints, int n_points ){
    //this is only necessary if the keypoints size is greater than the number of desired points.
    if( n_points >= 0 && keypoints.size() > (size_t)n_points )
    {
        if (n_points==0)
        {
            keypoints.clear();
            return;
        }
        //first use nth element to partition the keypoints into the best and worst.
        std::nth_element(keypoints.begin(), keypoints.begin() + n_points - 1, keypoints.end(), KeypointResponseGreater());
        //this is the boundary response, and in the case of FAST may be ambiguous
        //float ambiguous_response = keypoints[n_points - 1].response;
        float ambiguous_response = keypoints[n_points - 1].score;
        //use std::partition to grab all of the keypoints with the boundary response.
        std::vector<KeyPoint_t>::const_iterator new_end =
        std::partition(keypoints.begin() + n_points, keypoints.end(),
                       KeypointResponseGreaterThanOrEqualToThreshold(ambiguous_response));
        //resize the keypoints, given this new end point. nth_element and partition reordered the points inplace
        keypoints.resize(new_end - keypoints.begin());
    }
}
};

};

#endif