//Source.cpp
#include "ProcessImage.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#define _TEST
using namespace cv;
int main(int argc, char * argv[])
{
	//�ж������Ƿ�����Ҫ��
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

	//��ʾԭͼ
	namedWindow("ԭͼ", CV_WINDOW_AUTOSIZE);
	cvShowImage("ԭͼ", pSrc);

	IplImage *pTemp = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);
	IplImage *pDst = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);

	//��ԭͼ��ת��Ϊ��ֵͼ��
	cvThreshold(pSrc, pTemp, 128, 1, CV_THRESH_BINARY_INV);
	//ϸ��
	thinImage(pTemp, pDst);

#ifdef _TEST
	//��ʾϸ�����ͼ��
	IplImage *pThinImage = cvCreateImage(cvGetSize(pSrc), pSrc->depth, pSrc->nChannels);
	cvCopy(pDst, pThinImage);
	cvThreshold(pThinImage, pThinImage, 0.5, 255,CV_THRESH_BINARY);
	namedWindow("1 ͼ��ϸ���Ľ��", CV_WINDOW_AUTOSIZE);
	cvShowImage("1 ͼ��ϸ���Ľ��", pThinImage);
	cvReleaseImage(&pThinImage);
#endif

	//������
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	cvFindContours(pDst	, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

#ifdef _TEST
	//������������
	IplImage *pDrawing1 = cvCreateImage(cvGetSize(pSrc),8,3);
	cvZero(pDrawing1);
	cvDrawContours(pDrawing1, contours, Scalar(255, 0, 0), Scalar(0, 0, 255), 1, 2, 8, cvPoint(0, 0));
	namedWindow("2 ������", CV_WINDOW_AUTOSIZE);
	cvShowImage("2 ������", pDrawing1);
	cvReleaseImage(&pDrawing1);
#endif


	//�����Ѿ�Ѱ�ҵ�������contours�д��,������Ҫ�������������
	//FitLine�������÷���
	// ��ά�ռ�����ʱ �� float[4]
	// ��λ�ռ�����ʱ �� float[6]	
	float *line1 = new float[4];
	float *line2 = new float[4];
	// ��һ�������� �洢������
	// �ڶ��������� ����㷨������ CV_DIST_L2 ����ƽ������С���˷�
	// ���������ģ���������Ƽ�ֵ�� 0,   0.01,  0.01,
	// ���������� line�д洢����ֵ
	// ��ά�ռ�ʱ�� line[0--3] �ֱ�Ϊ (vx, vy, x0, y0)
	//      ���� vx�� vy �����滯֮���б�������� x0,y0 ��ֱ�߾����ĵ㡣
	// ��ά�ռ�ʱ�� line[0--5]  �ֱ��� (vx, vy, vz, x0, y0, z0) ������ͬ��
	cvFitLine(contours, CV_DIST_L2, 0, 0.01, 0.01, line1);
	cvFitLine(contours->h_next, CV_DIST_L2, 0, 0.01, 0.01, line2);
	
	//����ĸ���
	std::cout << "��һ���ߣ� " << line1[0] << " " << line1[1] << " " << line1[2] << " " << line1[3] << std::endl;
	std::cout << "�ڶ����ߣ� " << line2[0] << " " << line2[1] << " " << line2[2] << " " << line2[3] << std::endl;
	
#ifdef _TEST
	//����ֱ�߷��̹�ʽ�����Ǵ�ֱ����ȡ�㣬��������
	IplImage *pDrawing2 = cvCreateImage(cvGetSize(pSrc), 8, 3);
	cvZero(pDrawing2);
	cvLine(pDrawing2, cvPoint(0, (int)(line1[3] - line1[1] / line1[0] * line1[2])),
		cvPoint(pDrawing2->width - 1, (int)((pDrawing2->width - 1 - line1[2])*line1[1] / line1[0] + line1[3])),
		cvScalar(255, 0, 0));
	cvLine(pDrawing2, cvPoint(0, (int)(line2[3] - line2[1] / line2[0] * line2[2])), 
		cvPoint(pDrawing2->width - 1, (int)((pDrawing2->width - 1 - line2[2])*line2[1] / line2[0] + line2[3])), 
		cvScalar(0, 0, 255));
	namedWindow("3 ֱ�����", CV_WINDOW_AUTOSIZE);
	cvShowImage("3 ֱ�����", pDrawing2);
	cvReleaseImage(&pDrawing2);
#endif

	//���Ǹ��ݾ��뷽�̣��������ֱ�ߵľ���
	double distance = abs(line1[0] * (line2[3]-line1[3]) - line1[1] * (line2[2]-line1[2]));	//ע��,vx,vy�Ѿ����滯��
	std::cout << "����ֱ��֮��ľ���Ϊ: " << distance << std::endl;
	delete[] line1;
	delete[] line2;

	cvReleaseMemStorage(&storage);
	cvReleaseImage(&pSrc);
	cvReleaseImage(&pTemp);
	cvReleaseImage(&pDst);

	waitKey(0);

	return 0;
}