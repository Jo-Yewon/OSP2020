# Edge and Corner Detection

## Laplacian of Gaussian

- LoG_gray.cpp
- LoG_RGB.cpp

실행을 위해서는 실행 파일과 같은 경로에 “lena.jpg”라는 이름의 입력 이미지가 존재해야 합니다.

이 코드의 목적은 이미지에 Laplacian of Gaussian 기법을 적용하여 이미지의 엣지를 찾아내는데에 목적이 있습니다. 이 코드에서 사용된 코드의 대부분은 이전 장에서 구현한 라플라시안 필터 및 가우시안 필터 적용과 거의 유사합니다.

<img src=./log.png width="480"> 

<br>

## Canny Edge Detector

- Canny.cpp

실행을 위해서는 실행 파일과 같은 경로에 “lena.jpg”라는 이름의 입력 이미지가 존재해야 합니다.

이 코드의 목적은 이미지에 Canny Edge Detector 방법을 적용하여 엣지를 찾는 것에 목적이 있습니다. 코드에서는 OpenCV에서 제공되는 Canny 함수를 사용합니다.

<img src=./canny.png width="480"> 


<br>

## Harris Corner Detector

- Harris_corner.cpp

실행을 위해서는 실행 파일과 같은 경로에 입력 이미지를 위치 시키고 main 함수에 해당 이미지의 이름을 넣어주어야 합니다.

이 코드의 목적은 이미지에 Harris Corner Detector 알고리즘을 적용하여 코너를 찾아내는 데에 의미가 있습니다. 코드에서는 OpenCV에서 제공하는 cornerHarris 함수를 이용합니다. 이후, 이 결과에 NonMaximum_suppression 혹은 Sub-pixel refinement를 수행하는 과정을 거칩니다. 이 과정의 수행 여부는 main 내부의 bool 변수를 통해 지정할 수 있습니다.

<img src=./hr.png width="480"> 

<img src=./hc.png width="480"> 

<img src=./hc_sp.png width="480"> 