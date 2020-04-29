# Image Restoration

## Generate salt-and-pepper noise

- salt_and_pepper.cpp

실행을 위해서는 실행 파일과 같은 경로에 입력 이미지가 존재해야 합니다.

이 소스코드는 이미지에 Salt-and-Pepper noise를 추가합니다. 
```Add_salt_pepper_Noise(const Mat input, float ps, float pp)```이며, input은 입력 이미지, ps는 salt noise가 발생할 확률, pp는 pepper noise가 발생할 확률입니다.

<img src=./snp1.png width="480">

<br>

## Remove salt-and-pepper noise using median filter

- salt_and_pepper.cpp

실행을 위해서는 실행 파일과 같은 경로에 입력 이미지가 존재해야 합니다.

이 소스코드는 이미지에 Salt-and-Pepper noise가 있는 이미지에 대해 median filter를 적용하여, 노이즈를 줄 이는데에 목적이 있습니다. 주요 함수의 형태는 ```Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt)``` 이며, n은 커널의 크기를 정하는 변수이고, opt는 이미지의 경계를 처리하는 방식을 정하기 위해 사용됩니다.

<img src=./median1.png width="480">

<br>

## Generate gaussian noise

- Gaussian.cpp

실행을 위해서는 실행 파일과 같은 경로에 입력 이미지가 존재해야 합니다.

이 소스코드는 이미지에 Gaussian noise를 생성하는데에 목적이 있습니다. 주요 함수의 형태는 ```Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);``` 입니다. AWGN은 mean이 0이 기 때문에 파라미터 mean으로는 0가 들어가며, sigma는 가우시안 함수에서 사용되는 변수로 노이즈의 정도를 조절하는데 쓰입니다.

<img src=./gaussian.png width="480">


<br>

## Gaussian Noise Removal using Gaussian Filtering

- Gaussian.cpp

실행을 위해서는 실행 파일과 같은 경로에 “lena.jpg”라는 이름의 입력 이미지가 존재해야 합니다.

이 소스코드는 가우시안 노이즈가 있는 이미지에 가우시안 필터를 적용하여 노이즈를 줄이는 것에 목적이 있습니 다. 함수의 형태는 ```Mat Gaussianfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, const char *opt);``` 이며, n은 커널의 크기를, sigma는 가우시안 함수에서 이용되는 상수이며, opt는 경계처리방 식을 지정하기 위한 파라미터입니다.

<img src=./gf.png width="480">

<br>

## Gaussian Noise Removal using Bilateral Filtering

- Bilateral.cpp

실행을 위해서는 실행 파일과 같은 경로에 “lena.jpg”라는 이름의 입력 이미지가 존재해야 합니다.

이 소스코드는 가우시안 노이즈가 있는 이미지에 Bilateral Filter를 적용하여 노이즈를 줄이는 것에 목적이 있습 니다. 함수의 형태는 ```Mat Bilateralfilter_Gray(const Mat input, int n, double sigmaT, double sigmaS, double sigmaR, const char *opt);```입니다. n은 커널의 크기, sigmaT, sigmaS는 필터의 가우시안 부분에 사용되는 파라미터이며, sigmaR은 intensity의 diversity를 계산하는 식에 사용됩니다. Opt는 이미지의 경계를 처리하는 방식을 지정하는데 사용됩니다.

<img src=./bi.png width="480">
