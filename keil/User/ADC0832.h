#ifndef HARDWARE_ADC0832_H
#define HARDWARE_ADC0832_H

#include <reg52.h>
#include <intrins.h>

sbit ADC0832_CS  = P3^3;
sbit ADC0832_CLK = P3^4;
sbit ADC0832_DI0 = P3^5;

void ADC0832_init(void);
unsigned char ADC0832_conv(unsigned char channel);

#endif
