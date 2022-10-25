// meek.cpp : Defines the entry point for the console application.
//
// this is a small, image enhancement kit whose functions can be useful for 
// improving the quality of archeological pictures, such as image of absides with 
// backlight or low-light environements like archeological excavation

// the acronym is from the expression:  iMage EnhancEment Kit

#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <super.h>
#include <rek.h>

using namespace std;
using namespace cv;


void
ReadParameterFile(char* name, int *super_grid, double* p, double* al, int* clip)
{
	ifstream paramFile(name);
	string   super, rek, clahe;

	cout << endl << " =============== " << endl;
	cout << " Image Enhancement Parameters: " << endl;
	while (!paramFile.eof())
	{
		if (!(paramFile >> super >> (*super_grid) >> rek >> (*p) >> (*al)
			>> clahe >> (*clip)))
			break;

		cout << super << " n. tiles =  " << *super_grid << endl;
		cout << rek << " exponent =  " << *p << " " << *al << endl;
		cout << clahe << " clip =  " << *clip << endl;

	}
	cout << " =============== " << endl;
	cout << " Note that if alpha = -1, REK estimates automatically a value for alpha." << endl;
	return;
}

int       cmd; // cmd =1 -> super; cmd = 2-> rek; cmd = 3 -> clahe etc
cv::Rect rect1(40, 540, 100, 50);
cv::Rect rect2(160, 540, 100, 50);
cv::Rect rect3(280, 540, 100, 50);
cv::Rect rect4(400, 540, 100, 50);
cv::Rect rect5(520, 540, 100, 50);
cv::Rect rect6(640, 540, 100, 50);
Mat  panel;
Mat Out;
Mat image;
int super_grid, clip;
double rek_p, rek_alpha;
char* winname = "MEEK";
char stop;

Mat
CreateGUIPanel()
{
	int   nr = 600, nc = 1286, r, c, i;
	panel = Mat::zeros(nr, nc, CV_8UC3);

	panel.setTo(Scalar(128, 128, 128));

	int rs = 2;
	int re = rs + 480;
	int cs = 2 + 640;
	int ce = cs + 640;

	// canvas for processed image
	for (r = rs; r < re; ++r)
		for (c = cs; c < ce; ++c)
		{
			for (i = 0; i < 3; ++i)
				panel.at<Vec3b>(r, c)[i] = 250;
		}

	// canvas for input image
	for (r = 2; r < 2 + 480; ++r)
		for (c = 2; c < 640 + 2; ++c)
		{
			for (i = 0; i < 3; ++i)
				panel.at<Vec3b>(r, c)[i] = 250;
		}

	// include buttons
	Point text_position(50, 570);
	// button for SuPeR

	rectangle(panel, rect1, Scalar(64, 64, 64), CV_FILLED, 8, 0);

	putText(panel, "SuPeR", text_position,
		FONT_HERSHEY_PLAIN, 1.5, Scalar(250, 250, 250), 2, 8, false);

	// button for ReK
	text_position.x = 190;
	text_position.y = 570;

	rectangle(panel, rect2, Scalar(64, 64, 64), CV_FILLED, 8, 0);

	putText(panel, "ReK", text_position,
		FONT_HERSHEY_PLAIN, 1.5, Scalar(250, 250, 250), 2, 8, false);

	// button for CLAHE
	text_position.x = 290;
	text_position.y = 570;


	rectangle(panel, rect3, Scalar(64, 64, 64), CV_FILLED, 8, 0);

	putText(panel, "CLAHE", text_position,
		FONT_HERSHEY_PLAIN, 1.5, Scalar(250, 250, 250), 2, 8, false);

	// button for Reset
	text_position.x = 410;
	text_position.y = 570;

	rectangle(panel, rect4, Scalar(64, 64, 64), CV_FILLED, 8, 0);

	putText(panel, "Reset", text_position,
		FONT_HERSHEY_PLAIN, 1.5, Scalar(250, 250, 250), 2, 8, false);

	// button for Save
	text_position.x = 530;
	text_position.y = 570;

	rectangle(panel, rect5, Scalar(64, 64, 64), CV_FILLED, 8, 0);

	putText(panel, "Save", text_position,
		FONT_HERSHEY_PLAIN, 1.5, Scalar(250, 250, 250), 2, 8, false);

	// button for Quit
	text_position.x = 650;
	text_position.y = 570;

	rectangle(panel, rect6, Scalar(64, 64, 64), CV_FILLED, 8, 0);

	putText(panel, "Quit", text_position,
		FONT_HERSHEY_PLAIN, 1.5, Scalar(250, 250, 250), 2, 8, false);


	// LOGO
	text_position.x = 1000;
	text_position.y = 580;
	putText(panel, "MEEK", text_position,
		FONT_HERSHEY_SIMPLEX, 3.5, Scalar(200, 200, 0), 8, 8, false);

	return panel;
}


