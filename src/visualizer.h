#pragma once
#include <opencv2/opencv.hpp>

void draw_hud_box(cv::Mat& img, cv::Rect rect, float distance_meters);
void draw_target_line(cv::Mat& img, cv::Rect rect, float center_x, float center_y);

