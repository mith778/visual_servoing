#include <iostream>
#include<opencv2/opencv.hpp>
#include<my_funcs.h>

using namespace std;
using namespace cv;

void MyFilledCircle( Mat img, Point center , int r,int g, int b)
  {
   int thickness = 5;
   int lineType = 8;

   circle( img,
           center,
           4,
           Scalar( r, g, b ),
           thickness,
           lineType );
  }
