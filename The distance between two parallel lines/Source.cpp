//Source.cpp
#include "ProcessImage.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#define _TEST
using namespace cv;
int main(int argc, char * argv[])
{
	//判断输入是否满足要求
	if (argc != 2)
	{
		std::cout << "argument error!";
		return -1;
	}
	IplImage *pSrc = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (!pSrc)
	{
		std::cout << "read file failed!";
		return -1;
	}

	//显示原图
	namedWindow("原图", CV_WINDOW_AUTOSIZE);
	cvShowImage("原图", pSrc);

	IplImage *pTemp = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);
	IplImage *pDst = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);

	//将原图像转换为二值图像
	cvThreshold(pSrc, pTemp, 128, 1, CV_THRESH_BINARY_INV);
	//细化
	thinImage(pTemp, pDst);

#ifdef _TEST
	//显示细化后的图像
	IplImage *pThinImage = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);
	cvCopy(pDst, pThinImage);
	cvThreshold(pThinImage, pThinImage, 0.5, 255,CV_THRESH_BINARY);
	namedWindow("1 图像细化的结果", CV_WINDOW_AUTOSIZE);
	cvShowImage("1 图像细化的结果", pThinImage);
	cvReleaseImage(&pThinImage);
#endif

	//求轮廓
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	cvFindContours(pDst	, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

#ifdef _TEST
	//将轮廓画出来
	IplImage *pDrawing1 = cvCreateImage(cvGetSize(pSrc),8,3);
	cvZero(pDrawing1);
	cvDrawContours(pDrawing1, contours, Scalar(255, 0, 0), Scalar(0, 0, 255), 1, 2, 8, cvPoint(0, 0));
	namedWindow("2 求轮廓", CV_WINDOW_AUTOSIZE);
	cvShowImage("2 求轮廓", pDrawing1);
	cvReleaseImage(&pDrawing1);
#endif


	//轮廓已经寻找到，均在contours中存放,我们需要对轮廓进行拟合
	//FitLine函数的用法：
	// 二维空间点拟合时 是 float[4]
	// 三位空间点拟合时 是 float[6]	
	float *line1 = new float[4];
	float *line2 = new float[4];
	// 第一个参数： 存储点序列
	// 第二个参数： 拟合算法，其中 CV_DIST_L2 就是平常的最小二乘法
	// 第三，第四，第五参数推荐值是 0,   0.01,  0.01,
	// 第六参数： line中存储返回值
	// 二维空间时： line[0--3] 分别为 (vx, vy, x0, y0)
	//      其中 vx， vy 是正规化之后的斜率向量。 x0,y0 是直线经过的点。
	// 三维空间时： line[0--5]  分别是 (vx, vy, vz, x0, y0, z0) 。意义同上
	cvFitLine(contours, CV_DIST_L2, 0, 0.01, 0.01, line1);
	cvFitLine(contours->h_next, CV_DIST_L2, 0, 0.01, 0.01, line2);
	
	//输出四个点
	std::cout << "第一条线： " << line1[0] << " " << line1[1] << " " << line1[2] << " " << line1[3] << std::endl;
	std::cout << "第二条线： " << line2[0] << " " << line2[1] << " " << line2[2] << " " << line2[3] << std::endl;
	
#ifdef _TEST
	//根据直线方程公式，我们从直线上取点，并画出来
	IplImage *pDrawing2 = cvCreateImage(cvGetSize(pSrc), 8, 3);
	cvZero(pDrawing2);
	cvLine(pDrawing2, cvPoint(0, (int)(line1[3] - line1[1] / line1[0] * line1[2])),
		cvPoint(pDrawing2->width - 1, (int)((pDrawing2->width - 1 - line1[2])*line1[1] / line1[0] + line1[3])),
		cvScalar(255, 0, 0));
	cvLine(pDrawing2, cvPoint(0, (int)(line2[3] - line2[1] / line2[0] * line2[2])), 
		cvPoint(pDrawing2->width - 1, (int)((pDrawing2->width - 1 - line2[2])*line2[1] / line2[0] + line2[3])), 
		cvScalar(0, 0, 255));
	namedWindow("3 直线拟合", CV_WINDOW_AUTOSIZE);
	cvShowImage("3 直线拟合", pDrawing2);
	cvReleaseImage(&pDrawing2);
#endif

	//我们根据距离方程，求出两条直线的距离
	double distance = abs(line1[0] * (line2[3]-line1[3]) - line1[1] * (line2[2]-line1[2]));	//注意,vx,vy已经正规化了
	std::cout << "两条直线之间的距离为: " << distance << std::endl;
	delete[] line1;
	delete[] line2;

	cvReleaseMemStorage(&storage);
	cvReleaseImage(&pSrc);
	cvReleaseImage(&pTemp);
	cvReleaseImage(&pDst);

	waitKey(0);

	return 0;
}