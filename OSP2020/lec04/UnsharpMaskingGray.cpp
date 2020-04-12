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

Mat unsharpMask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k);
Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

int main() {
    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat input_gray;
    Mat output;

    cvtColor(input, input_gray, CV_RGB2GRAY);

    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("GrayScale", WINDOW_AUTOSIZE);
    imshow("GrayScale", input_gray);
    
    output = unsharpMask(input_gray, 11, 5, 5, "mirroring", 0.7);
    //Boundary process: zero-paddle, mirroring, adjustkernel
    namedWindow("unsharpMask", WINDOW_AUTOSIZE);
    imshow("unsharpMask", output);

    waitKey(0);

    return 0;
}


Mat unsharpMask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k){
    int row = input.rows;
    int col = input.cols;
    
    Mat lowFilterRes = gaussianfilter(input, n, sigmaT, sigmaS, opt);
    Mat output = Mat::zeros(row, col, input.type());
    
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++){
            float out = (input.at<G>(i, j) -  lowFilterRes.at<G>(i, j) * k)/(1 - k);
            if (out < 0) out = 0;
            else if (out > 255) out = 255;
            
            output.at<G>(i, j) = (G)(out);
        }
    
    return output;
}

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {
    Mat kernel;
    
    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa;
    int tempb;
    float denom;
    float kernelvalue;

    // Initialiazing Kernel Matrix
    kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
    
    denom = 0.0;
    for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
        for (int b = -n; b <= n; b++) {
            float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
            kernel.at<float>(a+n, b+n) = value1;
            denom += value1;
        }
    }

    for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
        for (int b = -n; b <= n; b++) {
            kernel.at<float>(a+n, b+n) /= denom;
        }
    }

    Mat output = Mat::zeros(row, col, input.type());
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {

            if (!strcmp(opt, "zero-paddle")) {
                float sum1 = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++)
                        if ((i + a <= row - 1) && (i + a >= 0) &&
                            (j + b <= col - 1) && (j + b >= 0)) {
                            //if the pixel is not a border pixel
                            kernelvalue = kernel.at<float>(a+n, b+n);
                            sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
                        }
                output.at<G>(i, j) = (G)sum1;
            }
            
            else if (!strcmp(opt, "mirroring")) {
                float sum1 = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++) {
                        if (i + a > row - 1) tempa = i - a;
                        else if (i + a < 0) tempa = -(i + a);
                        else tempa = i + a;
                        
                        if (j + b > col - 1) tempb = j - b;
                        else if (j + b < 0) tempb = -(j + b);
                        else tempb = j + b;
                        
                        kernelvalue = kernel.at<float>(a+n, b+n);
                        sum1 += kernelvalue*(float)(input.at<G>(tempa, tempb));
                    }
                output.at<G>(i, j) = (G)sum1;
            }


            else if (!strcmp(opt, "adjustkernel")) {
                float sum1 = 0.0;
                float sum2 = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++)
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                        kernelvalue = kernel.at<float>(a+n, b+n);
                        sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
                        sum2 += kernelvalue;
                    }
                output.at<G>(i, j) = (G)(sum1/sum2);
            }
        }
    }
    return output;
}
