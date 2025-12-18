#include "visualizer.h"
#include <algorithm>

void draw_hud_box(cv::Mat& img, cv::Rect rect, float distance_meters) {
    // 1. Fill the box with light green
    cv::Mat overlay;
    img.copyTo(overlay);
    cv::rectangle(overlay, rect, cv::Scalar(0, 255, 0), -1);
    cv::addWeighted(overlay, 0.2, img, 0.8, 0, img); 
    
    // 2. Draw corners
    int x = rect.x, y = rect.y, w = rect.width, h = rect.height;
    int l = std::min(w, h) / 4; 
    int t = 2; 
    cv::Scalar color(0, 255, 0); 
    cv::line(img, cv::Point(x, y), cv::Point(x + l, y), color, t);
    cv::line(img, cv::Point(x, y), cv::Point(x, y + l), color, t);
    cv::line(img, cv::Point(x + w, y), cv::Point(x + w - l, y), color, t);
    cv::line(img, cv::Point(x + w, y), cv::Point(x + w, y + l), color, t);
    cv::line(img, cv::Point(x, y + h), cv::Point(x + l, y + h), color, t);
    cv::line(img, cv::Point(x, y + h), cv::Point(x, y + h - l), color, t);
    cv::line(img, cv::Point(x + w, y + h), cv::Point(x + w - l, y + h), color, t);
    cv::line(img, cv::Point(x + w, y + h), cv::Point(x + w, y + h - l), color, t);

    // 3. Distance text
    char dist_str[32];
    sprintf(dist_str, "%.1fm", distance_meters);
    cv::putText(img, dist_str, cv::Point(x, y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 255), 2);
}

void draw_target_line(cv::Mat& img, cv::Rect rect, float center_x, float center_y) {
    float obj_cx = rect.x + rect.width / 2.0f;
    float obj_cy = rect.y + rect.height / 2.0f;
    // Yellow line from center to target
    cv::line(img, cv::Point(center_x, center_y), cv::Point(obj_cx, obj_cy), cv::Scalar(0, 255, 255), 2);
    // Red dot at target center
    cv::circle(img, cv::Point(obj_cx, obj_cy), 5, cv::Scalar(0, 0, 255), -1);
}

