#include "hist_func.h"
#pragma warning(disable:4996)

using namespace std;

void hist_matching(Mat &input, Mat &ref, Mat &equalized, G *trans_func);

int main(){
    // Get input image
    Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR);
    Mat input_gray;
    cvtColor(input, input_gray, CV_RGB2GRAY);    // convert RGB to Grayscale

    // Get reference image
    Mat ref_input = imread("ref_gray.jpg", CV_LOAD_IMAGE_COLOR);
    Mat ref_gray;
    cvtColor(ref_input, ref_gray, CV_RGB2GRAY);    // convert RGB to Grayscale

    // Make output image
    Mat equalized = input_gray.clone();

    // PDF or transfer function txt files
    FILE *f_PDF;
    FILE *f_equalized_PDF_gray;
    FILE *f_trans_func_matching;

    f_PDF = fopen("PDF.txt", "w+");
    f_equalized_PDF_gray = fopen("equalized_PDF_gray.txt", "w+");
    f_trans_func_matching = fopen("trans_func_matching.txt", "w+");

    // PDF of Input image(Grayscale) : [L]
    float *PDF = cal_PDF(input_gray);

    // transfer function
    G trans_func_matching[L] = { 0 };

    hist_matching(input_gray, ref_gray, equalized, trans_func_matching);

    // equalized PDF (grayscale)
    float *equalized_PDF_gray = cal_PDF(equalized);

    for (int i = 0; i < L; i++) {
        // write PDF
        fprintf(f_PDF, "%d\t%f\n", i, PDF[i]);
        fprintf(f_equalized_PDF_gray, "%d\t%f\n", i, equalized_PDF_gray[i]);

        // write transfer functions
        fprintf(f_trans_func_matching, "%d\t%d\n", i, trans_func_matching[i]);
    }

    // memory release
    free(PDF);
    fclose(f_PDF);
    fclose(f_equalized_PDF_gray);
    fclose(f_trans_func_matching);

    ////////////////////// Show each image ///////////////////////

    namedWindow("Input", WINDOW_AUTOSIZE);
    imshow("Input", input_gray);

    namedWindow("Equalized", WINDOW_AUTOSIZE);
    imshow("Equalized", equalized);

    namedWindow("Reference", WINDOW_AUTOSIZE);
    imshow("Reference", ref_gray);

    //////////////////////////////////////////////////////////////

    waitKey(0);
    return 0;

}

//histogram matching in grayScale image
void hist_matching(Mat &input, Mat &ref, Mat &equalized, G *trans_func){

    G s;
    int left, right;

    // Compute transfer function T for histogram equalization of an input image
    float *CDF_input = cal_CDF(input);    // CDF of Input image(Grayscale) : [L]
    G trans_func_t[L] = { 0 };
    for (int i = 0; i < L; i++)
        trans_func_t[i] = (G)((L - 1) * CDF_input[i]);

    // Compute transfer fuction G^-1 for histogram equalization of a reference image.
    float *CDF_ref = cal_CDF(ref);

    bool check[L] = { false }; // Check update
    G trans_func_g_reverse[L] = { 0 };

    for (int i = 0; i < L; i++){
        s = (G)((L - 1) * CDF_ref[i]);
        trans_func_g_reverse[s] = i;
        check[s] = true;
    }

    // Compute not-updated things using neighboring one.
    for (int i = 0; i < L; i++){
        if (!check[i]){
            left = i - 1;
            right = i + 1;
            while (0 < left or right < L){
                if (0 < left and check[left]){
                    trans_func_g_reverse[i] = trans_func_g_reverse[left];
                    break;
                }
                if (right < L and check[right]){
                    trans_func_g_reverse[i] = trans_func_g_reverse[right];
                    break;
                }
                left -= 1;
                right += 1;
            }
        }
    }

    // Compute final transfer funtion with T and G^-1
    for (int i = 0; i < L; i++)
        trans_func[i] = trans_func_g_reverse[trans_func_t[i]];

    // Perform the transfer function
    for (int i = 0; i < input.rows; i++)
    for (int j = 0; j < input.cols; j++)
        equalized.at<G>(i, j) = trans_func[input.at<G>(i, j)];
}
