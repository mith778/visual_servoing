#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<pcl_ros/point_cloud.h>
#include<pcl/point_cloud.h>
#include<pcl_conversions/pcl_conversions.h>
//#include<object_detect/image_data.h>
#include<std_msgs/Float64.h>
#include<sensor_msgs/PointCloud2.h>
#include<new_detection/points_four_image_data.h>
#include "opencv2/core/core.hpp"
#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

Mat image;

bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 86, vmax = 196, smin = 116;

int iLastX = 0;
int iLastY = 0;

int iLowH =46;
int iHighH =84;

int iLowS = 129;
int iHighS =244;

int iLowV = 84;
int iHighV = 177;

// User draws box around object to track. This triggers CAMShift to start tracking
static void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
    }

    switch( event )
    {
    case EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
            trackObject = -1;   // Set up CAMShift properties in main() loop
        break;
    }
}

string hot_keys =
        "\n\nHot keys: \n"
        "\tESC - quit the program\n"
        "\tc - stop the tracking\n"
        "\tb - switch to/from backprojection view\n"
        "\th - show/hide object histogram\n"
        "\tp - pause video\n"
        "To initialize tracking, select the object with mouse\n";

static void help()
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
            "You select a color objects such as your face and it tracks it.\n"
            "This reads from video camera (0 by default, or the camera number the user enters\n"
            "Usage: \n"
            "   ./camshiftdemo [camera number]\n";
    cout << hot_keys;
}

const char* keys =
{
    "{help h | | show help message}{@camera_number| 0 | camera number}"
};


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    //    image_in = msg;
    try
    {
        image = cv_bridge::toCvShare(msg, "bgr8")->image;
        //        cv::imshow("test", image);
        //        cv::waitKey(0);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }

}

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

void ptCloudcallBack(const sensor_msgs::PointCloud2::ConstPtr &input, pcl::PointCloud<pcl::PointXYZRGB>::Ptr ptCloud, bool &flag)
{

    pcl::fromROSMsg( *input, *ptCloud );
    flag = true;
}

