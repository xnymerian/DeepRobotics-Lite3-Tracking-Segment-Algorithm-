#include "detector.h"
#include "config.h"
#include <algorithm>
#include <numeric>

float bbox_iou(const cv::Rect2f& a, const cv::Rect2f& b) {
    float inter = (a & b).area();
    float union_a = a.area() + b.area() - inter;
    return (union_a <= 0) ? 0 : inter / union_a;
}

std::vector<int> nms(const std::vector<Object>& objs, float thresh) {
    std::vector<int> indices;
    std::vector<int> order(objs.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) { 
        return objs[a].score > objs[b].score; 
    });
    
    while (!order.empty()) {
        int idx = order.front();
        indices.push_back(idx);
        std::vector<int> tmp;
        for (size_t i = 1; i < order.size(); ++i) {
            if (bbox_iou(objs[idx].rect, objs[order[i]].rect) <= thresh) {
                tmp.push_back(order[i]);
            }
        }
        order.swap(tmp);
    }
    return indices;
}

std::vector<Object> decode_yolo_seg(const float* output, int num_anchors, float scale_x, float scale_y) {
    std::vector<Object> proposals; 
    const int kMaskDims = 32;
    const float* ptr_score = output + 4 * num_anchors;
    
    for (int i = 0; i < num_anchors; ++i) {
        if (ptr_score[i] < OBJ_THRESH) continue; 
        float x = output[0 * num_anchors + i];
        float y = output[1 * num_anchors + i];
        float w = output[2 * num_anchors + i];
        float h = output[3 * num_anchors + i];
        
        Object obj;
        obj.rect = cv::Rect2f((x - w * 0.5f) * scale_x, (y - h * 0.5f) * scale_y, w * scale_x, h * scale_y);
        obj.label = 0;
        obj.score = ptr_score[i];
        obj.mask_coeffs.resize(kMaskDims);
        for (int m = 0; m < kMaskDims; ++m) {
            obj.mask_coeffs[m] = output[(84 + m) * num_anchors + i];
        }
        proposals.push_back(obj);
    }
    
    std::vector<Object> results;
    auto keep = nms(proposals, NMS_THRESH);
    for (int idx : keep) {
        results.push_back(proposals[idx]);
    }
    return results;
}

