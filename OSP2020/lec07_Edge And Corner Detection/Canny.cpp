#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;

int main() {

    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat output = Mat::zeros(input.rows, input.cols, input.type());

    // check for validation
    if (!input.data) {
        printf("Could not open\n");
        return -1;
    }

    Canny(input, output, 80, 170);
    
    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", input);

    namedWindow("Canny", WINDOW_AUTOSIZE);
    imshow("Canny", output);

    waitKey(0);

    return 0;
}



