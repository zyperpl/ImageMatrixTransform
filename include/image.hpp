#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

class Parser;

#include "linalg.h"

#define mul_int(vi, sd) static_cast<int>(((double)(vi)*sd + 0.5))
#define str std::to_string

struct Color
{
  int r, g, b;
  Color() : r{255}, g{255}, b{255} {}
  Color(int red, int green, int blue) : r{red}, g{green}, b{blue} {}

  Color operator*(const double &s) {
    return { mul_int(r,s), mul_int(g,s), mul_int(b,s) };
  }

  Color operator+(const Color &oc) {
    return { r + oc.r, g + oc.g, b + oc.b };
  }

  inline Color clamp() {
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    return { r, g, b };
  }

  const std::string toString() { return str(r) + "," + str(g) + "," + str(b); };
};

using Matrix = linalg::mat<double, 3, 3>;

class Image
{
  public: 
    virtual Image *apply(Matrix matrix, int width = 0, int height = 0) = 0;
    virtual void save(const char *fileName, int width, int height) = 0;
    virtual ~Image() {};
};
