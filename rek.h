#pragma once
#ifndef REK_H
#define REK_H


#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat
ReK(Mat image, double p, double alpha);

#endif // !REK_H