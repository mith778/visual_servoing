#include <iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void image_processing( Mat image, cv::Mat &UU, float &centroid_x, float &centroid_y)
{

    /*---------------------------------------------------------------------------------*/

    Point2f  rect_points[4];
    Mat hsv_img;
    Mat threshold_frame_blue;
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours_blue;
    int l;
    int k;
    float sum_x, sum_y;

    cvtColor(image, hsv_img, CV_BGR2HSV);

    cv::inRange( hsv_img, Scalar(150,150,60), Scalar(179,225,255), threshold_frame_blue);
    imwrite("mask_blue.png", threshold_frame_blue);

/*-------------find contour-------------------*/
    findContours(threshold_frame_blue, contours_blue, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );

/*------------ find retangle-------------------*/
    RNG rng(12345);
    vector<RotatedRect> minRect( contours_blue.size() );
    for( int i = 0; i < contours_blue.size(); i++ )
    {
        minRect[i] = minAreaRect( Mat(contours_blue[i]) );
        minRect[i].points( rect_points );
    }

    threshold_frame_blue.release();
    l=contours_blue.size();

/*---------find centroid of the Desired------------*/
    k = contours_blue[l-1].size();
    sum_x=0;
    sum_y=0;
    for(int ii=0; ii<k; ii++)
    {
        sum_x=sum_x+contours_blue[l-1][ii].x;
        sum_y=sum_y+contours_blue[l-1][ii].y;
    //  cout<<"pt"<< ptCloud->at(contours_blue[l-1][ii].x,contours_blue[l-1][ii].y)<<endl;
    }
    centroid_x = sum_x/float(k);
    centroid_y = sum_y/float(k);
//    cout<< "centroid_x" << centroid_x <<""<< centroid_y<<" "<<endl;
    int j=0;
    double uu[8];
    for (int i=0;i<4;i++)
    {
        uu[j]=rect_points[i].x;
        uu[j+1]=rect_points[i].y;
        j=j+2;
    }
    UU = cv::Mat(8, 1, CV_64F, uu);
}
