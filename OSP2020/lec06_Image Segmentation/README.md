# Image Segmentation

## Adaptive Thresholding using Moving Averages

- Adaptivethreshold.cpp

실행을 위해서는 실행 파일과 같은 경로에 “writing.jpg”라는 이름의 입력 이미지가 존재해야 합니다.

이 코드의 목적은 흑백 이미지에 adaptive thresholding을 적용하는 것에 목표가 있습니다. 이를 이용해 전체적 으로 균일하지 않은 이미지도 부분 별로 thresholding을 적용하게 되어 기존의 Uniform한 thresholding에 비 해 더 좋은 효과를 보일 수 있습니다. ```Mat adaptive_thres(const Mat input, int n, float bnumber)``` 가 함수 형태입니다. N을 통해 커널의 사이즈를 지정할 수 있으며, b는 weight을 의미합니다.

<img src=./ad1.png width="480">  

<img src=./ad2.png width="480">


<br>

## K-Means Clustering

- KMeanGray.cpp
- KMeanRGB.cpp

실행을 위해서는 실행 파일과 같은 경로에 “lena.jpg”라는 이름의 입력 이미지가 존재해야 합니다.

이 코드의 목적은 k-means 클러스터링 기법을 적용하여 이미지를 세그먼트화 하는데에 목적이 있습니다.
```Mat kMeansGray(Mat input, int clusterCount, int attempts);``` 이 함수에서는 intensity I 만을 가지고 클러스터링을 수행합니다. clusterCount는 클러스터링 할 집합의 개수로 초기 포인트의 개수이기도 합니다. attempt는 k-mean의 과정을 총 몇번 수행할 것인지를 의미합니다.

<img src=./kmean1.png width="480"> 

```Mat KMeansGrayPosition(Mat input, int clusterCount, int attempts, double sigma);``` 이 함수에서는 intensity와 position을 이용해서 클러스터링을 수행합니다. 다른 파라미터는 kMeansGray와 같으며 sigma는 position 값에 적용될 함수입니다. Sigma가 작을 수록 클러스터링에서 위치를 더 많이 고려하게 됩니다.

<img src=./kmean2.png width="480"> 
