//ProcessImage.h
#pragma once
#include <opencv2/highgui/highgui.hpp>

/* ������ͼ�����ϸ��
 * srcΪ����ͼ��,��cvThreshold�����������8λ�Ҷ�ͼ���ʽ��Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
 * dstΪ��srcϸ��������ͼ��,��ʽ��src��ʽ��ͬ������ǰ��Ҫ����ռ䣬Ԫ����ֻ��0��1,1������Ԫ�أ�0����Ϊ�հ�
 * maxIterations���Ƶ���������������������ƣ�Ĭ��Ϊ-1���������Ƶ���������ֱ��������ս��
 */
void thinImage(IplImage* src, IplImage* dst, int maxIterations = -1);
