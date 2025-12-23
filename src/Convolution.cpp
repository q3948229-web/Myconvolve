#include "Convolution.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Convolution::Convolution() : kernel(3, 3), stride(1), paddingMode(Padding_Zero) {
    // Default identity kernel
    kernel.setElement(1, 1, 1.0);
}

Convolution::Convolution(const Matrix& k, int s, PaddingMode p) : kernel(k), stride(s), paddingMode(p) {}

void Convolution::setKernel(const Matrix& k) {
    kernel = k;
}

void Convolution::setStride(int s) {
    stride = s;
}

void Convolution::setPadding(PaddingMode p) {
    paddingMode = p;
}

Image Convolution::apply(const Image& input) const {
    int kRows = kernel.getRows();
    int kCols = kernel.getCols();
    int inRows = input.getRows();
    int inCols = input.getCols();

    int padH = 0, padW = 0;
    if (paddingMode != Padding_None) {
        padH = (kRows - 1) / 2;
        padW = (kCols - 1) / 2;
    }

    int outRows = (inRows + 2 * padH - kRows) / stride + 1;
    int outCols = (inCols + 2 * padW - kCols) / stride + 1;

    if (outRows <= 0 || outCols <= 0) {
        return Image(0, 0);
    }

    Image output(outRows, outCols);

    for (int i = 0; i < outRows; ++i) {
        for (int j = 0; j < outCols; ++j) {
            double sum = 0.0;
            
            // Center of the kernel on the input image
            int startY = i * stride - padH;
            int startX = j * stride - padW;

            for (int m = 0; m < kRows; ++m) {
                for (int n = 0; n < kCols; ++n) {
                    int imgY = startY + m;
                    int imgX = startX + n;

                    double pixelVal = 0.0;

                    if (imgY >= 0 && imgY < inRows && imgX >= 0 && imgX < inCols) {
                        pixelVal = input.getElement(imgY, imgX);
                    } else {
                        if (paddingMode == Padding_Zero) {
                            pixelVal = 0.0;
                        } else if (paddingMode == Padding_Replicate) {
                            int clampedY = imgY < 0 ? 0 : (imgY >= inRows ? inRows - 1 : imgY);
                            int clampedX = imgX < 0 ? 0 : (imgX >= inCols ? inCols - 1 : imgX);
                            pixelVal = input.getElement(clampedY, clampedX);
                        }
                    }
                    sum += pixelVal * kernel.getElement(m, n);
                }
            }
            output.setElement(i, j, sum);
        }
    }

    return output;
}

Matrix Convolution::createIdentityKernel(int size) {
    Matrix k(size, size);
    int center = size / 2;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            k.setElement(i, j, (i == center && j == center) ? 1.0 : 0.0);
        }
    }
    return k;
}

Matrix Convolution::createBoxBlurKernel(int size) {
    Matrix k(size, size);
    double val = 1.0 / (size * size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            k.setElement(i, j, val);
        }
    }
    return k;
}

Matrix Convolution::createGaussianKernel(int size, double sigma) {
    Matrix k(size, size);
    double sum = 0.0;
    int center = size / 2;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int x = i - center;
            int y = j - center;
            double val = (1.0 / (2.0 * M_PI * sigma * sigma)) * 
                         exp(-(x * x + y * y) / (2.0 * sigma * sigma));
            k.setElement(i, j, val);
            sum += val;
        }
    }

    // Normalize
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            k.setElement(i, j, k.getElement(i, j) / sum);
        }
    }
    return k;
}

Matrix Convolution::createSobelXKernel() {
    Matrix k(3, 3);
    // -1 0 1
    // -2 0 2
    // -1 0 1
    k.setElement(0, 0, -1); k.setElement(0, 1, 0); k.setElement(0, 2, 1);
    k.setElement(1, 0, -2); k.setElement(1, 1, 0); k.setElement(1, 2, 2);
    k.setElement(2, 0, -1); k.setElement(2, 1, 0); k.setElement(2, 2, 1);
    return k;
}

Matrix Convolution::createSobelYKernel() {
    Matrix k(3, 3);
    // -1 -2 -1
    //  0  0  0
    //  1  2  1
    k.setElement(0, 0, -1); k.setElement(0, 1, -2); k.setElement(0, 2, -1);
    k.setElement(1, 0, 0);  k.setElement(1, 1, 0);  k.setElement(1, 2, 0);
    k.setElement(2, 0, 1);  k.setElement(2, 1, 2);  k.setElement(2, 2, 1);
    return k;
}
