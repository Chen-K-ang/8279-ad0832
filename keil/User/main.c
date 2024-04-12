#include "ADC0832.h"
#include "LCD1602.h"
#include "U8279.h"
#include "delay.h"
#include "time.h"

#define Q_LIMIT 100

sbit BUZZER = P3^0; //��Դ������
sbit RELAY = P3^1;  //�̵���

void main(void)
{
	/****˲ʱ��������****/
	float ad_show_Q = 0;
	unsigned char ad_result_Q = 0, ad_sampling_cnt = 0, Q_str[5] = {0};
	int ad_filiter_Q = 0, ad_Q = 0;

	/****�ۼ���������****/
	float Q_num = 0;
	unsigned char Q_num_str[5] = {0};
	
	/****����������ʾ****/
	unsigned char Q_limit = Q_LIMIT, Q_limit_str[5] = {0},
			Q_dispaly_flag = 1, limit_len;
	
	/****��������****/
	bit stop_flag = 0;
	
	/****�豸��ʼ��****/
	U8279_init();
	ADC0832_init();
	LCD1602_init();
	time0_init();
	BUZZER = 0;
	RELAY = 1;
	delay_ms(10);
	LCD1602_show_str(0, 0, "Q:      cc/s");
	LCD1602_show_str(0, 1, "Qs:     cc");
	LCD1602_show_str(11, 1, "L:");
	while(1) 
	{
		/****˲ʱ�����ɼ�****/
		ad_result_Q = ADC0832_conv(0x01);
		ad_filiter_Q += ad_result_Q;
		ad_sampling_cnt++;
		if (ad_sampling_cnt > 9) {
			ad_sampling_cnt = 0;
			/****˲ʱ������������ʾ****/
			ad_Q = ad_filiter_Q * 0.1;
			ad_filiter_Q = 0;
			ad_show_Q = ad_Q * 1.0 / 256 * 5.0;
			sprintf(Q_str, "%.2f", ad_show_Q);
			LCD1602_show_str(3, 0, Q_str);
		}
		
		/****�ۼ�������ʾ****/
		if (time0_flag) {
			time0_flag = 0;
			Q_num += ad_show_Q;
			sprintf(Q_num_str, "%.1f", Q_num);
			LCD1602_show_str(3, 1, Q_num_str);
		}
		
		/****�������������̵����Ͽ���lcd������ʾERR****/
		if (Q_num > Q_limit) {
			RELAY = 0;
			BUZZER = 1;
			LCD1602_show_str(13, 1, "ERR");
		}

		/****��ȡ����****/
WHILE_1:
		if (keyval) {
			switch (keyval) {
				case 0xC0: Q_dispaly_flag = 1; Q_limit += 10; break;
				case 0xC8: Q_dispaly_flag = 1; Q_limit -= 10; break;
				case 0xC1: Q_dispaly_flag = 2; Q_num = 0.0f; break;
				case 0xC9: stop_flag = ~stop_flag; break;
				default: break;
			}
			keyval = 0x00;
		}

		/****����������ʾ****/
		if (Q_dispaly_flag == 1) {
			Q_dispaly_flag = 0;
			limit_len = LCD1602_integer_to_str(Q_limit, Q_limit_str);
			while (limit_len < 3)
				Q_limit_str[limit_len++] = ' ';
			Q_limit_str[limit_len] = '\0';
			LCD1602_show_str(13, 1, Q_limit_str);		
		} else if (Q_dispaly_flag == 2) {
			Q_dispaly_flag = 0;
			LCD1602_show_str(3, 1, "0.0 ");
		} else {
		
		}
		
		/****��ͣ****/
		if(stop_flag) {
			time0_flag = 0;
			goto WHILE_1;		
		}
	}
} 
