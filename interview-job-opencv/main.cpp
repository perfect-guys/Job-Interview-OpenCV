//Perfect guys Job Interview project

#include <iostream>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <fstream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

string inputDir = "/usr/local/Cellar/opencv/3.4.1_2/share/OpenCV/haarcascades/";
String face_cascade_name = inputDir + "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = inputDir + "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
int blinkCount = 0;
bool isRectangle;
bool checkAllowed = true;
int flag = 0;
void thresholdAndOpen(Mat src, Mat dst); //threshold and consequent bluring operation
void thresholdInRange(Mat src, Mat dst); //figure out the range of the eye_blink_diff
vector<Rect> detectEyeAndFace(Mat src);


int main(){
    cout<<"Initialize OpenCL runtime..." << "\n";
    cout<<"Blink count"<< "\n";
    
    //load algorithm
    if( !face_cascade.load( face_cascade_name ) ){
        printf("face_cascade_name failed to load...\n");
        getchar();
    }
    if( !eyes_cascade.load( eyes_cascade_name ) ){
        printf("eye_cascade_name failed to load...\n");
        getchar();
    }
    
    VideoCapture cap(0);
    cap.open(0);
    
    if(!cap.isOpened()){
        cout << "fail to open camera" << endl;
        return -1;
    }
    
    Mat frame[2];
    
    
    //  main process
    Rect box;
    
    for(int i=0; waitKey(1) != 27; i=!i){
        
        cap >> frame[i];

        //frame size
        cvtColor(frame[i], frame[i], COLOR_BGR2BGRA);
        pyrDown(frame[i], frame[i]);
        
    
        // count eye blinks
        vector<Rect> eyes = detectEyeAndFace(frame[i]);
        for(size_t j=0; j<eyes.size(); j++){
            rectangle(frame[i], eyes[j], 255, 3);
            isRectangle = true;
        }
        
        if (!isRectangle && checkAllowed == true){
            blinkCount++;
            checkAllowed = false;
        }
        
        isRectangle = false;
        flag++;
        if (flag > 5){
            flag = 0;
            checkAllowed = true;
        }
        
        // int to string for blink count
        stringstream str_blinkCount;
        str_blinkCount << blinkCount;
        string str = str_blinkCount.str();
        String strBlinkCnt = "BLINK COUNT : " + str;
        
       
        // test for eye blink on the display
        Point blinkCountPoint = Point(50, 320);
        int fontFace = FONT_ITALIC;
        double fontScale = 1;
        Scalar fontColor = Scalar(250, 0, 0);
        int fontThinkness = 3;
        cv::putText(frame[i], strBlinkCnt,blinkCountPoint , fontFace, fontScale, fontColor , fontThinkness, true);
        
        // show frame
        imshow("camera", frame[i]);
    }
    
}


void thresholdAndOpen(Mat src, Mat dst){
    threshold(src, dst, 10, 255, THRESH_BINARY);
    int maskSize = 5;
    
    Mat ele = getStructuringElement(MORPH_ELLIPSE, Size(maskSize, maskSize));
    morphologyEx(src, dst, MORPH_OPEN, ele);
}

void thresholdInRange(Mat src, Mat dst){
    inRange(src, 10, 30, dst);
    int maskSize = 5;
    Mat ele = getStructuringElement(MORPH_ELLIPSE, Size(maskSize, maskSize));
    morphologyEx(src, dst, MORPH_OPEN, ele);
}

vector<Rect> detectEyeAndFace(Mat src){
    vector<Rect> faces, eyes;
    
    face_cascade.detectMultiScale( src, faces, 1.1, 2, 0, Size(30, 30));
    
    for( size_t i = 0; i < faces.size(); i++)
    {
        Mat faceROI = src( faces[i] );
        
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0, Size(30, 30));
        
        for( size_t j = 0; j < eyes.size(); j++)
            eyes[j] += faces[i].tl();
    }
    return eyes;
}


