# Pixel

## PDF/CDF of Image

- hist_func.h  
- PDF_CDF.cpp

To use, place your image in the same directory with exec files.

In this code, it calculates PDF(Probability Density Function) and CDF(Cumulative Distribution Function) of image as text files.

<br>

## Histogram Stretching

- hist_func.h  
- hist_stitching.cpp  

To use, place your image in the same directory with exec files. 

This code increase the visibility by stretching the histogram of the image.

```void linear_stretching(Mat &input, Mat &stretched, G *trans_func, G x1, G x2, G y1, G y2)```: trans function is defined linearly as three straight lines. x1, x2, y1, y2 denote the point where the straight lines meet.


<img src=./original.png width="480"> 

<img src=./stretch.png width="480"> 


<br>

## Histogram Equalization

- hist_func.h  
- hist_eq.cpp  
- hist_eq_RGB.cpp (Intentional wrong result)  
- hist_eq_YUV.cpp

To use, place your image in the same directory with exec files. 

This code increase the visibility of the image by equalizing histogram of it. Also, the code that performs equlization for each RGB is intended to be wrong. For equal results, when equalization is applied to a color image, it must be applied after conversion to YUV.

```void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF)```


<img src=./original_rgb.png width="480"> 

<img src=./equalized_yuv.png width="480"> 


<br>

## Histogram Matching

- hist_func.h  
- hist_matching.cpp  
- hist_matching_YUV.cpp

To use, place your input image and reference image in the same directory with exec files. 

The purpose of this code is to adjust the histogram of input image similar to that of reference image.

```void hist_matching(Mat &input, Mat &ref, Mat &matched, G *trans_func)```


<img src=./original_rgb.png width="480"> 

<img src=./ref.png width="480"> 

<img src=./matched.png width="480"> 