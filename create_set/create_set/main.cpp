#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream>
#include <conio.h>
#include <strstream>

#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"
using namespace cv;
using namespace std;

//变量定义
Mat org,img,tmp;//原始图像，处理图像，显示图像，截取图像
float widthHeightRatio = 1.0;//图像比例：宽度/长度
string orgName;//图像名称

void on_mouse(int event,int x,int y,int flags,void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{
	static Point2i pre_pt,end_pt;//截取图像初始坐标，结束坐标
	Point2i cur_pt = Point2i(x,y);//实时坐标
	char ptPos[16];//指针坐标
	sprintf(ptPos,"(%d,%d)",x,y);//创建实时坐标显示文本
	img.copyTo(tmp);//将img复制到临时图像tmp坐标上，用于显示实时坐标

	

	if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数，什么都不做
	{
	}
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标
	{
		pre_pt = Point2i(x,y);//读取初始坐标
	}  
	if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上画矩形  
	{
		end_pt = Point2i(cur_pt.x,pre_pt.y+(cur_pt.x-pre_pt.x)/widthHeightRatio);//计算截取结束坐标
		//end_pt = Point2i(cur_pt.x, cur_pt.y);
		cv::rectangle(tmp,pre_pt,end_pt,Scalar(255,255,0,0),5,8,0);//在显示图像上实时显示鼠标拖动时形成的矩形  
	} 
	if (event == CV_EVENT_LBUTTONUP)//左键松开，在图像上画矩形并保存，保存截取区域
	{
		static int i = 0;
		strstream ss;
		string s;

		rectangle(img,pre_pt,end_pt,Scalar(0,255,0,0),1,8,0);//在处理图像上显示矩形
		imwrite(orgName+"0.jpg",img);//保存img

		ss<<"e"<<i<<".jpg";
		ss>>s;
		cout<<s<<endl;
		Mat outImg = org(Rect(pre_pt,end_pt));
		imwrite(s,outImg);
		i++;
	}

	//在图像上放置坐标 参数为：图片，文字，文字在图像中的左下角坐标，字体类型，大小，颜色，粗细，线型
	putText(tmp,ptPos,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,10);
	imshow("img",tmp); //显示图像

}

int main()
{
	////---------------------图像------------------------
	////输入图像名称
	//cout<<"输入JPG图像名称：";
	//cin>>orgName;
	//org = imread(orgName+".jpg");
	//org.copyTo(img);
	//namedWindow("img",WINDOW_NORMAL);
	//setMouseCallback("img",on_mouse,0);//调用回调函数
	//imshow("img",img);

	////输入按键退出程序
	//bool stop = false;
	//while(!stop)
	//{
	//	int i = waitKey(0);
	//	if ((i>='a' && i<='z')||(i>='A' && i<='Z'))
	//	{
	//		stop = true;
	//	}
	//}

	//return 0;

	//---------------------视频------------------------
	//设置视频图像处理间隔时间
	double dTime = 1;
	cout<<"输入n跳到下一时刻"<<endl;
	cout<<"输入视频图像处理间隔时间：";
	cin>>dTime;

	//打开视频
	cout<<"输入AVI视频名称：";
	cin>>orgName;
	VideoCapture cap(orgName+".mp4");	  
	if(!cap.isOpened()){cout<<"视频读取错误"<<endl;system("puase");return -1;}
	double rate=cap.get(CV_CAP_PROP_FPS);//获取帧率

	while(1)//输入按键退出程序
	{
		//获取视频下一帧
		if(!cap.read(org))
		{
			break;
		}
		org.copyTo(img);
		namedWindow("img",WINDOW_NORMAL);
		setMouseCallback("img",on_mouse,0);//调用回调函数
		imshow("img",img);


		//等待按键输入
		bool stop = false;
		while(!stop)
		{
			int n = waitKey(0);
			if (n=='n'||n=='N')//跳到下一时刻
			{
				for (int i = 0;i<dTime*rate;i++)
				{
					if(!cap.read(org))
					{
						return 0;
					}
				}
				stop = true;
			} 
			else if ((n>='a' && n<='z')||(n>='A' && n<='Z'))
			{
				stop = true;
				return 0;
			}
		}
	}


	return 0;

}