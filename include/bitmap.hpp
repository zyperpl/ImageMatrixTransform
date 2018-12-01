#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <cstring>

class Parser;

#include "image.hpp"

enum InterpolationType
{
  Nearest, Bilinear, Lanczos3
};

class BitmapImage : public Image 
{
  public:
    BitmapImage() = default;
    BitmapImage(int width, int height) : width{width}, height{height}, components{3}
    {
      this->data = new uint8_t[width*height*components];
      memset(&(this->data[0]), 0, width*height*components);
    }
    ~BitmapImage() { delete[] data; }
    Image *apply(Matrix matrix, int width, int height);
    void save(const char *fileName, int width, int height);

    void putLine(int x1, int y1, int x2, int y2, Color color = { 255, 255, 255 });
    void putPixel(int x, int y, Color color = { 255, 255, 255 });

    void inline setInterpolation(InterpolationType interp) { this->interpolation = interp; };

  protected:
    int width{-1};
    int height{-1};
    int components{0};

    uint8_t *data{nullptr};

    InterpolationType interpolation;

    friend class Parser;
};

