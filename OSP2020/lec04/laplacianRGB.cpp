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

Mat laplacianfilter_RGB(const Mat input);

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
    output = laplacianfilter_RGB(input); //Boundary process: zero-paddle, mirroring, adjustkernel

    namedWindow("Laplacian", WINDOW_AUTOSIZE);
    imshow("Laplacian", output);

    waitKey(0);
    return 0;
}


Mat laplacianfilter_RGB(const Mat input) {

    Mat kernel;

    int row = input.rows;
    int col = input.cols;
    int tempa, tempb, sum_b, sum_g, sum_r;
    int n = 1; // Sobel Filter Kernel N
    const int c = 6;

    Mat L = Mat::zeros(3, 3, CV_32F);
    L.at<int>(0, 1) = L.at<int>(1, 0) = L.at<int>(1, 2) = L.at<int>(2, 1) = 1;
    L.at<int>(1, 1) = -4;

    Mat output = Mat::zeros(row, col, input.type());

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sum_b = sum_g = sum_r = 0;
            
            for (int a = -n; a <= n; a++) {
                for (int b = -n; b <= n; b++) {
                    // Use mirroring boundary process
                    if (i + a > row - 1) tempa = i - a;
                    else if (i + a < 0) tempa = -(i + a);
                    else tempa = i + a;
                    
                    if (j + b > col - 1) tempb = j - b;
                    else if (j + b < 0) tempb = -(j + b);
                    else tempb = j + b;
                    
                    sum_b += input.at<C>(tempa, tempb)[0] * L.at<int>(a+n, b+n);
                    sum_g += input.at<C>(tempa, tempb)[1] * L.at<int>(a+n, b+n);
                    sum_r += input.at<C>(tempa, tempb)[2] * L.at<int>(a+n, b+n);
                }
            }
            output.at<C>(i, j)[0] = (G)(c * abs(sum_b));
            output.at<C>(i, j)[1] = (G)(c * abs(sum_g));
            output.at<C>(i, j)[2] = (G)(c * abs(sum_r));
        }
    }
    return output;
}
