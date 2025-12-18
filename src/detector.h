#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

struct Object { 
    cv::Rect2f rect; 
    int label; 
    float score; 
    std::vector<float> mask_coeffs; 
};

float bbox_iou(const cv::Rect2f& a, const cv::Rect2f& b);
std::vector<int> nms(const std::vector<Object>& objs, float thresh);
std::vector<Object> decode_yolo_seg(const float* output, int num_anchors, float scale_x, float scale_y);
