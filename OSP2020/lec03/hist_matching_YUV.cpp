#include "hist_func.h"
#pragma warning(disable:4996)

using namespace std;

void hist_matching_color(Mat &input, Mat &ref, Mat &matched, G *trans_func);

int main(){
    // Get input and ref images
    Mat input = imread("input.jpg", CV_LOAD_IMAGE_COLOR);
    Mat ref = imread("ref_color.jpg", CV_LOAD_IMAGE_COLOR);

    // Make output image
    Mat matched_YUV;
    cvtColor(input, matched_YUV, CV_RGB2YUV);

    // split each channel(Y, U, V) of matched image
    Mat channels_matched[3];
    split(matched_YUV, channels_matched);
    Mat Y_matched = channels_matched[0];   // U = channels[1], V = channels[2]

    // split each channel(Y, U, V) of ref image
    Mat channels_ref[3];
    split(ref, channels_ref);
    Mat Y_ref = channels_ref[0];

    // PDF or transfer function txt files
    FILE *f_matched_PDF_YUV, *f_PDF_RGB;
    FILE *f_trans_func_matching_YUV;

    float **PDF_RGB = cal_PDF_RGB(input);        // PDF of Input image(RGB) : [L][3]

    f_PDF_RGB = fopen("PDF_RGB.txt", "w+");
    f_matched_PDF_YUV = fopen("matched_PDF_YUV.txt", "w+");
    f_trans_func_matching_YUV = fopen("trans_func_matching_YUV.txt", "w+");

    // transfer function
    G trans_func_matching_YUV[L] = { 0 };

    // histogram matching on Y channel
    hist_matching_color(Y_matched, Y_ref, channels_matched[0], trans_func_matching_YUV);

    // merge Y, U, V channels
    merge(channels_matched, 3, matched_YUV);

    // YUV -> RGB (use "CV_YUV2RGB" flag)
    cvtColor(matched_YUV, matched_YUV, CV_YUV2RGB);

    // equalized PDF (grayscale)
    float **matched_PDF_YUV = cal_PDF_RGB(matched_YUV);

    for (int i = 0; i < L; i++) {
        // write PDF
        fprintf(f_PDF_RGB, "%d\t%f\t%f\t%f\n", i, PDF_RGB[i][0], PDF_RGB[i][1], PDF_RGB[i][2]);
        fprintf(f_matched_PDF_YUV, "%d\t%f\t%f\t%f\n", i, matched_PDF_YUV[i][0], matched_PDF_YUV[i][1], matched_PDF_YUV[i][2]);

        // write transfer functions
        fprintf(f_trans_func_matching_YUV, "%d\t%d\n", i, trans_func_matching_YUV[i]);
    }

    // memory release
    free(PDF_RGB);
    fclose(f_PDF_RGB);
    fclose(f_matched_PDF_YUV);
    fclose(f_trans_func_matching_YUV);

    ////////////////////// Show each image ///////////////////////

    namedWindow("Input", WINDOW_AUTOSIZE);
    imshow("Input", input);

    namedWindow("Matched", WINDOW_AUTOSIZE);
    imshow("Matched", matched_YUV);

    namedWindow("Reference", WINDOW_AUTOSIZE);
    imshow("Reference", ref);

    //////////////////////////////////////////////////////////////

    waitKey(0);
    return 0;

}

//histogram matching for color image
void hist_matching_color(Mat &input, Mat &ref, Mat &matched, G *trans_func){

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
            matched.at<G>(i, j) = trans_func[input.at<G>(i, j)];

    // memory release
    free(CDF_input);
    free(CDF_ref);
}
