#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main() {
    VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
    
    if (!stream1.isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
    }
    
    //unconditional loop
    while (true) {
        Mat cameraFrame;
        stream1.read(cameraFrame);
        imshow("cam", cameraFrame,  aspect=);
        if (waitKey(30) >= 0)
            break;
    }
    return 0;
}
