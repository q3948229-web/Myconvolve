#ifndef SOBELDETECTOR_H
#define SOBELDETECTOR_H

#include "Convolution.h"

class SobelDetector : public Convolution {
private:
    bool useThreshold;
    double thresholdValue;
    bool invertOutput;

public:
    SobelDetector();
    
    // 设置阈值（启用阈值处理）
    void setThreshold(double t);
    
    // 禁用阈值处理
    void disableThreshold();

    // 设置是否反转输出（true=白底黑边，false=黑底白边）
    void setInvert(bool inv);

    // 重写 apply 方法
    virtual Image apply(const Image& input) const;
};

#endif
