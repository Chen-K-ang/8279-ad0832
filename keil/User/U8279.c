#include "U8279.h"

void U8279_init(void)
{
	COM8279 = 0x00;  //��8λ��ʾ��������̣�˫������

	COM8279 = 0x34;  //20��Ƶ����100KHzʱ��

	COM8279 = 0xD1;  //���ڴ��FIFO����

	EX0 = 1; //�����ж�,���ⲿ�ж�0
	IT0 = 1;
	EA  = 1;
}

unsigned char keyval = 0x00;

void U8279_ISR(void) interrupt 0
{
	COM8279 = 0x40;  //��FIFO�Ĵ�������ַ������

	keyval = DAT8279; //����ֵ

	COM8279 = 0xD1;  //���ڴ��FIFO����

}