int main( int argc, char** argv )
{
    //    VideoCapture cap;
    Rect trackWindow;
    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours_blue;
    Point2f  rect_points[4];
    double uu[8];



    Mat imgTh;

    //    CommandLineParser parser(argc, argv, keys);
    //    if (parser.has("help"))
    //    {
    //        help();
    //        return 0;
    //    }
    //    int camNum = parser.get<int>(0);
    //    cap.open(camNum);

    //    if( !cap.isOpened() )
    //    {
    //        help();
    //        cout << "***Could not initialize capturing...***\n";
    //        cout << "Current parameter's value: \n";
    //        parser.printMessage();
    //        return -1;
    //    }
    //    cout << hot_keys;


    ros::init(argc, argv, "object_detect");
    ros::NodeHandle handle1;
    image_transport::ImageTransport img_trans(handle1);
    bool flag_ptcloud;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr ptcloud(new pcl::PointCloud<pcl::PointXYZRGB>());

    ros::Publisher pub = handle1.advertise<new_detection::points_four_image_data>("/object_points", 10);
    new_detection::points_four_image_data data;

    //        namedWindow( "Histogram", 0 );
    //        namedWindow( "CamShift Demo", 0 );
    image_transport::Subscriber sub_img1 = img_trans.subscribe("/camera/rgb/image_raw",1,imageCallback);
    sleep(1);
    ros::spinOnce();


    namedWindow( "Histogram", 0 );
    namedWindow( "CamShift Demo", 0 );
    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    setMouseCallback( "CamShift Demo", onMouse, 0 );
//    createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
//    createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
//    createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );
    createTrackbar("LowH", "Control", &iLowH, 179);
    createTrackbar("HighH", "Control", &iHighH,179);

    createTrackbar("LowS", "Control", &iLowS, 255);
    createTrackbar("HighS", "Control", &iHighS,255);

    createTrackbar("LowV", "Control", &iLowV, 255);
    createTrackbar("HighV", "Control", &iHighV,255);

    while(image.empty())
    {
        ros::spinOnce();
        cout << "empty image" << endl;

    }

    ros::Subscriber sub = handle1.subscribe<sensor_msgs::PointCloud2>("/camera/depth/points", 1, boost::bind(ptCloudcallBack, _1, boost::ref(ptcloud),boost::ref(flag_ptcloud)));
    sleep(1);
    ros::spinOnce();

    Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
    bool paused = false;

 double area;
    while(ros::ok())
    {

        pcl::PointXYZRGB pt;
        Mat imgLines = Mat::zeros(image.size(), CV_8UC3);

        if( !paused )
        {
            // image = frame;
            if( image.empty() )
                break;
        }

        //        frame.copyTo(image);

        if( !paused )
        {
            cvtColor(image, hsv, COLOR_BGR2HSV);

            if( trackObject )
            {
                int _vmin = vmin, _vmax = vmax;

//                inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
//                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);

                inRange(hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), mask);
                cv::imshow("Mask", mask);

                erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
                dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));

                dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
                erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));

                mask.copyTo(imgTh);
                findContours(imgTh, contours_blue, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
                vector<RotatedRect> minRect( contours_blue.size() );
                for( int i = 0; i < contours_blue.size(); i++ )
                {
                    minRect[i] = minAreaRect( Mat(contours_blue[i]) );
                    minRect[i].points( rect_points );
                }
                int j=0;
                for (int i=0;i<4;i++)
                {
                    uu[j]=rect_points[i].x;
                    uu[j+1]=rect_points[i].y;
                    j=j+2;
                }
                j=0;
                for (int i=0;i<4;i++)
                {
                    MyFilledCircle( image, Point(uu[j], uu[j+1]),0,0,255 );
                    j=j+2;
                }

                Moments oMoments = moments(mask);

                double dM01 = oMoments.m01;
                double dM10 = oMoments.m10;
                double dArea = oMoments.m00;

                area = dArea;

                if(dArea >10000)
                {
                    int posX = dM10 / dArea;
                    int posY = dM01/dArea;


                    if(iLastX >= 0 && iLastY >=0 && posY >=0)
                    {
                        line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255),2);

                    }

                    iLastX = posX;
                    iLastY = posY;
                }


                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                mixChannels(&hsv, 1, &hue, 1, ch, 1);

                if( trackObject < 0 )
                {
                    // Object has been selected by user, set up CAMShift search properties once
                    Mat roi(hue, selection), maskroi(mask, selection);
                    calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
                    normalize(hist, hist, 0, 255, NORM_MINMAX);

                    trackWindow = selection;
                    trackObject = 1; // Don't set up again, unless user selects new ROI

                    histimg = Scalar::all(0);
                    int binW = histimg.cols / hsize;
                    Mat buf(1, hsize, CV_8UC3);
                    for( int i = 0; i < hsize; i++ )
                        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                    cvtColor(buf, buf, COLOR_HSV2BGR);

                    for( int i = 0; i < hsize; i++ )
                    {
                        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
                        rectangle( histimg, Point(i*binW,histimg.rows),
                                   Point((i+1)*binW,histimg.rows - val),
                                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
                    }
                }

                // Perform CAMShift
                calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
                backproj &= mask;
                

                RotatedRect trackBox = CamShift(backproj, trackWindow,
                                                TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
                if( trackWindow.area() <= 1 )
                {
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                                       trackWindow.x + r, trackWindow.y + r) &
                            Rect(0, 0, cols, rows);
                    ROS_INFO("out of camera scope");
                }

                if( backprojMode )
                    cvtColor( backproj, image, COLOR_GRAY2BGR );
                ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
            }
        }
        else if( trackObject < 0 )
            paused = false;

        if( selectObject && selection.width > 0 && selection.height > 0 )
        {
            Mat roi(image, selection);
            bitwise_not(roi, roi);
        }

        imshow( "CamShift Demo", image );
        imshow( "Histogram", histimg );

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'b':
            backprojMode = !backprojMode;
            break;
        case 'c':
            trackObject = 0;
            histimg = Scalar::all(0);
            break;
        case 'h':
            showHist = !showHist;
            if( !showHist )
                destroyWindow( "Histogram" );
            else
                namedWindow( "Histogram", 1 );
            break;
        case 'p':
            paused = !paused;
            break;
        default:
            ;
        }

        pt = ptcloud ->at(iLastX, iLastY);
        ROS_INFO("%d , %d ", iLastX, iLastY);
        ROS_INFO("%d , %d ", ptcloud->width, ptcloud->height);


        double depth = double(pt.z);
//        double depth = 0.0;

        float new_depth;
        new_depth = float(depth);

        cout<<"new_depth="<<new_depth<<endl;
        cout << "centroid x" <<iLastX <<endl;
        cout << "centroid y" <<iLastY <<endl;


//        data.detected_point_x.data = iLastX;
//        data.detected_point_y.data = iLastY;
//        data.detected_point_depth.data =(float)pt.z;

        data.detected_point_x.data = iLastX;
        data.detected_point_y.data = iLastY;
        data.detected_point_depth.data =(float)pt.z;
      
        data.detected_point_0.data = (float)uu[0];
        data.detected_point_1.data = (float)uu[1];
        data.detected_point_2.data = (float)uu[2];
        data.detected_point_3.data = (float)uu[3];
        data.detected_point_4.data = (float)uu[4];
        data.detected_point_5.data = (float)uu[5];
        data.detected_point_6.data = (float)uu[6];
        data.detected_point_7.data = (float)uu[7];
        data.area.data = (float)area;




        cout<<"publishing data"<<endl;

        pub.publish(data);

        ros::spinOnce();


    }




    return 0;
}
