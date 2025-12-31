#include <iostream>
#include <cstdlib>
#include <string>
#include "Vector.h"
#include "Matrix.h"
#include "Image.h"
#include "Convolution.h"
#include "SobelDetector.h"

using namespace std;

void testMatrixExceptions() {
    cout << "\n=== Matrix Exception Test ===" << endl;
    
    // 1. 准备维度不匹配的矩阵
    Matrix m1(2, 2); // 2行2列
    Matrix m2(2, 3); // 2行3列
    Matrix m3(3, 2); // 3行2列

    // 2. 测试加法异常
    cout << "[Test 1] Addition with mismatched columns (2x2 + 2x3): ";
    try {
        Matrix result = m1 + m2; // 应该抛出异常
        cout << "FAILED (Exception not thrown)" << endl;
    } catch (double e) {
        if (e == -1.0) {
            cout << "PASSED (Caught expected exception: " << e << ")" << endl;
        } else {
            cout << "FAILED (Caught wrong value: " << e << ")" << endl;
        }
    } catch (...) {
        cout << "FAILED (Caught unknown exception type)" << endl;
    }

    // 3. 测试减法异常
    cout << "[Test 2] Subtraction with mismatched rows (2x2 - 3x2): ";
    try {
        Matrix result = m1 - m3; // 应该抛出异常
        cout << "FAILED (Exception not thrown)" << endl;
    } catch (double e) {
        if (e == -1.0) {
            cout << "PASSED (Caught expected exception: " << e << ")" << endl;
        } else {
            cout << "FAILED (Caught wrong value: " << e << ")" << endl;
        }
    }
}

// Helper to create a sample image for demonstration
void createSampleImage(const string& filename) {
    int width = 200;
    int height = 200;
    Image img(height, width);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Background: Black
            double val = 0;

            // Shape 1: White Rectangle
            if (i >= 20 && i < 80 && j >= 20 && j < 80) {
                val = 255;
            }

            // Shape 2: Gray Circle
            int cx = 140, cy = 60;
            int r = 30;
            if ((i - cy)*(i - cy) + (j - cx)*(j - cx) < r*r) {
                val = 128;
            }

            // Shape 3: Gradient Strip
            if (i >= 120 && i < 180 && j >= 20 && j < 180) {
                val = (double)(j - 20) / 160.0 * 255.0;
            }

            img.setElement(i, j, val);
        }
    }
    
    if (img.savePGM(filename)) {
        cout << "Generated sample image: " << filename << endl;
    } else {
        cerr << "Failed to generate sample image." << endl;
    }
}

int main(int argc, char* argv[]) {
    string inputPath;
    string outputPath;
    double threshold = -1.0;
    bool invert = false;

    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input_pgm> <output_pgm> [threshold] [invert]" << endl;
        cout << "  threshold: 0-255, or -1 to disable" << endl;
        cout << "  invert: 'invert', 'true', or '1' to invert output (white background)" << endl;
        cout << "No arguments provided. Running internal tests and generating sample image..." << endl;
        
        testMatrixExceptions();

        createSampleImage("sample.pgm");
        inputPath = "sample.pgm";
        outputPath = "sample_edge.pgm";
        threshold = 100.0; // Demo threshold
    } else {
        inputPath = argv[1];
        outputPath = argv[2];
        if (argc > 3) {
            threshold = atof(argv[3]);
        }
        if (argc > 4) {
            string arg4 = argv[4];
            if (arg4 == "invert" || arg4 == "true" || arg4 == "1") {
                invert = true;
            }
        }
    }

    try {
        Image img;
        cout << "Loading image from " << inputPath << "..." << endl;
        if (!img.loadPGM(inputPath)) {
            cerr << "Error: Failed to load image file: " << inputPath << endl;
            return 1;
        }
        cout << "Image loaded. Size: " << img.getCols() << "x" << img.getRows() << endl;

        cout << "Applying Sobel edge detection..." << endl;
        SobelDetector sobel;
        sobel.setPadding(Convolution::Padding_Replicate);
        
        if (threshold >= 0) {
            cout << "Using threshold: " << threshold << endl;
            sobel.setThreshold(threshold);
        } else {
            cout << "Thresholding disabled." << endl;
        }

        if (invert) {
            cout << "Output inversion enabled (White background)." << endl;
            sobel.setInvert(true);
        }

        Image result = sobel.apply(img);

        cout << "Saving result to " << outputPath << "..." << endl;
        if (!result.savePGM(outputPath)) {
            cerr << "Error: Failed to save image file: " << outputPath << endl;
            return 1;
        }

        cout << "Processing complete successfully." << endl;

    } catch (int e) {
        cerr << "Error: Exception caught (int): " << e << endl;
        return 1;
    } catch (double e) {
        cerr << "Error: Exception caught (double): " << e << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Error: Unknown exception occurred." << endl;
        return 1;
    }

    return 0;
}