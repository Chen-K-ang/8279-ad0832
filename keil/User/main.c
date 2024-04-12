#include "ADC0832.h"
#include "LCD1602.h"
#include "U8279.h"
#include "delay.h"
#include "time.h"

#define Q_LIMIT 100

sbit BUZZER = P3^0; //有源蜂鸣器
sbit RELAY = P3^1;  //继电器

void main(void)
{
	/****瞬时流量参数****/
	float ad_show_Q = 0;
	unsigned char ad_result_Q = 0, ad_sampling_cnt = 0, Q_str[5] = {0};
	int ad_filiter_Q = 0, ad_Q = 0;

	/****累计流量参数****/
	float Q_num = 0;
	unsigned char Q_num_str[5] = {0};
	
	/****流量限制显示****/
	unsigned char Q_limit = Q_LIMIT, Q_limit_str[5] = {0},
			Q_dispaly_flag = 1, limit_len;
	
	/****其他参数****/
	bit stop_flag = 0;
	
	/****设备初始化****/
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
		/****瞬时流量采集****/
		ad_result_Q = ADC0832_conv(0x01);
		ad_filiter_Q += ad_result_Q;
		ad_sampling_cnt++;
		if (ad_sampling_cnt > 9) {
			ad_sampling_cnt = 0;
			/****瞬时流量处理与显示****/
			ad_Q = ad_filiter_Q * 0.1;
			ad_filiter_Q = 0;
			ad_show_Q = ad_Q * 1.0 / 256 * 5.0;
			sprintf(Q_str, "%.2f", ad_show_Q);
			LCD1602_show_str(3, 0, Q_str);
		}
		
		/****累计流量显示****/
		if (time0_flag) {
			time0_flag = 0;
			Q_num += ad_show_Q;
			sprintf(Q_num_str, "%.1f", Q_num);
			LCD1602_show_str(3, 1, Q_num_str);
		}
		
		/****蜂鸣器报警，继电器断开，lcd右下显示ERR****/
		if (Q_num > Q_limit) {
			RELAY = 0;
			BUZZER = 1;
			LCD1602_show_str(13, 1, "ERR");
		}

		/****读取按键****/
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

		/****流量限制显示****/
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
		
		/****暂停****/
		if(stop_flag) {
			time0_flag = 0;
			goto WHILE_1;		
		}
	}
} 
