#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#define IM_TYPE    CV_8UC3

using namespace cv;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat sobelfilter_RGB(const Mat input);

int main() {

    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat output;

    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", input);
    output = sobelfilter_RGB(input); //Boundary process: zero-paddle, mirroring, adjustkernel

    namedWindow("Sobel Filter", WINDOW_AUTOSIZE);
    imshow("Sobel Filter", output);

    waitKey(0);
    return 0;
}


Mat sobelfilter_RGB(const Mat input) {

    Mat kernel;

    int row = input.rows;
    int col = input.cols;
    int tempa, tempb;
    float ix_b, iy_b, ix_g, iy_g, ix_r, iy_r;
    int n = 1; // Sobel Filter Kernel N
    const float c = 0.8;

    // Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
    //Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)
    Mat Sx = Mat::zeros(3, 3, CV_32F);
    Sx.at<int>(0, 0) = Sx.at<int>(2, 0) = -1;
    Sx.at<int>(1, 0) = -2;
    Sx.at<int>(0, 2) = Sx.at<int>(2, 2) = 1;
    Sx.at<int>(1, 2) = 2;
    
    Mat Sy = Mat::zeros(3, 3, CV_32F);
    Sy.at<int>(0, 0) = Sy.at<int>(0, 2) = -1;
    Sy.at<int>(2, 0) = Sy.at<int>(2, 2) = 1;
    Sy.at<int>(0, 1) = -2;
    Sy.at<int>(2, 1) = 2;

    Mat output = Mat::zeros(row, col, input.type());

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            ix_b = iy_b = ix_g = iy_g = ix_r = iy_r = 0;
            
            for (int a = -n; a <= n; a++) {
                for (int b = -n; b <= n; b++) {
                    // Use mirroring boundary process
                    if (i + a > row - 1) tempa = i - a;
                    else if (i + a < 0) tempa = -(i + a);
                    else tempa = i + a;
                    
                    if (j + b > col - 1) tempb = j - b;
                    else if (j + b < 0) tempb = -(j + b);
                    else tempb = j + b;
                    
                    ix_b += input.at<C>(tempa, tempb)[0] * Sx.at<int>(a+n, b+n);
                    ix_g += input.at<C>(tempa, tempb)[1] * Sx.at<int>(a+n, b+n);
                    ix_r += input.at<C>(tempa, tempb)[2] * Sx.at<int>(a+n, b+n);
                    
                    iy_b += input.at<C>(tempa, tempb)[0] * Sy.at<int>(a+n, b+n);
                    iy_g += input.at<C>(tempa, tempb)[1] * Sy.at<int>(a+n, b+n);
                    iy_r += input.at<C>(tempa, tempb)[2] * Sy.at<int>(a+n, b+n);
                }
            }
            output.at<C>(i, j)[0] = (G)(c * sqrt(ix_b * ix_b + iy_b * iy_b));
            output.at<C>(i, j)[1] = (G)(c * sqrt(ix_g * ix_g + iy_g * iy_g));
            output.at<C>(i, j)[2] = (G)(c * sqrt(ix_r * ix_r + iy_b * iy_r));
        }
    }
    return output;
}
