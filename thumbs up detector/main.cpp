//
//  main.cpp
//  get_camera_working
//
//  Created by Christopher Sheehan on 12/06/2018.
//  Copyright © 2018 Christopher Sheehan. All rights reserved.
//

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void help()
{
 cout<< "\n This program... Im suprised you even saw this";
 
}

string cascadeName;
string nestedCascadeName;

int main(int argc, const char** argv) {
    VideoCapture capture;
    Mat frame, image, hand_frame;
    string inputName;

    capture.open(0);
 
    if(!capture.isOpened()) {
    
    cerr << "ERROR! Unable to open camera\n";
    
    }
 
    //GRAB AND SHOW LOOP
 
    cout << "Press any key to terminate" << endl;
 
    for (;;)
    {
    
    capture.read(frame);
    
    if(frame.empty()){
    
    cerr << "Error! blank frame ahoy \n";
    break;
    
    }
    
    
    //how to choose region in cpp
    Point pt1, pt2;
    int bound_box_height, bound_box_width;
    bound_box_height = 400;
    bound_box_width = 400;
    pt1.x = 200;
    pt1.y = 200;
    pt2.x = pt1.x + bound_box_height;
    pt2.y = pt1.y + bound_box_width;
    //Draw the rectangle on the frame
    rectangle(frame, pt1, pt2, CV_RGB(255,0,0), 1);
    //remove frame to process seperatly
    Rect roi;
    roi.x = pt1.x;
    roi.y = pt1.y;
    roi.height = bound_box_height;
    roi.width = bound_box_width;
    hand_frame = frame(roi);
    
    
    //cvtColor(<#InputArray src#>, <#OutputArray dst#>, <#int code#>);
  
    //Mat imCrop = frame(r);
    //
    imshow("Live", frame);
    imshow("hand", hand_frame);
    
    if(waitKey(5) >= 0 ) break;
    
    }

 return 0;
}
