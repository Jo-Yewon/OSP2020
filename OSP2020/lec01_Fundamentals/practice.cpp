//
//  1_practice.cpp
//  opencv_test
//
//  Created by Yewon on 2020/03/25.
//  Copyright © 2020 Yewon. All rights reserved.
//

#include "practice.hpp"

using namespace cv;
using namespace std;

void image_read_write(){
    // Color image to Gray Image
    Mat img = imread("mouse.jpeg", IMREAD_COLOR);
    cvtColor(img, img, COLOR_RGB2GRAY);
    imwrite("mouse_gray.jpeg", img);
}

void image_show(){
    // Show image with new window
    Mat img = imread("mouse.jpeg", IMREAD_COLOR);
    
    if (!img.data){
        cout << "Could not open or find the image" << endl;
        return;
    }
    
    namedWindow("mouse");
    imshow("mouse", img);
    
    waitKey(0);
}

void image_blend(){
    double alpha = 0.5;
    double beta, input;
    Mat img1, img2, dst;
    
    img1 = imread("LinuxLogo.jpg", IMREAD_COLOR);
    img2 = imread("WindowsLogo.jpg", IMREAD_COLOR);
    
    if (img1.empty() || img2.empty()){
        cout << "Could not open or find the image" << endl;
        return;
    }
    
    cout << "Simple Linear Blender" << endl;
    cout << "---------------------" << endl;
    cout << "Enter alpha(0.0-1.0): ";
    cin >> input;
    
    if (0<=input && input<=1)
        alpha = input;
    
    beta = 1.0 - alpha;
    addWeighted(img1, alpha, img2, beta, 0.0, dst);
    
    imshow("Linear blend", dst);
    
    waitKey(0);
}


