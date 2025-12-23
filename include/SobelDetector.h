#ifndef SOBELDETECTOR_H
#define SOBELDETECTOR_H

#include "Convolution.h"

class SobelDetector : public Convolution {
private:
    bool useThreshold;
    double thresholdValue;

public:
    SobelDetector();
    
    // 设置阈值（启用阈值处理）
    void setThreshold(double t);
    
    // 禁用阈值处理
    void disableThreshold();

    // 重写 apply 方法
    virtual Image apply(const Image& input) const;
};

#endif
