# MEEK
iMage EnhancEment Kit

This is the implementation of MEEK, a basic tool for enhancing images acquired undeer low-light, backligth and spotlight. MEEK has been tested as an enhancer of archaeological images, which depict places or objects that are difficult to be well illuminated, such as excavation, crypts, mosaics and paintings.


![meek-01](https://user-images.githubusercontent.com/82895709/197752535-e00de778-858c-4f18-b51f-97d8e03b84ee.JPG)


MEEK embeds three algorithms for image enhancement:
1) SuPeR [1], a Retinex inspired algorithm for improving non uniform low-light images, also mitigiating/removing possible color catsts due to the illuminations;
2) REK [2], a relighting method based on the von Kries model and specifically designed for the enhancement of images with strong backlight and spotlight;
3) CLAHE [3] , a popular algorithm for enhancing the image contrast by redistributing the image luminance.

The picture reported above shows the interface of MEEK. In this example, a backlight image (on left) has been relighted by REK (on right).

MEEK is implemented in C++ and exploits the OpenCV C++ library (https://opencv.org/). 

***********************************************************
## Compilation Rule:

To compile MEEK (meek.cpp) please include the OpenCV libraries and the files rek.h, rek.cpp, super.h.

Example of Compilation under Unix/Ubuntu:

g++ -Wall  -I.  -I/home/mySoftware/opencv-3.4.0/release//include/ -g    -L. -L/home/mySoftware/opencv-3.4.0/release/lib/  ./Meek.o ./rek.o -o ./Meek -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_features2d -lopencv_video -lopencv_photo

Before compiling under Windows please uncomment #define WIN  in rek.h, rek.cpp and meek.cpp

***********************************************************
## Using MEEK:

Run the following command:

meek <input_image> <parameterFile.txt>

where meek is the exucutabl file produced by compiling, <input_image> is the image to be enhanced, <parameterFile.txt> is the file with the parameters of the three algorithms embedded in MEEK.

This command will open a window (with fixed size), showing on left the input image. Press the commands on bottom for running the desired enhancer(s), resetting the enhancement (i.e. restarting the enhancement from the input image), saving the output image or quitting. 
Pressing 'Save', the output image will be saved as enhanced_image.png.

Note that the code of SuPeR has been cloned from  https://github.com/StefanoMesselodi/Super

***********************************************************
## References:

[1] M. Lecca and S. Messelodi. SuPeR: Milano Retinex implementation exploiting a regular image grid. Journal of the Optical Society of America A - Optics Image Science and Vision, 36(8):1423--1432, 2019

[2] M. Lecca. Relighting Backlight and Spotlight Images using the von Kries Model, 2nd International Conference on Image Processing and Vision Engineering - IMPROVE, April 2022

[3] K. J. Zuiderveld. Contrast limited adaptive histogram equalization. In Graphics Gems, 1994.
 
