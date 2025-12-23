#include "SobelDetector.h"
#include <cmath>

SobelDetector::SobelDetector() : Convolution(), useThreshold(false), thresholdValue(0.0), invertOutput(false) {
    // SobelDetector doesn't use the base 'kernel' member for the main operation,
    // but we initialize the base class anyway.
}

void SobelDetector::setThreshold(double t) {
    useThreshold = true;
    thresholdValue = t;
}

void SobelDetector::disableThreshold() {
    useThreshold = false;
}

void SobelDetector::setInvert(bool inv) {
    invertOutput = inv;
}

Image SobelDetector::apply(const Image& input) const {
    // Create Gx and Gy kernels
    Matrix kx = Convolution::createSobelXKernel();
    Matrix ky = Convolution::createSobelYKernel();

    // Create Convolution objects for X and Y
    // We use the padding mode set in this SobelDetector instance (inherited from Convolution)
    Convolution convX(kx, 1, paddingMode);
    Convolution convY(ky, 1, paddingMode);

    // Apply convolutions
    Image gx = convX.apply(input);
    Image gy = convY.apply(input);

    // Result image
    int rows = gx.getRows();
    int cols = gx.getCols();
    Image result(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double valX = gx.getElement(i, j);
            double valY = gy.getElement(i, j);
            
            // Calculate magnitude
            double magnitude = sqrt(valX * valX + valY * valY);

            // Apply threshold if enabled
            if (useThreshold) {
                if (magnitude > thresholdValue) {
                    magnitude = 255.0;
                } else {
                    magnitude = 0.0;
                }
            }

            // Apply inversion
            if (invertOutput) {
                magnitude = 255.0 - magnitude;
                if (magnitude < 0) magnitude = 0; // Safety clamp
            }

            result.setElement(i, j, magnitude);
        }
    }

    return result;
}
