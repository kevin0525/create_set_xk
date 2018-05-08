#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <iostream>
#include <conio.h>
#include <strstream>

#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"
using namespace cv;
using namespace std;

//��������
Mat org,img,tmp;//ԭʼͼ�񣬴���ͼ����ʾͼ�񣬽�ȡͼ��
float widthHeightRatio = 1.0;//ͼ����������/����
string orgName;//ͼ������

void on_mouse(int event,int x,int y,int flags,void *ustc)//event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ���  
{
	static Point2i pre_pt,end_pt;//��ȡͼ���ʼ���꣬��������
	Point2i cur_pt = Point2i(x,y);//ʵʱ����
	char ptPos[16];//ָ������
	sprintf(ptPos,"(%d,%d)",x,y);//����ʵʱ������ʾ�ı�
	img.copyTo(tmp);//��img���Ƶ���ʱͼ��tmp�����ϣ�������ʾʵʱ����

	

	if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�������ʲô������
	{
	}
	if (event == CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ����
	{
		pre_pt = Point2i(x,y);//��ȡ��ʼ����
	}  
	if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ�����  
	{
		end_pt = Point2i(cur_pt.x,pre_pt.y+(cur_pt.x-pre_pt.x)/widthHeightRatio);//�����ȡ��������
		//end_pt = Point2i(cur_pt.x, cur_pt.y);
		cv::rectangle(tmp,pre_pt,end_pt,Scalar(255,255,0,0),5,8,0);//����ʾͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
	} 
	if (event == CV_EVENT_LBUTTONUP)//����ɿ�����ͼ���ϻ����β����棬�����ȡ����
	{
		static int i = 0;
		strstream ss;
		string s;

		rectangle(img,pre_pt,end_pt,Scalar(0,255,0,0),1,8,0);//�ڴ���ͼ������ʾ����
		imwrite(orgName+"0.jpg",img);//����img

		ss<<"e"<<i<<".jpg";
		ss>>s;
		cout<<s<<endl;
		Mat outImg = org(Rect(pre_pt,end_pt));
		imwrite(s,outImg);
		i++;
	}

	//��ͼ���Ϸ������� ����Ϊ��ͼƬ�����֣�������ͼ���е����½����꣬�������ͣ���С����ɫ����ϸ������
	putText(tmp,ptPos,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,10);
	imshow("img",tmp); //��ʾͼ��

}

int main()
{
	////---------------------ͼ��------------------------
	////����ͼ������
	//cout<<"����JPGͼ�����ƣ�";
	//cin>>orgName;
	//org = imread(orgName+".jpg");
	//org.copyTo(img);
	//namedWindow("img",WINDOW_NORMAL);
	//setMouseCallback("img",on_mouse,0);//���ûص�����
	//imshow("img",img);

	////���밴���˳�����
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

	//---------------------��Ƶ------------------------
	//������Ƶͼ������ʱ��
	double dTime = 1;
	cout<<"����n������һʱ��"<<endl;
	cout<<"������Ƶͼ������ʱ�䣺";
	cin>>dTime;

	//����Ƶ
	cout<<"����AVI��Ƶ���ƣ�";
	cin>>orgName;
	VideoCapture cap(orgName+".mp4");	  
	if(!cap.isOpened()){cout<<"��Ƶ��ȡ����"<<endl;system("puase");return -1;}
	double rate=cap.get(CV_CAP_PROP_FPS);//��ȡ֡��

	while(1)//���밴���˳�����
	{
		//��ȡ��Ƶ��һ֡
		if(!cap.read(org))
		{
			break;
		}
		org.copyTo(img);
		namedWindow("img",WINDOW_NORMAL);
		setMouseCallback("img",on_mouse,0);//���ûص�����
		imshow("img",img);


		//�ȴ���������
		bool stop = false;
		while(!stop)
		{
			int n = waitKey(0);
			if (n=='n'||n=='N')//������һʱ��
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