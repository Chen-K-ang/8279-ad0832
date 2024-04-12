#include "time.h"

/*******T0计时中断函数

	1s的定时次数= 1/(12/12000000)= 1000000, 16位计数器最大可计数65536次
	所以通常定时ms为单位，比如定时10ms，则循环100次为1s，则计数次数为1000000/100=10000，则初值为
	TH0 = (65535-10000)/256;  //重新赋初值
	TL0 = (65535-10000)%256;
*****/

void time0_init(void)		//10毫秒@12.000MHz
{
	TMOD &= 0xF0;		//设置定时器模式  选择定时器0
	TMOD |= 0x01;		//设置定时器模式  0001   M1M0 01 ==> 方式1
	
	TL0 = 0xF0;		//设置定时初值
	TH0 = 0xD8;		//设置定时初值

	TF0 = 0; 		//清除TF0标志
	ET0 = 1;
	EA = 1;
	TR0 = 1;		//定时器0开始计时
}

bit time0_flag = 0;

void time0_ISR(void) interrupt 1
{
	static unsigned char count = 0;
	TR0 = 0; 
	TL0 = 0xF0;
        TH0 = 0xD8;
	
	count++;
	//T = 1s清零
	if (count >= 100){
		count = 0;
		time0_flag = 1;
	}
	
	TR0 = 1; //开启T0
}
