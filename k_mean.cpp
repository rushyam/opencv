#include "cv.hpp"
#include "stdio.h"
int cp=0;
Mat img=imread("horse.jpg");
int k;
int coun=0;
struct Info
{
  int i,j,hue,sat,val;
  int index;
};
struct Cen
{
  int hue,sat,val;
};
int print(Info info[],Cen center[]);
int cluster(Info info[],Cen center[]);
int dis(Info info[],int index,Cen center[]);
int mean(Info info[],Cen center[]);
int main()
{
  //cvtColor(img,img,CV_BGR2HSV);
  printf("enter no. of cluster\n");
  scanf("%d",&k);
  Info *info=(Info*)malloc(img.rows*img.cols*sizeof(Info));
  for(int i=0;i<img.rows;i++)
    {
      for(int j=0;j<img.cols;j++)
	{
	  info[coun].i=i;
	  info[coun].j=j;
	  info[coun].hue=img.at<Vec3b>(i,j)[0];
	  info[coun].sat=img.at<Vec3b>(i,j)[1];
	  info[coun].val=img.at<Vec3b>(i,j)[2];
	  coun++;
	}
    }
  Cen center[20];
  for(int i=0;i<k;i++)
    {
      center[i].hue=info[(int)((coun*i)/k)].hue;
      center[i].sat=info[(int)((coun*i)/k)].sat;
      center[i].val=info[(int)((coun*i)/k)].val;
    }
  cluster(info,center);
  return 0;
}
int cluster(Info info[],Cen center[])
{
  cp++;
  for(int i=0;i<coun;i++)
    info[i].index = dis(info,i,center);
  mean(info,center);
  //Mat img1(img.rows,img.cols,CV_8UC3);
  for(int i=0;i<coun;i++)
    {
      img.at<Vec3b>(info[i].i,info[i].j)[0]=center[info[i].index].hue;
      img.at<Vec3b>(info[i].i,info[i].j)[1]=center[info[i].index].sat;
      img.at<Vec3b>(info[i].i,info[i].j)[2]=center[info[i].index].val;
    }
  namedWindow("dsaf",WINDOW_NORMAL);
  imshow("dsaf",img);
  waitKey(0);
    if(cp=7)
    imwrite("road.jpg",img);
  cluster(info,center);

}
int dis(Info info[],int index,Cen center[])
{
  int d[20],copyd[20];
  for(int i=0 ;i<k;i++)
    {
      d[i]=(info[index].hue-center[i].hue)*(info[index].hue-center[i].hue)+(info[index].sat-center[i].sat)*(info[index].sat-center[i].sat)+(info[index].val-center[i].val)*(info[index].val-center[i].val);
      copyd[i]=d[i];
    }
  for(int i=0;i<k;i++)
    {
      for(int j=0;j<k;j++)
	{
	  if(copyd[i]<copyd[j] && i!=j)
	    {
	      int temp=copyd[i];
	      copyd[i]=copyd[j];
	      copyd[j]=temp;
	    }
	}
    }
  for(int i=0;i<k;i++)
    {
      if(copyd[0]==d[i])
	{
	  return i;
	}
    }
}
int mean(Info info[],Cen center[])
{
  int sum_hue[20],sum_sat[20],sum_val[20],count[20];
  for(int i=0;i<k;i++)
    {
      sum_hue[i]=0;
      sum_sat[i]=0;
      sum_val[i]=0;
      count[i]=1;
    }
  for(int i=0;i<coun;i++)
    {
      sum_hue[info[i].index]+=info[i].hue;
      sum_sat[info[i].index]+=info[i].sat;
      sum_val[info[i].index]+=info[i].val;
      count[info[i].index]++;
    }
  for(int i=0;i<k;i++)
    {
      center[i].hue=(int)(sum_hue[i]/count[i]);
      center[i].sat=(int)(sum_sat[i]/count[i]);
      center[i].val=(int)(sum_val[i]/count[i]);
    }
  return 0;
}

