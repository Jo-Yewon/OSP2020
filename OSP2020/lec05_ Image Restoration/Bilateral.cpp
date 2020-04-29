#include <opencv2/opencv.hpp>
#include <stdio.h>

#define IM_TYPE    CV_64FC3

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


Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);
Mat Bilateralfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char *opt);
Mat Bilateralfilter_RGB(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char* opt);

int main() {
    
    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat input_gray;
    
    // check for validation
    if (!input.data) {
        printf("Could not open\n");
        return -1;
    }
    
    cvtColor(input, input_gray, CV_RGB2GRAY);    // convert RGB to Grayscale
    
    // 8-bit unsigned char -> 64-bit floating point
    input.convertTo(input, CV_64FC3, 1.0 / 255);
    input_gray.convertTo(input_gray, CV_64F, 1.0 / 255);
    
    // Add noise to original image
    Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
    Mat noise_RGB = Add_Gaussian_noise(input, 0, 0.1);
    
    // Denoise, using gaussian filter
    Mat Denoised_Gray = Bilateralfilter_Gray(noise_Gray, 3, 10, 10, 0.15, "adjustkernel");
    Mat Denoised_RGB = Bilateralfilter_RGB(noise_RGB, 3, 10, 10, 0.15, "adjustkernel");
    
    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    imshow("Grayscale", input_gray);
    
    namedWindow("RGB", WINDOW_AUTOSIZE);
    imshow("RGB", input);
    
    namedWindow("Gaussian Noise (Grayscale)", WINDOW_AUTOSIZE);
    imshow("Gaussian Noise (Grayscale)", noise_Gray);
    
    namedWindow("Gaussian Noise (RGB)", WINDOW_AUTOSIZE);
    imshow("Gaussian Noise (RGB)", noise_RGB);
    
    namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
    imshow("Denoised (Grayscale)", Denoised_Gray);
    
    namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
    imshow("Denoised (RGB)", Denoised_RGB);
    
    waitKey(0);
    
    return 0;
}

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma) {
    
    Mat NoiseArr = Mat::zeros(input.rows, input.cols, input.type());
    RNG rng;
    rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);
    
    add(input, NoiseArr, NoiseArr);
    
    return NoiseArr;
}

Mat Bilateralfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char *opt) {
    Mat kernel_S;
    
    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa, tempb;
    double kernelvalue_S, kernelvalue_I, kernelvalue, sum1, sum2;
    
    // Initialiazing Kernel Matrix for spatial
    kernel_S = Mat::zeros(kernel_size, kernel_size, CV_32F);
    
    for (int a = -n; a <= n; a++)
        for (int b = -n; b <= n; b++)
            kernel_S.at<float>(a+n, b+n) = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
    
    Mat output = Mat::zeros(row, col, input.type());
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!strcmp(opt, "zero-paddle")) {
                sum1 = sum2 = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++){
                        kernelvalue_S = kernel_S.at<float>(a+n, b+n);
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            //if the pixel is not a border pixel
                            kernelvalue_I = exp(-(pow(input.at<G>(i, j) - input.at<G>(i + a, j + b), 2)) / (2 * pow(sigmaR, 2)));
                            kernelvalue = kernelvalue_S * kernelvalue_I;
                            sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
                            sum2 += kernelvalue;
                        }
                        else{
                            //if the pixel is a border pixel
                            kernelvalue_I = exp(-(pow(input.at<G>(i, j), 2)) / (2 * pow(sigmaR, 2)));
                            kernelvalue = kernelvalue_S * kernelvalue_I;
                            sum2 += kernelvalue;
                        }
                    }
                output.at<G>(i, j) = (G)(sum1/sum2);
            }
            
            else if (!strcmp(opt, "mirroring")) {
                sum1 = sum2 = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++) {
                        if (i + a > row - 1) tempa = i - a;
                        else if (i + a < 0) tempa = -(i + a);
                        else tempa = i + a;
                        
                        if (j + b > col - 1) tempb = j - b;
                        else if (j + b < 0) tempb = -(j + b);
                        else tempb = j + b;
                        
                        kernelvalue_S = kernel_S.at<float>(a + n, b + n);
                        kernelvalue_I = exp(-(pow(input.at<G>(i, j) - input.at<G>(tempa, tempb), 2)) / (2 * pow(sigmaR, 2)));
                        kernelvalue = kernelvalue_S * kernelvalue_I;
                        sum1 += kernelvalue * (float)(input.at<G>(tempa, tempb));
                        sum2 += kernelvalue;
                    }
                output.at<G>(i, j) = (G)(sum1/sum2);
            }
            
            
            else if (!strcmp(opt, "adjustkernel")) {
                sum1 = sum2 = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++)
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            //if the pixel is not a border pixel
                            kernelvalue_S = kernel_S.at<float>(a+n, b+n);
                            kernelvalue_I = exp(-(pow(input.at<G>(i, j) - input.at<G>(i + a, j + b), 2)) / (2 * pow(sigmaR, 2)));
                            kernelvalue = kernelvalue_S * kernelvalue_I;
                            sum1 += kernelvalue * (float)(input.at<G>(i + a, j + b));
                            sum2 += kernelvalue;
                        }
                output.at<G>(i, j) = (G)(sum1/sum2);
            }
        }
    }
    return output;
}

