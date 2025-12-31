#ifndef IMAGE_H
#define IMAGE_H

#include "Matrix.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Image : public Matrix {
private:
    void skipComments(ifstream& file) {
        char c;
        while (true) {
            file >> ws; // Skip leading whitespace
            c = file.peek();
            if (c == '#') {
                file.ignore(65536, '\n'); // Skip line
            } else {
                break;
            }
        }
    }

public:
    // 构造函数
    Image(int h = 0, int w = 0) : Matrix(h, w) {}

    virtual void printInfo() const {
        cout << "Image (" << rows << "x" << cols << ")" << endl;
    }

    // 从数组初始化
    void fromArray(const double* arr, int h, int w) {
        resize(h, w);
        int k = 0;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                setElement(i, j, arr[k++]);
            }
        }
    }

    // 读取 PGM (P2 或 P5)
    bool loadPGM(const string& filename) {
        ifstream file(filename.c_str(), ios::binary);
        if (!file) return false;

        string format;
        file >> format;
        if (format != "P2" && format != "P5") return false;

        int w, h, maxVal;
        skipComments(file);
        file >> w;
        skipComments(file);
        file >> h;
        skipComments(file);
        file >> maxVal;
        // 注意：这里不能再调用 skipComments，因为 P5 格式在 maxVal 后只有一个空白字符，
        // 紧接着就是二进制数据。如果二进制数据恰好是空白字符或 '#'，skipComments 会出错。
        
        // 读取 maxVal 后的单个空白字符
        // 注意：Windows 下可能是 \r\n，视为一个分隔符处理
        if (maxVal <= 255) {
             int c = file.get();
             if (c == '\r' && file.peek() == '\n') {
                 file.get();
             }
        }

        if (file.fail()) return false;

        resize(h, w);

        if (format == "P2") {
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int val;
                    file >> val;
                    setElement(i, j, (double)val);
                }
            }
        } else {
            // P5 (Binary)
            if (maxVal > 255) return false; // 暂不支持 16 位 PGM
            unsigned char* rowBuf = new unsigned char[w];
            for (int i = 0; i < h; ++i) {
                file.read((char*)rowBuf, w);
                if (file.gcount() != w) {
                    delete[] rowBuf;
                    return false;
                }
                for (int j = 0; j < w; ++j) {
                    setElement(i, j, (double)rowBuf[j]);
                }
            }
            delete[] rowBuf;
        }
        return true;
    }

    // 保存 PGM (P2)
    bool savePGM(const string& filename) const {
        ofstream file(filename.c_str());
        if (!file) return false;

        file << "P2" << endl;
        file << getCols() << " " << getRows() << endl;
        file << "255" << endl;

        for (int i = 0; i < getRows(); ++i) {
            for (int j = 0; j < getCols(); ++j) {
                double val = getElement(i, j);
                int pixel = (int)val;
                if (pixel < 0) pixel = 0;
                if (pixel > 255) pixel = 255;
                file << pixel << (j == getCols() - 1 ? "" : " ");
            }
            file << endl;
        }
        return true;
    }

    // 裁剪
    Image crop(int x, int y, int w, int h) const {
        Image res(h, w);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (y + i < getRows() && x + j < getCols()) {
                    res.setElement(i, j, getElement(y + i, x + j));
                }
            }
        }
        return res;
    }

    // 调整大小 (最近邻插值)
    Image resizeImage(int new_w, int new_h) const {
        Image res(new_h, new_w);
        double scaleY = (double)getRows() / new_h;
        double scaleX = (double)getCols() / new_w;

        for (int i = 0; i < new_h; ++i) {
            for (int j = 0; j < new_w; ++j) {
                int srcY = (int)(i * scaleY);
                int srcX = (int)(j * scaleX);
                if (srcY >= getRows()) srcY = getRows() - 1;
                if (srcX >= getCols()) srcX = getCols() - 1;
                res.setElement(i, j, getElement(srcY, srcX));
            }
        }
        return res;
    }

    // 归一化 (0-255)
    void normalize() {
        if (getRows() == 0 || getCols() == 0) return;
        
        double minVal = getElement(0, 0);
        double maxVal = minVal;

        for (int i = 0; i < getRows(); ++i) {
            for (int j = 0; j < getCols(); ++j) {
                double val = getElement(i, j);
                if (val < minVal) minVal = val;
                if (val > maxVal) maxVal = val;
            }
        }

        if (maxVal - minVal < 1e-6) return;

        for (int i = 0; i < getRows(); ++i) {
            for (int j = 0; j < getCols(); ++j) {
                double val = getElement(i, j);
                setElement(i, j, (val - minVal) / (maxVal - minVal) * 255.0);
            }
        }
    }
};

#endif
