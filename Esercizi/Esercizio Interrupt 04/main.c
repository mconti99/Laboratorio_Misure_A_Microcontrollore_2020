#include <stm32f30x.h>
#include <math.h>
#include <stdio.h>

#define NARR3 3200

#define RCC_AHBENR (unsigned int *) 0x40021014
#define RCC_APB1ENR (unsigned int *) 0x4002101C
#define RCC_APB2ENR (unsigned int *) 0x40021018
#define GPIOA_MODER (unsigned int*) 0x48000000
#define ADC12_CCR (unsigned int*) 0x50000308
#define ADC1_CR (unsigned int*) 0x50000008
#define ADC1_DIFSEL (unsigned int*) 0x500000B0
#define ADC1_ISR (unsigned int*) 0x50000000
#define ADC1_SQR1 (unsigned int*) 0x50000030
#define ADC1_SMPR1 (unsigned int*) 0x50000014
#define ADC1_CFGR (unsigned int*) 0x5000000C
#define ADC1_DR (unsigned int*) 0x50000040
#define DAC_CR (unsigned int*) 0x40007400
#define DMA1_CPAR1 (unsigned int*) 0x40020010
#define DMA1_CMAR1 (unsigned int*) 0x40020014
#define DMA1_CNDTR1 (unsigned int*) 0x4002000C
#define DMA1_CCR1 (unsigned int*) 0x40020008
#define DMA1_ISR (unsigned int*) 0x40020000
#define DMA1_IFCR (unsigned int*) 0x40020004
#define DMA2_CPAR3 (unsigned int*) 0x40020438
#define DMA2_CMAR3 (unsigned int*) 0x4002043C
#define DMA2_CNDTR3 (unsigned int*) 0x40020434
#define DMA2_CCR3 (unsigned int*) 0x40020430
#define DAC_DHR12R1 (unsigned int*) 0x40007408
#define SYSCFG_CFGR1 (unsigned int*) 0x40010000;
#define TIM2_CR1 (unsigned int*) 0x40000000
#define TIM2_CR2 (unsigned int*) 0x40000004
#define TIM2_ARR (unsigned int*) 0x4000002C
#define TIM2_SR (unsigned int*) 0x40000010
#define TIM3_CR1 (unsigned int*) 0x40000400
#define TIM3_CR2 (unsigned int*) 0x40000404
#define TIM3_ARR (unsigned int*) 0x4000042C
#define TIM3_SR (unsigned int*) 0x40000410
#define ADCAL (unsigned int) (1<<31)
unsigned int * punt;
unsigned int v[150];
unsigned int y[667];
float z[667];
int i;
int NARR2;
int a;
int somma;
float media;
int temp;
#define pi 3.14
void main(){
  //RCC
 punt=RCC_AHBENR;
 *punt|=(1<<0)|(1<<1)|(1<<17)|(1<<28);
 punt=RCC_APB1ENR;
 *punt|=(1<<0)|(1<<1)|(1<<29);
 punt=RCC_APB2ENR;
 *punt|=(1<<0);
 //GPIO
 punt=GPIOA_MODER;
 *punt&= ~(1<<0);
 *punt&= ~(1<<1);
 *punt|=(1<<8)|(1<<9)|(1<<4)|(1<<5);
//GENERAZIONE SEGNALE 
 for(i=0;i<150;i++) 
   v[i]=(unsigned int) ((1.0+sin(2*pi*i/150.0)*1.5)*(4095.0/3.0));
     i=0;
 //ADC
 punt=ADC12_CCR;
 *punt|=(1<<16);
 punt=ADC1_CR;
 *punt &= ~(1<<29);
 *punt |=(1<<28);
 for(int i=0;i<7200000;i++);
 punt=ADC1_DIFSEL;
 *punt &= ~(1<<3);
 punt=ADC1_CR;
 *punt|=ADCAL;
 while(((*punt)&ADCAL)==ADCAL);//devo aspettare fin tanto che è 1
 *punt|=(1<<0);
 punt=ADC1_ISR;
 while(((*punt)&(1<<0))!=(1<<0));
 punt=ADC1_SQR1;
 *punt|=(1<<6)|(1<<7);
 punt=ADC1_SMPR1;
 *punt|=(1<<9)|(1<<10)|(1<<11);
 punt=ADC1_CFGR;
 *punt|=(1<<0);
 *punt|=(1<<10)|(1<<9)|(1<<7)|(1<<6)|(1<<1);
 //SYSCFG
 punt=SYSCFG_CFGR1;
 *punt|=(1<<7);
 //DAC1
 punt=DAC_CR;
 *punt|=(1<<2)|(1<<3)|(1<<12)|(1<<1)|(1<<0);
 //DMA1 CANALE 1 (ADC)
 punt=DMA1_CPAR1;
 *punt= (unsigned int) ADC1_DR;
 punt=DMA1_CMAR1;
 *punt=(unsigned int)&y[0];
 a=7;
 punt=DMA1_CNDTR1;
 *punt=a;
 punt=DMA1_CCR1;
 *punt|=(1<<5)|(1<<7)|(1<<1)|(1<<2)|(1<<12)|(1<<13)|(1<<9)|(1<<11)|(1<<0);
 //DMA2 CANALE 3 (DAC)
 punt=DMA2_CPAR3;
 *punt=(unsigned int) DAC_DHR12R1; 
  punt=DMA2_CMAR3;
 *punt=(unsigned int)&v[0];
 punt=DMA2_CNDTR3;
 *punt=150;
 punt=DMA2_CCR3;
 *punt|=(1<<5)|(1<<7)|(1<<12)|(1<<13)|(1<<9)|(1<<11)|(1<<4)|(1<<0);
 //TIMER3 (DAC)
 punt=TIM3_ARR;
 *punt=NARR3;
 punt=TIM3_CR2;
 *punt|=(1<<5);
 //TIMER2 (ADC)
 NARR2= 72000;
 punt=TIM2_ARR;
 *punt=NARR2;
 punt=TIM2_CR2;
 *punt|=(1<<5);
 //INTERRUZIONI
 NVIC->ISER[0]|=(1<<6)|(1<<11);
 EXTI->IMR|=(1<<0);
 EXTI->RTSR|=1;
 //ADSTART
 punt=ADC1_CR;
 *punt|=(1<<2);

 //ATTIVO I TIMER
 punt=TIM3_CR1;
 *punt|=1;
 punt=TIM2_CR1;
 *punt|=1;
 for(int j=0;j<72000000;j++);
 while(1);
}

