#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;
using namespace std;

const String CONTROL_WINDOW_NAME = "Controls";

// Define Hue, Saturation, and Value values for the sliders (trackbars)
const int THRESHOLD_MAX = 256;
RNG rng(12345);

// Define Hue, Saturation, and Value values for the sliders (trackbars)
int h_threshold_min = 0, h_threshold_max = 256;
int s_threshold_min = 0, s_threshold_max = 256;
int v_threshold_min = 0, v_threshold_max = 256;

static void on_trackbar( int, void* ){} // Don't need to do much here yet...

void make_UI(){
    namedWindow(CONTROL_WINDOW_NAME);

    createTrackbar("Min Hue", CONTROL_WINDOW_NAME, &h_threshold_min, THRESHOLD_MAX, on_trackbar);
    createTrackbar("Max Hue", CONTROL_WINDOW_NAME, &h_threshold_max, THRESHOLD_MAX, on_trackbar);
    
    createTrackbar("Min Saturation", CONTROL_WINDOW_NAME, &s_threshold_min, THRESHOLD_MAX, on_trackbar);
    createTrackbar("Max Saturation", CONTROL_WINDOW_NAME, &s_threshold_max, THRESHOLD_MAX, on_trackbar);
    
    createTrackbar("Min Value", CONTROL_WINDOW_NAME, &v_threshold_min, THRESHOLD_MAX, on_trackbar);
    createTrackbar("Max Value", CONTROL_WINDOW_NAME, &v_threshold_max, THRESHOLD_MAX, on_trackbar);
    
}

int main(int argc, char* argv[]){
    VideoCapture capture(0);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    Mat Webcam;
    Mat HSVcam;
    Mat thresholdCam;
    int x_cor = 0, y_cor = 0;
    
    make_UI();
    
    while(true) {
        
        capture.read(Webcam);
        // Convert original webcam images from BGR to HSV space
        cvtColor(Webcam, HSVcam, COLOR_BGR2HSV);
        printf("h_max: %d\n", h_threshold_max);
        printf("h_min: %d\n", h_threshold_min);
        printf("s_max: %d\n", s_threshold_max);
        printf("s_min: %d\n", s_threshold_min);
        printf("v_max: %d\n", v_threshold_max);
        printf("v_min: %d\n", v_threshold_min);
        
        /* for sliders */

        /*
         inRange(HSVcam,Scalar(h_threshold_min, s_threshold_min, v_threshold_min),
         Scalar(h_threshold_max, s_threshold_max, v_threshold_max),
         thresholdCam);
         */
        
        inRange(HSVcam,Scalar(145, 128, 66),
                Scalar(256, 243, 256),
                thresholdCam);
        
        
        /* for drawn red box */
        /*
        inRange(HSVcam,Scalar(0, 101, 95),
                Scalar(256, 180, 139),
                thresholdCam);
        */
        
        /* for red pen */
        /*
        inRange(HSVcam,Scalar(130, 58, 30),
                Scalar(185, 256, 111),
                thresholdCam);
         */
        
        Mat erosionKernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        erode(thresholdCam, thresholdCam, erosionKernel);
        
        
        Mat dilationKernel = getStructuringElement(MORPH_RECT, Size(8, 8));
        dilate(thresholdCam, thresholdCam, dilationKernel);
        
        imshow("Binary threshold (dilation)", thresholdCam);
        
        Mat edges;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        // Detect edges using canny
        cv::Canny(thresholdCam, edges, 100, 200);
        
        findContours(thresholdCam, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        
        Mat drawing = Mat::zeros( edges.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );
            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            
        }

        
        // Lower the refresh rate so it doesn't die...
        waitKey(15);
    }
}