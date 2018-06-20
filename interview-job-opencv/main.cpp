#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

// algorithm for detecting eyes
string inputDir = "/usr/local/Cellar/opencv/3.4.1_2/share/OpenCV/haarcascades/";
String face_cascade_name = inputDir + "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = inputDir + "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

// variables for  counting blink

// declare function
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
    
    //open camera
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened()){
        cout << "fail to open camera" << endl;
        return -1;
    }
    
    Mat frame[2];
    
    
    //main process
    Rect box;
    
    
    vector<Rect> eyes;
    int blinkCnt=0;
    bool flag = false;
    
    for(int i=0; waitKey(1) != 27; i=!i){
        cap >> frame[i];

        //frame size
        cvtColor(frame[i], frame[i], COLOR_BGR2BGRA);
        pyrDown(frame[i], frame[i]);
        
    
        // count eye blinks
        eyes = detectEyeAndFace(frame[i]);
        
        //when opened eye
        if(eyes.size()==2){
            flag = true;
        }else if(eyes.size()!=2){
            if(flag){
                blinkCnt++;
                flag = false;
            }
        }
        
        //test
        cout<< blinkCnt <<endl;
        
        
    
        for(size_t j=0; j<eyes.size(); j++){
            rectangle(frame[i], eyes[j], 255, 3);
        }
        
        
        // int to string for blink count
        stringstream ss;
        ss << blinkCnt;
        string str = ss.str();
        String strBlinkCnt = "BLINK COUNT : " + str;
        
       
        // text for eye blink on the display
        Point blinkCountPoint = Point(50, 320);
        int fontFace = FONT_ITALIC;
        double fontScale = 0.8;
        Scalar fontColor = Scalar(500, 0, 0);
        int fontThinkness = 2;
        cv::putText(frame[i], strBlinkCnt,blinkCountPoint , fontFace, fontScale, fontColor , fontThinkness, false);
        
        ss << rand() % 100;
        string str02 = ss.str();
        String strConcentration = "Concentration : " + str02;
        
        
        //text for concentration on the display
        Point concentrationPoint = Point(380, 320);
        cv::putText(frame[i], strConcentration, concentrationPoint , fontFace, fontScale, fontColor , fontThinkness, false);
        
        // show frame
        imshow("camera", frame[i]);
    }
    
}


vector<Rect> detectEyeAndFace(Mat src){
    vector<Rect> faces, eyes;
    
    face_cascade.detectMultiScale(src, faces);
    
    for( size_t i = 0; i < faces.size(); i++){
        Mat faceROI = src( faces[i] );
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0, Size(30, 30));
        for( size_t j = 0; j < eyes.size(); j++)
            eyes[j] += faces[i].tl();
    }
    
    return eyes;
}


