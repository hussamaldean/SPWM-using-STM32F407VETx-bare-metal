#include "stm32f4xx.h"                  // Device header
void GPIO_Init(void);
void Timer2_init(void);
void Timer3_init(void);
long map(long x, long in_min, long in_max, long out_min, long out_max);
void delay(int delayms );

uint8_t sinus[]={0,8,16,23,31,39,47,55,63,71,78,86,93,101,108,115,122,129,
136,143,149,156,162,168,174,180,185,191,196,201,206,210,215,219,223,
227,230,234,237,239,242,244,246,248,250,251,252,253,254,254,255,254,
254,253,252,251,250,248,246,244,242,239,237,234,230,227,223,219,215,
210,206,201,196,191,185,180,174,168,162,156,149,143,136,129,122,115,
108,101,93,86,78,71,63,55,47,39,31,23,16,8,0};

volatile int x,dir;
#define rate 1599

int main(void)
{
__disable_irq();
GPIO_Init();
Timer2_init();
Timer3_init();
__enable_irq();
while(1)
{
if(dir==1){
TIM2->CCR1=map(sinus[x],0,255,0,rate);
TIM2->CCR2=0;
}
else{TIM2->CCR2=map(sinus[x],0,255,0,rate);
TIM2->CCR1=0;}
}
}

void GPIO_Init(void)
{
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
//RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;	
GPIOA->AFR[0]|=0x11;
//GPIOA->AFR[0]|=GPIO_AFRL_AFSEL1_0;
GPIOA->MODER|=0xA;
//GPIOA->MODER|=GPIO_MODER_MODER1_1;
GPIOA->OSPEEDR|=0xF;
}
void  Timer2_init(void){
RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
TIM2->PSC=0;
TIM2->ARR=rate;
TIM2->CNT=0;
TIM2->CCMR1=0x6060;
TIM2->CCER|=0x11;
TIM2->CR1=1;
TIM2->CCR1=0;
}

void Timer3_init(void){
RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;
TIM3->PSC=0;
TIM3->ARR=1599;
TIM3->CR1|=0x01;
TIM3->DIER|=0x01;
NVIC_EnableIRQ(TIM3_IRQn);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



void TIM3_IRQHandler(void){
TIM3->SR=0;
if(++x>sizeof(sinus)){x=0;dir=!dir;}


}
