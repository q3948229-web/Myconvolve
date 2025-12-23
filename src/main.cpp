#include <iostream>
#include <cstdlib>
#include <string>
#include "Vector.h"
#include "Matrix.h"
#include "Image.h"
#include "Convolution.h"
#include "SobelDetector.h"

using namespace std;

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

    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <input_pgm> <output_pgm> [threshold]" << endl;
        cout << "No arguments provided. Generating and using sample image..." << endl;
        
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