/**
*功能：自动数值分析
*
*/
#include "common.h"
#include "include.h"
#define N 5000                  //要分析点的个数
#define RISE 50                 //上升阶段点数
#define TREND 10

struct wave
{
  int rize;
  int down;
  int offset;
}pid;
struct wave pid=
{
  .rize = 0,
  .down = 0,
  .offset = 10                              //允许上升值
};
  
//PID分析
//上下波动不超过值得10%，当符合要求时优化精度
//不符合要求的点的个数小于总点数的10%为满足
//return ： 0为符合要求，1为有待改进
int PID(float *p ,float *i ,float *d ,float value)
{
  *i = *d = 0;
  
  static int count[4]={0};                 //标志不符合要求的点的个数、当前的点数,,上1下2升降趋势,
//  static int 
  static float history[2] = {0};                //传入值的历史，计数历史
  int edited_p=0,edited_i=0,edited_d=0;
  if(count[1] > RISE)
  {
    if(count[1]<N)
    {
      if(value < history[0] * 1.1 && value > history[0] * 0.9)            //20%的波动
      {;}
      else                                                              //捕获第一个下降点
      {
        if(history[0] > value)
        {
          if(!count[2])
            count[3] = 2;
          count[2] ++;
          if(count[2] > TREND)
          {
            pid.down ++;
            count[2] = 0;
          }
        }
        else if(history[0] < value + pid.offset)
        {
          if(!count[2])
            count[3] = 1;
          count[2] ++;
          if(count[2] > TREND)
          {
            
            count[2] = 0;
          }
        }
        count[0] ++;
      }
      
      if(history[1] == count[3])
      {
        ;//待写
      }
      else
      {
        switch(count[3])
        {
          case 1:pid.rize ++;break;
          case 2:pid.down ++;break;
          default:;;
        }
        history[1] = count[3];
      }
      count[1] ++;
    }
    else                                        //超过分析点
    {
      pid.rize = 0;
      pid.down = 0;
      count[2] = 0;
      if(count[0] > N * 0.1)
        return 0;
    }
  }
  else
  {
    pid.rize = 0;
    pid.down = 0;
    count[2] = 0;
    count[1] ++;
  }
  
  /**
  *     跑完之后查看rize down的值
  */
  if(!*i)
  {
    if(pid.rize < 3)
    {
      *p = *p + 0.01;
      if(pid.rize >= 4)
      {
          *p *= 0.75;                             //0.75取p值
          edited_p = 1;
      }
    }
    else
    {
      *p = *p - 0.01;
      if(pid.rize <6)
      {
          *p *= 0.75;                             //0.75取p值
          edited_p = 1;
      } 
    }
  }
  else if(edited_p && !edited_i)
  {
    if(pid.rize < 3)
    {
      *i = *i + 0.01;
      if(pid.rize >= 4)
      {
          *i *= 1.3;                             //0.75取p值
          edited_i = 1;
      }
    }
    else
    {
      *i = *i - 0.01;
      if(pid.rize < 6)
      {
          *i *= 1.3;                             //0.75取p值
          edited_i = 1;
      } 
    }
  }
  else if(edited_i && !edited_d)
  {
    if(pid.rize < 3)
    {
      *d = *d + 0.01;
      if(pid.rize >= 4)
      {
          *d *= 0.4;                             //0.75取p值
          edited_d = 1;
      }
    }
    else
    {
      *d = *d - 0.01;
      if(pid.rize < 6)
      {
          *d *= 1.3;                             //0.75取p值
          edited_d = 1;
      } 
    }
  }
  
  history[0] = value;
  return 1;
}