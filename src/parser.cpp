#include "parser.hpp"
#include "bitmap.hpp"
#include "vector.hpp"

#include "stb_image.h"

Image *Parser::readBitmapImage(const char *fileName)
{ 
  BitmapImage *img = new BitmapImage();
  img->data = stbi_load(fileName, &(img->width), &(img->height), &(img->components), STBI_rgb);
  
  if (!img->data) 
  {
    fprintf(stderr, "Cannot load bitmap file \"%s\"!\n", fileName);
    delete img;
    return nullptr;
  }
  return img;
}

std::pair<int, Image*> Parser::readVectorImage(const char *fileName)
{
  std::string line;

  std::ifstream f(fileName);

  if (!f.is_open()) return { -3, nullptr };

  std::getline(f, line);
  std::istringstream iss(line);
  std::string token;
  if (!(iss >> token)) { return std::make_pair(-1, nullptr);  } 

  if (token != GK3_VECTOR_TOKEN) return { -2, nullptr };

  int read = 0;
  VectorImage *img = new VectorImage();
  
  std::string currentToken = "";

  Polygon *currentPolygon = nullptr;

  while (std::getline(f, line))
  {
    //printf("%s\n", line.c_str());
    if (isToken(line, "polygon"))
    {
      currentToken = "polygon";
      currentPolygon = img->addPolygon(new Polygon());
      read++;
    }

    if (currentToken == "polygon")
    {
      if (isToken(line, "color")) 
      {
        std::istringstream iss(line);
        std::string text;
        int r, g, b;
        if (!(iss >> text >> r >> g >> b))
        {
          printf("Cannot read color token!\n");
          return { -4, img };
        }
        (*currentPolygon).color = { r, g, b };
      } else
      if (isToken(line, "points"))
      {
        currentToken = "points";
      }
    } else
    if (currentToken == "points")
    {
      std::istringstream iss(line);
      double x, y;
      if (!(iss >> x >> y)) 
      {
        currentToken = "";
        currentPolygon = nullptr;
      } else
      {
        currentPolygon->points.push_back({x, y});
      }
    }
  }
  
  f.close();
  return { read, img };
}

Image *Parser::readImage(const char *fileName)
{
  int read = -1;
  Image *image = nullptr;
  std::tie(read, image) = readVectorImage(fileName);

  if (read <= 0) image = readBitmapImage(fileName);

  return image;
}

std::pair<int, Matrix> Parser::readMatrix(const char *fileName)
{
  std::ifstream f(fileName);

  if (!f.is_open()) return { -1, Matrix() };

  std::string line;
  std::vector<Matrix> matrices;

  Matrix mat;

  int adding = 0;

  while (std::getline(f, line)) 
  { 
    if (isToken(line, "[")) {
      adding = 1;
      mat = Matrix();
    } else
    if (adding > 0)
    {
      if (isToken(line, "]")) 
      {
        adding = 0;
        matrices.push_back(mat);
      } else
      {
        std::istringstream iss(line);
        double a, b, c;
        if (!(iss >> a >> b >> c)) {
          printf("Cannot read matrix values");
        }
        int row = adding-1;
        mat[0][row] = a;
        mat[1][row] = b;
        mat[2][row] = c;

        adding++;
      }
    }
  }
  f.close();

  if (matrices.size() <= 0) return { -2, Matrix() };

  mat = matrices[0];

  for (size_t i = 1; i < matrices.size(); i++)
  {
    mat = linalg::mul(mat, matrices[i]);
  }
  
  return { matrices.size(), mat };
}
