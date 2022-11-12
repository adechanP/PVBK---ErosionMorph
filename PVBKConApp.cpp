// PVBKConApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <opencv2/opencv.hpp>

#pragma comment(lib, "opencv_world460")

using namespace cv;
using namespace std;

bool checkImageProperties(Mat& img1, Mat& img2)
{
	if (img1.empty())
	{
		printf("img1 is empty");
		return false;
	}

	if (img2.empty())
	{
		printf("img2 is empty");
		return false;
	}

	if (img1.cols != img2.cols)
	{
		printf("widths are different\n");
		return false;
	}

	if (img1.rows != img2.rows)
	{
		printf("heights are different\n");
		return false;
	}

	if (img1.channels() != img2.channels())
	{
		printf("Channels are different\n");
		return false;
	}

	return true;
}


///////////// Pixel Operation /////////////
/////// This is the basic template ////////
///////////// Don't change it /////////////
int Grayscale(Mat& img_src, Mat& img_dst)
{
	Vec3b bgr_src;
	uchar gray_dst;

	for (int y = 0; y < img_src.rows; y++)
	{
		for (int x = 0; x < img_src.cols; x++)
		{
			bgr_src = img_src.at<Vec3b>(y, x);
			unsigned char b = bgr_src[0];
			unsigned char g = bgr_src[1];
			unsigned char r = bgr_src[2];

			uchar gr = (uchar)((float)(b + g + r) / 3);
			gray_dst = gr;

			img_dst.at<uchar>(y, x) = gray_dst;
		}
	}
	return 0;
}


int ErosionMorph(Mat& img_src1, Mat& img_dst1)
{
	uchar gray_src1 = 0;
	Vec3b min_eroded;
	uchar data[9];
	//Mat morph_result = Mat::zeros(img_src1.size(), CV_8UC1);

	Mat morph_mask = (Mat_<char>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	for (int y = 1; y < img_src1.rows - 1; y++)
	{
		for (int x = 1; x < img_src1.cols - 1; x++)
		{
			gray_src1 = img_src1.at<uchar>(y, x);
			data[0] = img_src1.at<uchar>(y - 1, x - 1);
			data[1] = img_src1.at<uchar>(y - 1, x);
			data[2] = img_src1.at<uchar>(y - 1, x + 1);
			data[3] = img_src1.at<uchar>(y, x - 1);
			data[4] = img_src1.at<uchar>(y, x);
			data[5] = img_src1.at<uchar>(y, x + 1);
			data[6] = img_src1.at<uchar>(y + 1, x - 1);
			data[7] = img_src1.at<uchar>(y + 1, x);
			data[8] = img_src1.at<uchar>(y + 1, x + 1);

			int16_t eroded[9] = { 0 };
			for (int i = 0; i < 9; i++)
			{
				eroded[i] = data[i] * morph_mask.at<char>(i / 3, i % 3);
			}
			min_eroded[0] = eroded[0];
			min_eroded[1] = eroded[0];
			min_eroded[2] = gray_src1;
			for (int i = 0; i < 9; i++)
			{
				if (eroded[i] < min_eroded[0])
				{
					min_eroded[0] = eroded[i];
					min_eroded[1] = eroded[i];
				}
			}
			img_dst1.at<Vec3b>(y, x) = min_eroded;
		}
	}
	return 0;
}

int main()
{
	// Read the file
	Mat inputImg_1 = imread("Morph_Test.jpg", IMREAD_GRAYSCALE);

	// Check for invalid input
	if (inputImg_1.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Create new image
	Mat GRinputImg_1 = Mat::zeros(inputImg_1.size(), CV_8UC1);
	Mat ErodedImg_1 = Mat::zeros(inputImg_1.size(), CV_8UC3);

	// Invoke RGB2GRAY function here
	ErosionMorph(inputImg_1, ErodedImg_1);


	//Confirmation Code - Using imgproc.h
	/*cvtColor(inputImg, outputImg, COLOR_RGB2HSV);*/
	//cvtColor(inputImg, outputImg, COLOR_BGR2HSV);

	// Create a window for display.
	namedWindow("Input_1", WINDOW_AUTOSIZE);
	namedWindow("OutputER_1", WINDOW_AUTOSIZE);


	// Show our image inside it.
	imshow("Input_1", inputImg_1);
	imshow("OutputER_1", ErodedImg_1);




	// Wait for a keystroke in the window
	waitKey(0);

	destroyAllWindows();

	return 0;
}


//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
