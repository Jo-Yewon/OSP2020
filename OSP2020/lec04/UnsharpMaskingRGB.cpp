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

Mat unsharpMask_RGB(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k);
Mat gaussianfilter_RGB(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

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
    
    output = unsharpMask_RGB(input, 11, 5, 5, "mirroring", 0.7);
    //Boundary process: zero-paddle, mirroring, adjustkernel
    namedWindow("unsharpMask", WINDOW_AUTOSIZE);
    imshow("unsharpMask", output);

    waitKey(0);

    return 0;
}


Mat unsharpMask_RGB(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k){
    int row = input.rows;
    int col = input.cols;
    float out;
    
    Mat lowFilterRes = gaussianfilter_RGB(input, n, sigmaT, sigmaS, opt);
    Mat output = Mat::zeros(row, col, input.type());
    
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            for (int channel = 0; channel < 3; channel++){
                out = (input.at<C>(i, j)[channel] -  lowFilterRes.at<C>(i, j)[channel] * k)/(1 - k);
                if (out < 0) out = 0;
                else if (out > 255) out = 255;
                output.at<C>(i, j)[channel] = (G)(out);
            }
    
    return output;
}

Mat gaussianfilter_RGB(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {
    Mat kernel;
    
    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa, tempb;
    float denom;
    float kernelvalue;
    float sum1_b, sum1_g, sum1_r, sum;

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
            sum1_b = sum1_g = sum1_r = 0.0;

            if (!strcmp(opt, "zero-paddle")) {
                sum1_b = sum1_g = sum1_r = 0.0;
                
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++)
                        if ((i + a <= row - 1) && (i + a >= 0) &&
                            (j + b <= col - 1) && (j + b >= 0)) {
                            //if the pixel is not a border pixel
                            kernelvalue = kernel.at<float>(a+n, b+n);
                            sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
                            sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
                            sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
                        }
                output.at<C>(i, j)[0] = (G)sum1_b;
                output.at<C>(i, j)[1] = (G)sum1_g;
                output.at<C>(i, j)[2] = (G)sum1_r;
            }
            
            else if (!strcmp(opt, "mirroring")) {
                sum1_b = sum1_g = sum1_r = 0.0;
                
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++) {
                        if (i + a > row - 1) tempa = i - a;
                        else if (i + a < 0) tempa = -(i + a);
                        else tempa = i + a;
                        
                        if (j + b > col - 1) tempb = j - b;
                        else if (j + b < 0) tempb = -(j + b);
                        else tempb = j + b;
                        
                        kernelvalue = kernel.at<float>(a+n, b+n);
                        sum1_b += kernelvalue * (float)(input.at<C>(tempa, tempb)[0]);
                        sum1_g += kernelvalue * (float)(input.at<C>(tempa, tempb)[1]);
                        sum1_r += kernelvalue * (float)(input.at<C>(tempa, tempb)[2]);
                    }
                output.at<C>(i, j)[0] = (G)sum1_b;
                output.at<C>(i, j)[1] = (G)sum1_g;
                output.at<C>(i, j)[2] = (G)sum1_r;
            }


            else if (!strcmp(opt, "adjustkernel")) {
                sum1_b = sum1_g = sum1_r = sum = 0.0;
                
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++)
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                        kernelvalue = kernel.at<float>(a+n, b+n);
                        sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
                        sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
                        sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
                        sum += kernelvalue;
                    }
                output.at<C>(i, j)[0] = (G)(sum1_b / sum);
                output.at<C>(i, j)[1] = (G)(sum1_g / sum);
                output.at<C>(i, j)[2] = (G)(sum1_r / sum);
            }
        }
    }
    return output;
}
