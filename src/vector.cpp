#include "vector.hpp"
#include "bitmap.hpp"

Polygon *VectorImage::addPolygon(Polygon *polygon)
{
  polygons.push_back(polygon);
  return polygons.back();
}

Image *VectorImage::apply(Matrix matrix, int width, int height)
{
  (void)(width+height);

  VectorImage *img = new VectorImage;
  
  for (const auto &poly : polygons)
  {
    auto np = poly->apply(matrix);
    np->color = poly->color;
    img->addPolygon(np);   
  }

  return img;
}

void VectorImage::save(const char *fileName, int width, int height)
{
  enum {
    VectorFileType, BitmapFileType
  } type = BitmapFileType;

  ssize_t fileNameLength = strlen(fileName);
  for (ssize_t i = fileNameLength-1; i >= 0; i--)
  {
    if (fileName[i] == '.')
    {
      if (strcmp(fileName+i, VECTOR_FILE_EXT) == 0)
      {
        type = VectorFileType;
        break;
      }
    }
  }

  if (type == VectorFileType) {
    printf("Saving file as vector file...\n");
    this->saveAsVector(fileName);      
    return;
  } 

  printf("Saving file as bitmap file...\n");
  this->saveAsBitmap(fileName, width, height);
}

void VectorImage::saveAsBitmap(const char *fileName, int width, int height)
{ 
  std::unique_ptr<BitmapImage> image(new BitmapImage(width, height));

  for (const auto &polygon : polygons)
  {
    size_t pointsNum = polygon->points.size();
    if (pointsNum <= 1) continue;

    for (size_t i = 0; i < pointsNum-1; i++)
    {
      auto p1 = polygon->points[i];
      auto p2 = polygon->points[i+1];

      image->putLine(p1.x, p1.y, p2.x, p2.y, polygon->color);
    }
    image->putLine(polygon->points[0].x, polygon->points[0].y,
            polygon->points[pointsNum-1].x, polygon->points[pointsNum-1].y,
            polygon->color);
  }

  image->save(fileName, width, height);
}

void VectorImage::saveAsVector(const char *fileName)
{
  // TODO: implement saving vector image as vector
  fprintf(stderr, "\nFUNCTION NOT IMPLEMENTED YET. \nCANNOT SAVE FILE %s AS VECTOR\n", fileName);
  printf("\nSaving vector image file as PNG file!\n");
  this->saveAsBitmap(fileName, 100, 100);
}

