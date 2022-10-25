#include <rek.h>
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
GetWeights(Mat lum, double mm, double MM, double myexp)
{
	
	double v;
	int    r, c, nr = lum.rows, nc = lum.cols;

	Mat nn = Mat::zeros(nr, nc, CV_64F);

	for (r = 0; r < nr; ++r)
		for (c = 0; c < nc; ++c)
		{
			v = (double)lum.at<uchar>(r, c);
			v = (v - mm) / (MM - mm);
			nn.at<double>(r, c) = pow((1 - v), myexp);
		}

	return nn;
}

Mat
ThresholdImage(Mat lum, double thresh)
{
	int r, c, i, nr = lum.rows, nc = lum.cols;
	Mat bitmap = Mat::zeros(nr, nc, CV_8UC1);
	double v, p;

	for (r = 0; r < nr; ++r)
		for (c = 0; c < nc; ++c)
		{
				v = (double)lum.at<uchar>(r, c);
							if (v > thresh)
				bitmap.at<uchar>(r, c) = 255;
		}

	return bitmap;
}



void
MeanOnDLRegionsPixels(Mat ima, Mat segm,
	double* LL, double* DD, double *dl, double* dd)
{
	int r, c, nr = ima.rows, nc = ima.cols;
	(*LL) = (*DD) = (*dl) = (*dd) = 0;
	double pl = 0;
	double pd = 0;
	double v;

	for (r = 0; r < nr; ++r)
		for (c = 0; c < nc; ++c)
			if (segm.at<uchar>(r, c) > 10)
			{
				(*LL) += (double)ima.at<uchar>(r, c) / 255;
				pl++;
			}
			else
			{
				(*DD) += (double)ima.at<uchar>(r, c) / 255;
				pd++;
			}
	(*LL) *= 255 / pl;
	(*DD) *= 255 / pd;

	double l = (*LL);
	double d = (*DD);
	(*dl) = 0;
	(*dd) = 0;
	for (r = 0; r < nr; ++r)
		for (c = 0; c < nc; ++c)
			if (segm.at<uchar>(r, c) > 10)
			{
				v = (double)ima.at<uchar>(r, c);
				(*dl) += (v - l)*(v - l);
			}
			else
			{
				v = (double)ima.at<uchar>(r, c);
				(*dd) += (v - d)*(v - d);
			}

	(*dl) /= pl;
	(*dd) /= pd;

	(*dl) = sqrt(*dl);
	(*dd) = sqrt(*dd);

	return;
}

Mat
ReLightImage(Mat image, double alpha[3])
{
	int r, c, i, nr = image.rows, nc = image.cols;
	double v;
	Mat out = Mat::zeros(nr, nc, CV_8UC3);
	for (r = 0; r < nr; ++r)
		for (c = 0; c < nc; ++c)
		{
			for (i = 0; i < 3; ++i)
			{
				v = image.at<Vec3b>(r, c)[i];
				v *= alpha[i];
				if (v > 256) v = 255;
				out.at<Vec3b>(r, c)[i] = (uchar)v;
			}
		}
	return out;
}



Mat
WeightedSum(Mat in, Mat out, Mat ww)
{
	int i, r, c, nr = in.rows, nc = in.cols;
	Mat res = Mat::zeros(nr, nc, CV_8UC3);
	double v[3], w, u[3];

	for (r = 0; r < nr; ++r)
		for (c = 0; c < nc; ++c)
		{
			w = ww.at<double>(r, c);

			for (i = 0; i < 3; ++i)
			{
				v[i] = (1 - w)*in.at<Vec3b>(r, c)[i];
				u[i] = w*out.at<Vec3b>(r, c)[i];

				res.at<Vec3b>(r, c)[i] = (uchar)(v[i] + u[i]);
			}

		}

	return res;
}

Mat 
ReK(Mat image, double p, double alpha)
{
	Mat lum, weights;
	double mm, MM, aa[3];

	cvtColor(image, lum, CV_BGR2GRAY);
	minMaxLoc(lum, &mm, &MM);
	weights = GetWeights(lum, mm, MM, p);
		
	if (alpha < 0) // automatic estimate of alpha
	{
		double thresh = (MM - mm) / 2, dark, light, Lstart, DD, dsL, dD;
		Mat segm = ThresholdImage(lum, thresh);
		MeanOnDLRegionsPixels(lum, segm, &Lstart, &DD, &dsL, &dD);
		alpha = (Lstart - dsL) / DD;
	}

	aa[0] = aa[1] = aa[2] = alpha;
	Mat out = ReLightImage(image, aa);
	Mat rek = WeightedSum(image, out, weights);

	return rek;
}
