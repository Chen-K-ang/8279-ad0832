#include "U8279.h"

void U8279_init(void)
{
	COM8279 = 0x00;  //左8位显示，编码键盘，双键锁定

	COM8279 = 0x34;  //20分频产生100KHz时钟

	COM8279 = 0xD1;  //清内存和FIFO数据

	EX0 = 1; //开总中断,开外部中断0
	IT0 = 1;
	EA  = 1;
}

unsigned char keyval = 0x00;

void U8279_ISR(void) interrupt 0
{
	COM8279 = 0x40;  //读FIFO寄存器，地址不增量

	keyval = DAT8279; //读键值

	COM8279 = 0xD1;  //清内存和FIFO数据

}
