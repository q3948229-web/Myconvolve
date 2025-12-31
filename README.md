# Matrix Image Processor

A C++98 library for matrix operations and image processing, featuring a custom Vector/Matrix implementation and Sobel edge detection.

## Build

Requires CMake and a C++ compiler (e.g., MinGW, g++).

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

Run the executable from the command line:

```bash
./matrix_conv.exe <input_pgm> <output_pgm> [threshold]
```

- `input_pgm`: Path to input PGM (P2) image.
- `output_pgm`: Path to save the result.
- `threshold`: (Optional) Threshold value (0-255) for binary edge detection. If omitted, outputs gradient magnitude.

## Demo

Run without arguments to generate a sample image (`sample.pgm`) and perform edge detection on it:

```bash
./matrix_conv
```