void
LoadImageOnPanel(Mat image, int A, int B, Mat &panel)
{
	int    nr = image.rows, nc = image.cols;
	int    maxR = 480, maxC = 640, q = -1;
	Mat    show = Mat::zeros(maxR, maxC, CV_8UC3);
	double s;

	if (nr > maxR || nc > maxC)
	{
		double	f = min(maxR*1.0 / nr, maxC*1.0 / nc);
		resize(image, show, Size(), f, f);
		q = 2;
	}

	if (q == -1)
		// insert in panel the exact image
		image.copyTo(panel(cv::Rect(B, A, image.cols, image.rows)));
	else
		// insert in panel the resized image
		show.copyTo(panel(cv::Rect(B, A, show.cols, show.rows)));

	return;
}

void callBackFunc(int event,
	int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (rect1.contains(Point(x, y)))
		{
			cout << "SuPeR" << " cmd = " << cmd << endl;
			if (cmd == 0)
			{
				cmd = 1;
				Super S;
				Mat copia;
				image.copyTo(copia);
				Mat *sup = S.compute(copia, super_grid);
				sup->copyTo(Out);
				LoadImageOnPanel(Out, 2, 642, panel);
				imshow(winname, panel);
				waitKey();

			}
			else
			{
				cmd = 1;
				Super S;
				Mat *sup = S.compute(Out, super_grid);
				sup->copyTo(Out);
				LoadImageOnPanel(Out, 2, 642, panel);
				imshow(winname, panel);
				waitKey();

			}


		}
		if (rect2.contains(Point(x, y)))
		{
			cout << "ReK cmd " << cmd << endl;

			if (cmd == 0)
			{
				cmd = 2;
				Out = ReK(image, rek_p, rek_alpha);
				LoadImageOnPanel(Out, 2, 642, panel);
				imshow(winname, panel);
				waitKey();

			}
			else
			{
				cmd = 2;
				Mat tmp;
				tmp = ReK(Out, rek_p, rek_alpha);
				tmp.copyTo(Out);
				LoadImageOnPanel(Out, 2, 642, panel);
				imshow(winname, panel);
				waitKey();
			}

		}
		if (rect3.contains(Point(x, y)))
		{
			cout << "CLAHE cmd " << cmd << endl;
			Ptr<CLAHE> clahe = createCLAHE();
			clahe->setClipLimit(clip);
			if (cmd == 0)
			{
				cmd = 3;
				Mat lab_image;
				cvtColor(image, lab_image, CV_BGR2Lab);

				// Extract the L channel
				std::vector<Mat> lab_planes(3);

				split(lab_image, lab_planes);

				// apply the CLAHE algorithm to the L channel
				Ptr<CLAHE> clahe = createCLAHE();
				clahe->setClipLimit(clip);
				Mat dst;
				clahe->apply(lab_planes[0], dst);

				// Merge the the color planes back into an Lab image
				dst.copyTo(lab_planes[0]);
				merge(lab_planes, lab_image);

				// convert back to RGB
				cvtColor(lab_image, Out, CV_Lab2BGR);
			}

			else
			{
				cmd = 3;
				Mat tmp;
				Mat lab_image;
				cvtColor(Out, lab_image, CV_BGR2Lab);

				// Extract the L channel
				vector<Mat> lab_planes(3);
				split(lab_image, lab_planes);

				// apply the CLAHE algorithm to the L channel
				Ptr<CLAHE> clahe = createCLAHE();
				clahe->setClipLimit(clip);
				Mat dst;
				clahe->apply(lab_planes[0], dst);

				// Merge the the color planes back into an Lab image
				dst.copyTo(lab_planes[0]);
				merge(lab_planes, lab_image);

				// convert back to RGB
				cvtColor(lab_image, tmp, CV_Lab2BGR);
				tmp.copyTo(Out);
			}
			LoadImageOnPanel(Out, 2, 642, panel);
			imshow(winname, panel);
			waitKey();

		}
		if (rect4.contains(Point(x, y)))
		{
			cout << "Reset" << endl;
			cmd = 0;
			cv::Rect rect(642, 2, 640, 480);
			rectangle(panel, rect, Scalar(250, 250, 250), CV_FILLED, 8, 0);

			imshow(winname, panel);
			waitKey();

		}
		if (rect5.contains(Point(x, y)))
		{
			cout << "Save" << endl;

			if (cmd = 0)
			{
				cout << " nothing to save " << endl;
				cmd = 5;
			}
			else
			{
				imwrite("enhanced_image.png", Out);
				cout << "Enhanced Image saved in enhanced_img.png" << endl;
				cmd = 5;
			}
		}
		if (rect6.contains(Point(x, y)))
		{
			cout << "Quit" << endl;
			cmd = 6;
			destroyAllWindows();
			exit(0);
		}
	}


}



int main(int argc, char* argv[])
{
	image = imread(argv[1]);
	char*    pfile = argv[2];

	// read paramaters
	ReadParameterFile(pfile, &super_grid, &rek_p, &rek_alpha, &clip);

	// load (rescaled) image open interface
	panel = CreateGUIPanel();
	cmd = 0;
	namedWindow(winname);
	setMouseCallback(winname, callBackFunc);

	LoadImageOnPanel(image, 2, 2, panel);

	imshow(winname, panel);
	waitKey();


	return 0;
}

