#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <cstring>
#include <memory>

#include "image.hpp"
#include "linalg.h"

using Matrix = linalg::mat<double, 3, 3>;

#define VECTOR_FILE_EXT ".gk3"

#define str std::to_string

struct Point

{
  double x;
  double y;

  const std::string toString() { return str(x) + ";" + str(y); }
};

struct Polygon 
{
  Color color = { 255, 0, 0 };
  std::vector<Point> points;

  inline Polygon *apply(Matrix matrix) 
  {
    Polygon *poly = new Polygon;
    for (const auto &p : points)
    {
      linalg::vec<double, 3> v(p.x, p.y, 1);
      auto nv = linalg::mul(matrix, v);
      poly->points.push_back({nv.x, nv.y});
    }
    return poly;
  }
};

class VectorImage : public Image 
{
  public:
    ~VectorImage() { for (auto p : polygons) delete p; };

    Polygon *addPolygon(Polygon *polygon);
    
    Image *apply(Matrix matrix, int width = 0, int height = 0);
    void save(const char *fileName, int width, int height);
    void saveAsBitmap(const char *fileName, int width, int height);
    void saveAsVector(const char *fileName);

  private:
    std::vector<Polygon*> polygons;
};
