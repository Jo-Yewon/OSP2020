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

Mat gaussianfilter_sep_RGB(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

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
    
    output = gaussianfilter_sep_RGB(input, 11, 5, 5, "adjustkernel");
    //Boundary process: zero-paddle, mirroring, adjustkernel

    namedWindow("Gaussian Filter(Seperable)", WINDOW_AUTOSIZE);
    imshow("Gaussian Filter(Seperable)", output);

    waitKey(0);
    return 0;
}


Mat gaussianfilter_sep_RGB(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {
    Mat kernel_t, kernel_s;
    
    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa, tempb;
    float denom;
    float kernelvalue;
    float sum_b, sum_g, sum_r, sum;

    // Initialiazing Kernel Matrix t
    kernel_t = Mat::zeros(1, kernel_size, CV_32F);
    
    denom = 0.0;
    for (int b = -n; b <= n; b++) {
        float value1 = exp(-(pow(b, 2) / (2 * pow(sigmaT, 2))));
        kernel_t.at<float>(0, b+n) = value1;
        denom += value1;
    }
    for (int b = -n; b <= n; b++)
        kernel_t.at<float>(0, b+n) /= denom;
    
    // Initializing Kernel Matrix s
    kernel_s = Mat::zeros(kernel_size, 1, CV_32F);
    
    denom = 0.0;
    for (int a = -n; a <= n; a++) {
        float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))));
        kernel_s.at<float>(a+n, 0) = value1;
        denom += value1;
    }
     for (int a = -n; a <= n; a++)
        kernel_s.at<float>(a+n, 0) /= denom;
    
    Mat intermediate_output = Mat::zeros(row, col, input.type());
    Mat output = Mat::zeros(row, col, input.type());
    
    //Calculate x
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!strcmp(opt, "zero-paddle")) {
                sum_b = sum_g = sum_r = 0.0;
                for (int b = -n; b <= n; b++)
                    if ((j + b <= col - 1) && (j + b >= 0)) {
                        //if the pixel is not a border pixel
                        kernelvalue = kernel_t.at<float>(0, b+n);
                        sum_b += kernelvalue * (float)(input.at<C>(i, j + b)[0]);
                        sum_g += kernelvalue * (float)(input.at<C>(i, j + b)[1]);
                        sum_r += kernelvalue * (float)(input.at<C>(i, j + b)[2]);
                    }
                intermediate_output.at<C>(i, j)[0] = (G)sum_b;
                intermediate_output.at<C>(i, j)[1] = (G)sum_g;
                intermediate_output.at<C>(i, j)[2] = (G)sum_r;
            }
            
            else if (!strcmp(opt, "mirroring")) {
                sum_b = sum_g = sum_r = 0.0;
                for (int b = -n; b <= n; b++) {
                    if (j + b > col - 1) tempb = j - b;
                    else if (j + b < 0) tempb = -(j + b);
                    else tempb = j + b;
                        
                    kernelvalue = kernel_t.at<float>(0, b+n);
                    sum_b += kernelvalue * (float)(input.at<C>(i, tempb)[0]);
                    sum_g += kernelvalue * (float)(input.at<C>(i, tempb)[1]);
                    sum_r += kernelvalue * (float)(input.at<C>(i, tempb)[2]);
                }
                intermediate_output.at<C>(i, j)[0] = (G)sum_b;
                intermediate_output.at<C>(i, j)[1] = (G)sum_g;
                intermediate_output.at<C>(i, j)[2] = (G)sum_r;
            }


            else if (!strcmp(opt, "adjustkernel")) {
                sum_b = sum_g = sum_r = sum = 0.0;
                for (int b = -n; b <= n; b++)
                    if ((j + b <= col - 1) && (j + b >= 0)) {
                        kernelvalue = kernel_t.at<float>(0, b+n);
                        sum_b += kernelvalue * (float)(input.at<C>(i, j + b)[0]);
                        sum_g += kernelvalue * (float)(input.at<C>(i, j + b)[1]);
                        sum_r += kernelvalue * (float)(input.at<C>(i, j + b)[2]);
                        sum += kernelvalue;
                    }
                intermediate_output.at<C>(i, j)[0] = (G)(sum_b / sum);
                intermediate_output.at<C>(i, j)[1] = (G)(sum_g / sum);
                intermediate_output.at<C>(i, j)[2] = (G)(sum_r / sum);
            }
        }
    }
    
    //Calculate y
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!strcmp(opt, "zero-paddle")) {
                sum_b = sum_g = sum_r = 0.0;
                for (int a = -n; a <= n; a++)
                    if ((i + a <= row - 1) && (i + a >= 0)) {
                        //if the pixel is not a border pixel
                        kernelvalue = kernel_s.at<float>(a+n, 0);
                        sum_b += kernelvalue * (float)(intermediate_output.at<C>(i + a, j)[0]);
                        sum_g += kernelvalue * (float)(intermediate_output.at<C>(i + a, j)[1]);
                        sum_r += kernelvalue * (float)(intermediate_output.at<C>(i + a, j)[2]);
                    }
                output.at<C>(i, j)[0] = (G)sum_b;
                output.at<C>(i, j)[1] = (G)sum_g;
                output.at<C>(i, j)[2] = (G)sum_r;
            }
                
            else if (!strcmp(opt, "mirroring")) {
                sum_b = sum_g = sum_r = 0.0;
                for (int a = -n; a <= n; a++){
                    if (i + a > row - 1) tempa = i - a;
                    else if (i + a < 0) tempa = -(i + a);
                    else tempa = i + a;
                            
                    kernelvalue = kernel_s.at<float>(a+n, 0);
                    sum_b += kernelvalue * (float)(intermediate_output.at<C>(tempa, j)[0]);
                    sum_g += kernelvalue * (float)(intermediate_output.at<C>(tempa, j)[1]);
                    sum_r += kernelvalue * (float)(intermediate_output.at<C>(tempa, j)[2]);
                }
                output.at<C>(i, j)[0] = (G)sum_b;
                output.at<C>(i, j)[1] = (G)sum_g;
                output.at<C>(i, j)[2] = (G)sum_r;
            }
    
            else if (!strcmp(opt, "adjustkernel")) {
                sum_b = sum_g = sum_r = sum = 0.0;
                for (int a = -n; a <= n; a++)
                    if ((i + a <= row - 1) && (i + a >= 0)) {
                        kernelvalue = kernel_s.at<float>(a+n, 0);
                        sum_b += kernelvalue * (float)(intermediate_output.at<C>(i + a, j)[0]);
                        sum_g += kernelvalue * (float)(intermediate_output.at<C>(i + a, j)[1]);
                        sum_r += kernelvalue * (float)(intermediate_output.at<C>(i + a, j)[2]);
                        sum += kernelvalue;
                    }
                output.at<C>(i, j)[0] = (G)(sum_b / sum);
                output.at<C>(i, j)[1] = (G)(sum_g / sum);
                output.at<C>(i, j)[2] = (G)(sum_r / sum);
            }
        }
    }
    return output;
}