void EXTI0_IRQHandler(){
  EXTI->PR|=(1<<0);
  if(NARR2==72000){
    NARR2=14400;
    a=34;
  }
  else if(NARR2==14400){
    NARR2=7200;
    a=67;
  }
  else if(NARR2==7200){
    NARR2=2880;
    a=167;
  }
  else if(NARR2==2880){
    NARR2=1440;
    a=334;
  }
  else if(NARR2==1440){
    NARR2=720;
    a=667;
  }
  else if(NARR2==720){
    NARR2=72000;
    a=7;
  }
  else printf("errore\n");
punt=TIM2_CR1;
*punt &= ~(1<<0);
  punt=DMA1_CCR1;
  *punt &=~(1<<0);
   punt=DMA1_CNDTR1;
 *punt=a;
  punt=TIM2_ARR;
 *punt=NARR2;
 punt=DMA1_CCR1;
 *punt |=(1<<0);
 punt=ADC1_CR;
 *punt|=(1<<2);
 punt=TIM2_CR1;
 *punt |=(1<<0);
 
}
int t;
void DMA1_Channel1_IRQHandler(){

  
  punt=DMA1_ISR;
  t=*punt;
  if((t&(1<<1))==(1<<1)){
    punt=DMA1_IFCR;
    *punt|=(1<<1);
    *punt &= ~(1<<1);
    somma=0;
    for(int j=0;j<a;j++){
      somma += (y[j]*3.0/4095.0);
    }
    temp=0;
    media=0.0;
    for(int j=0;j<a;j++){
      temp += y[j]*(3.0/4095.0)*y[j]*(3.0/4095.0);
    }
    media= (float) somma/a;
    printf("media: %.2f\n", media);
    printf("veff: %.2f\n", sqrt(temp/a));
  }
}