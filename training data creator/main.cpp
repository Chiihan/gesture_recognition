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
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <limits.h>
#include <unistd.h>

using namespace std;
using namespace cv;

//background seg variables
Mat fgMaskMOG2;
Ptr<BackgroundSubtractor> pMOG2;


static void help()
{
 cout<< "\n This program... Im suprised you even saw this";
 
}
// Global Variables
string cascadeName;
string nestedCascadeName;
Mat erosion_dst, dilation_dst, dwn_smple;
//2 3
int erosion_elem = 2;
int erosion_size = 1;
int dilation_elem = 2;
int dilation_size = 4;
int const max_elem = 2;
int const max_kernel_size = 21;
int record_mode = 1;
std::string gesture;

int record_frame_count = 51;
std::string image_text2 = "Recording";




int main(int argc, const char** argv) {
    VideoCapture capture;
    Mat frame, image, hand_frame;
    string inputName;
    Point pt1, pt2;
    int bound_box_height, bound_box_width;
    bound_box_height = 400;
    bound_box_width = 400;
    pt1.x = 200;
    pt1.y = 200;
    pt2.x = pt1.x + bound_box_height;
    pt2.y = pt1.y + bound_box_width;
    Rect roi;
    roi.x = pt1.x;
    roi.y = pt1.y;
    roi.height = bound_box_height;
    roi.width = bound_box_width;
 
    if(argc > 1) gesture = argv[1];
 
    cout << "This will record " << gesture << " gesture training data";

 
    namedWindow("Live");
    namedWindow("hand");
    moveWindow("backGrndExpriment", 1000, 500);
    namedWindow("backGrndExpriment");
    moveWindow("backGrndExpriment", 1000, 100);
    namedWindow("hand2");
    pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach


    capture.open(0);
 
    if(!capture.isOpened()) {
    
    cerr << "ERROR! Unable to open camera\n";
    
    }
 
    //GRAB AND SHOW LOOP
 
    cout << "Press any key to terminate" << endl;
 
    for (;;)
    {
    
    capture.read(frame);
    
    if(frame.empty())
    {
    
    cerr << "Error! blank frame ahoy \n";
    break;
    
    }
    
    //draw rectangle on frame
    rectangle(frame, pt1, pt2, CV_RGB(255,0,0), 1);
    hand_frame = frame(roi);
    
     //update background model
     pMOG2 -> apply(hand_frame, fgMaskMOG2);

    //erosion & dilate section
    int erosion_type = 1;
    if( erosion_elem == 0 ){erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS;}
    else if( erosion_elem == 2 ){ erosion_type = MORPH_ELLIPSE; }
    
    Mat elementE = getStructuringElement( erosion_type,
                                         Size( 2*erosion_size + 1, 2*erosion_size + 1),
                                         Point( erosion_size, erosion_size ));
     
     erode(fgMaskMOG2, erosion_dst, elementE);
     
     threshold( erosion_dst, erosion_dst, 100, 255, THRESH_BINARY);
     
     int dilation_type = 1;
     if( dilation_type == 0 ){ dilation_type = MORPH_RECT; }
     else if( dilation_elem == 1 ) {dilation_type = MORPH_CROSS; }
     else if( dilation_elem == 2 ) { dilation_type = MORPH_ELLIPSE; }
     
     Mat elementD = getStructuringElement( dilation_type,
                                          Size( 2*dilation_size + 1, 2*dilation_size + 1 ),
                                          Point( dilation_size, dilation_size));
     
    dilate( erosion_dst, dilation_dst, elementD);
    
    pyrDown(dilation_dst, dwn_smple, Size(dilation_dst.cols / 2, dilation_dst.rows / 2));
    pyrDown(dwn_smple, dwn_smple, Size(dwn_smple.cols / 2, dwn_smple.rows / 2));
    //resize(dilation_dst, dwn_smple, Size(15, 15));
    //cout << dwn_smple.cols << "  " << dwn_smple.rows;
    std::string argv_str(argv[0]);
    ////std::string base = argv_str.substr(0, argv_str.find_last_of("/"));
    //cout << "\n" << base << "\n";
    cv::putText(frame, gesture, cv::Point(250,250), FONT_HERSHEY_COMPLEX,
     1.0, Scalar(255,255,255),
     1);
     if( record_frame_count < 50 )
     {

     
     std::stringstream output_file_name;
     output_file_name << "./training_data/" << "training_data_" << gesture << "_" << record_frame_count << ".jpeg" ;
     std::string output_file_string = output_file_name.str();
     
     cv::putText(frame, image_text2, cv::Point(500,500), FONT_HERSHEY_COMPLEX,
     3.0, Scalar(255,255,255),
     1);
     imwrite(output_file_string, dwn_smple);
     
     cout << "\n" << output_file_string << "\n";
     }
     
    imshow("Live", frame);
    imshow("backGrndExpriment", fgMaskMOG2);
    imshow("hand", erosion_dst);
    imshow("hand2", dwn_smple);
    cvtColor(hand_frame, hand_frame, CV_RGB2HSV);
    
    cout<<waitKey(5)<<"\n";
    if(waitKey(5) == 114 ){
    record_frame_count = 0;
    }
    
    //cout<<"\n record_frame_count ="<<record_frame_count<<"\n";
    record_frame_count++;
    


    if(waitKey(5) == 113 ) break;
    
    }

 return 0;
}

