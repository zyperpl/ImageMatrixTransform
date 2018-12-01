#pragma once

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#define GK3_VECTOR_TOKEN "#GK3"

class Image;

#include "linalg.h"

using Matrix = linalg::mat<double,3,3>;

class Parser
{
  public:  
    static Image *readBitmapImage(const char *fileName);
    static std::pair<int, Image*> readVectorImage(const char *fileName);
    static Image *readImage(const char *fileName);
    static std::pair<int, Matrix> readMatrix(const char *fileName);

  private:

    inline static bool isToken(std::string text, std::string token)
    {
      int p = 0;
      while (text[p] == ' ') p++;
      return text.compare(p, token.size(), token) == 0;
    };
};
