#include "bitmap.hpp"
   
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image *BitmapImage::apply(Matrix matrix, int width, int height)
{
  this->interpolation = Lanczos3;

  BitmapImage *img = new BitmapImage(width, height);
  auto invMat = linalg::inverse(matrix); 

  auto getSourcePixel = [this](int x, int y) -> Color {
    if (x < 0) return { 0, 0, 0 };
    if (y < 0) return { 0, 0, 0 };
    if (x >= this->width)  return { 0, 0, 0 };
    if (x >= this->height) return { 0, 0, 0 };
    size_t idx = (x+y*this->width)*this->components;
    int r = this->data[idx+0];
    int g = this->data[idx+1];
    int b = this->data[idx+2];
    return { r, g, b };
  };

  for (int y = 0; y < height; y++)
  for (int x = 0; x < width; x++)
  {
    linalg::vec<double,3> v(x,y,1); 

    auto p = linalg::mul(invMat, v);

    if ((int)(p.x) >= this->width) continue;
    if ((int)(p.y) >= this->height) continue;
    if ((int)(p.x) < 0) continue;
    if ((int)(p.y) < 0) continue;

    int destPxI = (x+y*width)*components;

    if (interpolation == Nearest)
    {
      auto col = getSourcePixel(p.x, p.y);
      img->data[destPxI + 0] = col.r;
      img->data[destPxI + 1] = col.g;
      img->data[destPxI + 2] = col.b;
    } else
    if (interpolation == Bilinear)
    {
      double diffX = (p.x - (int)p.x);
      double diffY = (p.y - (int)p.y);
      auto c1_1 = getSourcePixel(p.x, p.y);
      auto c2_1 = getSourcePixel(p.x+1, p.y);
      auto c1_2 = getSourcePixel(p.x, p.y+1);
      auto c2_2 = getSourcePixel(p.x+1, p.y+1);

      auto subColor1 = c2_1*diffX + c1_1*(1.0-diffX);
      auto subColor2 = c2_2*diffX + c1_2*(1.0-diffX);

      auto out = subColor1*(1.0-diffY) + subColor2*diffY;
      
      img->data[destPxI + 0] = out.r;
      img->data[destPxI + 1] = out.g;
      img->data[destPxI + 2] = out.b;
    } else
    if (interpolation == Lanczos3)
    {
      auto funcL = [](double x) -> double
      {
        if (x == 0.0) return 1.0;
        if (fabs(x) >= 3.0) return 0.0;
        return (sin(M_PI*x) * sin((M_PI*x) / 3.0)) / ((M_PI*x)*((M_PI*x)/3.0));
        return ((sin(M_PI*x)/(M_PI*x) ) * (sin((M_PI*x)/3.0) / ((M_PI*x)/3.0)));
      };

      auto ik = [p,getSourcePixel,funcL](int k) {
        Color sum = { 0, 0, 0 };
        for (int i = -2; i <= 3; i++)
        {
          int xsi = int(p.x) + i;
          int ysk = int(p.y) + k;
          double ai = funcL(p.x - xsi);
          sum = sum + (getSourcePixel(xsi, ysk) * ai);
        }
        return sum.clamp();
      }; 
      
      Color col = { 0, 0, 0 };
      for (int k = -2; k <= 3; k++)
      {
        int ysi = int(p.y) + k;
        double bk = funcL(p.y - ysi);
        col = col + (ik(k)*bk);   
      }
      col.clamp();

      //printf("%s\n", col.toString().c_str());

      img->data[destPxI + 0] = col.r;
      img->data[destPxI + 1] = col.g;
      img->data[destPxI + 2] = col.b;
    } else
    {
      printf("Unknown interpolation (%d)!\n", interpolation);
      return nullptr;
    }
  }

  return img;  
}

void BitmapImage::save(const char *fileName, int width, int height)
{
  if (!data) { printf("No data to be saved!\n\n"); return; }
  
  // TODO: add file ext detection
  stbi_write_png(fileName, width, height, components, data, width*components); 
}
  
void BitmapImage::putLine(int x1, int y1, int x2, int y2, Color color)
{
	bool yLonger=false;
	int incrementVal;

	int shortLen=y2-y1;
	int longLen=x2-x1;
	if (abs(shortLen)>abs(longLen)) {
		int swap=shortLen;
		shortLen=longLen;
		longLen=swap;
		yLonger=true;
	}

	if (longLen<0) incrementVal=-1;
	else incrementVal=1;

	double divDiff;
	if (shortLen==0) divDiff=longLen;
	else divDiff=(double)longLen/(double)shortLen;
	if (yLonger) {
		for (int i=0;i!=longLen;i+=incrementVal) {
			putPixel(x1+(int)((double)i/divDiff),y1+i, color);
		}
	} else {
		for (int i=0;i!=longLen;i+=incrementVal) {
			putPixel(x1+i,y1+(int)((double)i/divDiff), color);
		}
	}
}

void BitmapImage::putPixel(int x, int y, Color color)
{
  if (!this->data) return;
  if (x >= width) return;
  if (y >= height) return;
  if (x < 0) return;
  if (y < 0) return;

  //printf("Drawing at %d;%d: %s\n", x, y, color.toString().c_str());
  data[(y * width + x)*components + 0] = color.r;
  data[(y * width + x)*components + 1] = color.g;
  data[(y * width + x)*components + 2] = color.b;
}
