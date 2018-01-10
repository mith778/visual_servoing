/* C++ Interface to other functions
 *
 * FILE: my_funcs.h
 *
 * Date: 09 August 2007 Thursday
 * Date: 12 October 2007 Friday
 * ---------------------------------- */
#ifndef _MY_FUNCS_H
#define _MY_FUNCS_H

#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void MyFilledCircle( Mat img, Point center , int r,int g, int b);
void image_processing( Mat image,cv::Mat &UU, float &centroid_x, float &centroid_y);
void compute_image_jacobian(double u[], double z[],cv::Mat &L);
void compute_image_jacobian_area(double u[], double z[], cv::Mat &L);
void compute_image_jacobian_depth(double u[], double z[],cv::Mat &L);
void compute_image_jacobian_four_points(double u[], double z[], cv::Mat &L);

void matrix_for_ee_to_camera( cv::Mat vector_ec,cv::Mat New_Rot,cv::Mat &mul_Jacob);
void detect_blob( Mat image, cv::Mat &UU, float &centroid_x, float &centroid_y);

#endif
