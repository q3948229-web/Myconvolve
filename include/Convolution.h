#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "Image.h"
#include "Matrix.h"

class Convolution {
public:
    enum PaddingMode {
        Padding_None,
        Padding_Zero,
        Padding_Replicate
    };

protected:
    Matrix kernel;
    int stride;
    PaddingMode paddingMode;

public:
    Convolution();
    Convolution(const Matrix& k, int s = 1, PaddingMode p = Padding_Zero);

    void setKernel(const Matrix& k);
    void setStride(int s);
    void setPadding(PaddingMode p);

    virtual Image apply(const Image& input) const;

    // Static helpers to create common kernels
    static Matrix createIdentityKernel(int size);
    static Matrix createBoxBlurKernel(int size);
    static Matrix createGaussianKernel(int size, double sigma);
    static Matrix createSobelXKernel();
    static Matrix createSobelYKernel();
};

#endif
