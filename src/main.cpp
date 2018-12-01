#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "vector.hpp"
#include "bitmap.hpp"
#include "parser.hpp"
#include "image.hpp"

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage:\n%s <input_file> <matrix_file> <output_file> <output_width> <output_height>", argv[0]);
    return 1;
  }
  const char *inputFileName = argv[1];
  const char *matrixFileName = argv[2];
  const char *outputFileName = argv[3];
  int outputImageWidth  = 800;
  int outputImageHeight = 600;
  if (argc >= 5)
  {
    outputImageWidth  = std::atoi(argv[4]);
    outputImageHeight = std::atoi(argv[5]);
  }


  // read image
  Image *image = Parser::readImage(inputFileName);
  if (!image) 
  {
    fprintf(stderr, "Cannot load image \"%s\"!", argv[1]);
    return 2;
  }

  // read matrix
  Matrix matrix;
  int matricesNum = 0;
  std::tie(matricesNum, matrix) = Parser::readMatrix(matrixFileName);

  if (matricesNum <= 0)
  {
    fprintf(stderr, "Cannot read matrix file \"%s\"!", matrixFileName);
    return 3;
  }

  // print matrix
  for (int y = 0; y < 3; y++) {
    for (int x = 0; x < 3; x++) {
      printf("%2.2f ", matrix[x][y]);
    }
    printf("\n");
  }

  // apply transformation
  Image *transformedImage = image->apply(matrix, outputImageWidth, outputImageHeight);
  if (!transformedImage) 
  {
    fprintf(stderr, "Matrix cannot be applied!\n");
    return 4;
  }

  // save image
  transformedImage->save(outputFileName, outputImageWidth, outputImageHeight); 

  delete transformedImage;
  delete image;

  return 0;
}
