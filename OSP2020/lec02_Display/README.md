# Image displaying

## Image rotation

- rotate.cpp  

To use, place your image in the same directory with exec files.

```Mat myrotate(const Mat input, float angle, const char* opt)```: "input" is input image and "angle" is the angle at which you want to rotate the input image. Using "opt", you can select method for rotation.(bilinear or nearest)

<img src=./rotate1.png width="480"> 

<br>

## Image stitching

- stitching.cpp  

To use, place your image in the same directory with exec files. And in this code, you must know the corresponding pixels between the images. You should include them on main, in variables "ptl_x", "ptl_y", "ptr_x", "ptr_y".


```Mat cal_affine(int ptl_x[], int ptl_y[], int ptr_x[], int ptr_y[], int number_of_points)```: In this function, it calculates affine matrix between two images using corresponding pixels of them.

```void blend_stitching(const Mat I1, const Mat I2, Mat &I_f, int diff_x, int diff_y, float alpha)```: it stitches two images(I1, I2) and the result is on I2. diff_x and diff_y is difference from (0,0) in merged image. Alpha determines how much weight each of the two images will have(0<=alpha<=1)

<img src=./stitching1.png width="600"> 