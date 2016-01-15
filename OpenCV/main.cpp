#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;

const String CONTROL_WINDOW_NAME = "Controls";

// Define Hue, Saturation, and Value values for the sliders (trackbars)
const int H_MAX = 256;
const int S_MAX = 256;
const int V_MAX = 256;

// Define Hue, Saturation, and Value values for the sliders (trackbars)
int h_threshold_min = 0, h_threshold_max = 256;
int s_threshold_min = 0, s_threshold_max = 256;
int v_threshold_min = 0, v_threshold_max = 256;

static void on_trackbar( int, void* ){} // Don't need to do much here yet...

void make_UI(){
    namedWindow(CONTROL_WINDOW_NAME);
    
    createTrackbar("Min Hue", CONTROL_WINDOW_NAME, &h_threshold_min, H_MAX, on_trackbar);
    createTrackbar("Max Hue", CONTROL_WINDOW_NAME, &h_threshold_max, H_MAX, on_trackbar);
    
    createTrackbar("Min Saturation", CONTROL_WINDOW_NAME, &s_threshold_min, S_MAX, on_trackbar);
    createTrackbar("Max Saturation", CONTROL_WINDOW_NAME, &s_threshold_max, S_MAX, on_trackbar);
    
    createTrackbar("Min Value", CONTROL_WINDOW_NAME, &v_threshold_min, V_MAX, on_trackbar);
    createTrackbar("Max Value", CONTROL_WINDOW_NAME, &v_threshold_max, V_MAX, on_trackbar);
    
}

int main(int argc, char* argv[]){
    VideoCapture capture(0);
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
        
        
         inRange(HSVcam,Scalar(h_threshold_min, s_threshold_min, v_threshold_min),
         Scalar(h_threshold_max, s_threshold_max, v_threshold_max),
         thresholdCam);
        
        
        /*
        inRange(HSVcam,Scalar(0, 101, 95),
                Scalar(256, 180, 139),
                thresholdCam);
        */
        
        
        imshow("HSV", HSVcam);
        imshow("RAW", Webcam);
        imshow("Binary threshold", thresholdCam);
        
        // Lower the refresh rate so it doesn't die...
        waitKey(15);
    }
}