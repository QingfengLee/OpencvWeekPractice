#include "ProcessImage.h"
#include <utility>
#include <vector>
void thinImage(IplImage* src, IplImage* dst, int maxIterations)
{
	using namespace cv;
	CvSize size = cvGetSize(src);
	cvCopy(src, dst);//将src中的内容拷贝到dst中  
	int count = 0;  //记录迭代次数  
	while (true)
	{
		count++;
		if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达  
			break;
		//std::cout << count << ' ';输出迭代次数  
		std::vector<std::pair<int, int> > mFlag; //用于标记需要删除的点  
		//对点标记  
		for (int i = 0; i<size.height; ++i)
		{
			for (int j = 0; j<size.width; ++j)
			{
				//如果满足四个条件，进行标记  
				//  p9 p2 p3  
				//  p8 p1 p4  
				//  p7 p6 p5  
				int p1 = CV_IMAGE_ELEM(dst, uchar, i, j);
				int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
				int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
				int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
				int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
				int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
				int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
				int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
				int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1)
					{
						if (p2*p4*p6 == 0)
						{
							if (p4*p6*p8 == 0)
							{
								//标记  
								mFlag.push_back(std::make_pair(i, j));
							}
						}
					}
				}
			}
		}

		//将标记的点删除  
		for (std::vector<std::pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
		}

		//直到没有点满足，算法结束  
		if (mFlag.size() == 0)
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空  
		}

		//对点标记  
		for (int i = 0; i<size.height; ++i)
		{
			for (int j = 0; j<size.width; ++j)
			{
				//如果满足四个条件，进行标记  
				//  p9 p2 p3  
				//  p8 p1 p4  
				//  p7 p6 p5  
				int p1 = CV_IMAGE_ELEM(dst, uchar, i, j);
				if (p1 != 1) continue;
				int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
				int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
				int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
				int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
				int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
				int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
				int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
				int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1)
					{
						if (p2*p4*p8 == 0)
						{
							if (p2*p6*p8 == 0)
							{
								//标记  
								mFlag.push_back(std::make_pair(i, j));
							}
						}
					}
				}
			}
		}
		//删除  
		for (std::vector<std::pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;
		}

		//直到没有点满足，算法结束  
		if (mFlag.size() == 0)
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空  
		}
	}
}