Mat Bilateralfilter_RGB(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char *opt) {
    Mat kernel_S;
    
    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int tempa, tempb;
    double kernelvalue_S, kernelvalue_I, kernelvalue, sum1[3], sum2[3];
    
    // Initialiazing Kernel Matrix for spatial
    kernel_S = Mat::zeros(kernel_size, kernel_size, CV_32F);
    
    for (int a = -n; a <= n; a++)
        for (int b = -n; b <= n; b++)
            kernel_S.at<float>(a+n, b+n) = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
    
    Mat output = Mat::zeros(row, col, input.type());
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!strcmp(opt, "zero-paddle")) {
                sum1[0] = sum1[1] = sum1[2] = 0.0;
                sum2[0] = sum2[1] = sum2[2] = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++){
                        kernelvalue_S = kernel_S.at<float>(a+n, b+n);
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)){
                            //if the pixel is not a border pixel
                            for (int channel = 0; channel < 3; channel++){
                                kernelvalue_I = exp(-(pow(input.at<C>(i, j)[channel] - input.at<C>(i + a, j + b)[channel], 2)) / (2 * pow(sigmaR, 2)));
                                kernelvalue = kernelvalue_S * kernelvalue_I;
                                sum1[channel] += kernelvalue * (float)(input.at<C>(i + a, j + b)[channel]);
                                sum2[channel] += kernelvalue;
                            }
                        }
                        else{
                            //if the pixel is a border pixel
                            for (int channel = 0; channel < 3; channel++){
                                kernelvalue_I = exp(-(pow(input.at<C>(i, j)[channel], 2)) / (2 * pow(sigmaR, 2)));
                                kernelvalue = kernelvalue_S * kernelvalue_I;
                                sum2[channel] += kernelvalue;
                            }
                        }
                    }
                output.at<C>(i, j)[0] = (G)(sum1[0]/sum2[0]);
                output.at<C>(i, j)[1] = (G)(sum1[1]/sum2[1]);
                output.at<C>(i, j)[2] = (G)(sum1[2]/sum2[2]);
            }
            
            else if (!strcmp(opt, "mirroring")) {
                sum1[0] = sum1[1] = sum1[2] = 0.0;
                sum2[0] = sum2[1] = sum2[2] = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++) {
                        if (i + a > row - 1) tempa = i - a;
                        else if (i + a < 0) tempa = -(i + a);
                        else tempa = i + a;
                        
                        if (j + b > col - 1) tempb = j - b;
                        else if (j + b < 0) tempb = -(j + b);
                        else tempb = j + b;
                        
                        kernelvalue_S = kernel_S.at<float>(a + n, b + n);
                        for (int channel = 0; channel < 3; channel++){
                            kernelvalue_I = exp(-(pow(input.at<C>(i, j)[channel] - input.at<C>(tempa, tempb)[channel], 2)) / (2 * pow(sigmaR, 2)));
                            kernelvalue = kernelvalue_S * kernelvalue_I;
                            sum1[channel] += kernelvalue * (float)(input.at<C>(i + a, j + b)[channel]);
                            sum2[channel] += kernelvalue;
                        }
                    }
                output.at<C>(i, j)[0] = (G)(sum1[0]/sum2[0]);
                output.at<C>(i, j)[1] = (G)(sum1[1]/sum2[1]);
                output.at<C>(i, j)[2] = (G)(sum1[2]/sum2[2]);
            }
            
            
            else if (!strcmp(opt, "adjustkernel")) {
                sum1[0] = sum1[1] = sum1[2] = 0.0;
                sum2[0] = sum2[1] = sum2[2] = 0.0;
                for (int a = -n; a <= n; a++)
                    for (int b = -n; b <= n; b++)
                        if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
                            //if the pixel is not a border pixel
                            kernelvalue_S = kernel_S.at<float>(a+n, b+n);
                            for (int channel = 0; channel < 3; channel++){
                                kernelvalue_I = exp(-(pow(input.at<C>(i, j)[channel] - input.at<C>(i + a, j + b)[channel], 2)) / (2 * pow(sigmaR, 2)));
                                kernelvalue = kernelvalue_S * kernelvalue_I;
                                sum1[channel] += kernelvalue * (float)(input.at<C>(i + a, j + b)[channel]);
                                sum2[channel] += kernelvalue;
                            }
                        }
                output.at<C>(i, j)[0] = (G)(sum1[0]/sum2[0]);
                output.at<C>(i, j)[1] = (G)(sum1[1]/sum2[1]);
                output.at<C>(i, j)[2] = (G)(sum1[2]/sum2[2]);
            }
        }
    }
    return output;
}
