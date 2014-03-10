//ProcessImage.h
#pragma once
#include <opencv2/highgui/highgui.hpp>

/* 对输入图像进行细化
 * src为输入图像,用cvThreshold函数处理过的8位灰度图像格式，元素中只有0与1,1代表有元素，0代表为空白
 * dst为对src细化后的输出图像,格式与src格式相同，调用前需要分配空间，元素中只有0与1,1代表有元素，0代表为空白
 * maxIterations限制迭代次数，如果不进行限制，默认为-1，代表不限制迭代次数，直到获得最终结果
 */
void thinImage(IplImage* src, IplImage* dst, int maxIterations = -1);
