#include "MySTM32F303VC.h"
#include <math.h>
#include <stdio.h>
#define RCC_APB1ENR (unsigned int*) 0x4002101C
#define TIM2_CR (unsigned int*) 0x40000000
#define TM2_CNT (unsigned int*) 0x40000024
#define TM2_ARR (unsigned int*) 0x4000002C
#define TM2_SR (unsigned int*) 0x40000010

//indirizzi registri ADC
#define ADC12_CCR (unsigned int*) 0x50000308
//(5000000+300+8)
#define ADC1_CR (unsigned int*) 0x50000008
#define ADC1_CFGR (unsigned int*) 0x5000000C
#define ADC1_DIFSEL (unsigned int*) 0x500000B0
#define ADC1_ISR (unsigned int*) 0x50000000
#define ADC1_SMPR1 (unsigned int*) 0x50000014
#define ADC1_SQR1 (unsigned int*) 0x50000030
#define ADC1_DR (unsigned int*) 0x50000040
//maschere bit per ADC
#define CKMODE_0 (unsigned int) 1<<16
#define CKMODE_1 (unsigned int) 1<<17

#define ADVREGEN_0 (unsigned int) 1<<28
#define ADVREGEN_1 (unsigned int) 1<<29

#define ADEN (unsigned int) 1<<0
#define ADSTART (unsigned int) 1<<2
#define ADCAL (unsigned int) 1<<31

#define ADRDY 1<<0
#define EOC (unsigned int) 1<<2

#define DIFSEL_1 (unsigned int) 1<<1

#define SMP1_0 (unsigned int) 1<<3
#define SMP1_1 (unsigned int) 1<<4
#define SMP1_2 (unsigned int) 1<<5

#define L_0 (unsigned int) 1<<0
#define L_1 (unsigned int) 1<<1
#define L_2 (unsigned int) 1<<2
#define L_3 (unsigned int) 1<<3

#define SQ1_0 (unsigned int) 1<<6
#define SQ1_1 (unsigned int) 1<<7
#define SQ1_2 (unsigned int) 1<<8
#define SQ1_3 (unsigned int) 1<<9
#define SQ1_4 (unsigned int) 1<<10

#define CONT (unsigned int) 1<<13
#define Tclk 0.000000125 //Tclk=1/Fclk (8MHz)
#define N_ARR 8000000//per contare 1s alla frequenza Fclk di 8MHz

float volt;
unsigned int* punt;
void main(){
//configurazione RCC
RCC->IOPAEN=1;
RCC->IOPEEN=1;
//punt=RCC_APB1ENR;
//*punt|=1; // abilito timer 2
RCC->TIM2EN=1;

//abilito ADC12 da RCC
RCC->ADC12=1;

//configurazione I/O
GPIOE->MODER9=Output;
GPIOA->MODER0=Analog; //abilito PA0 come analogico

//configurazione CKMODE
punt=ADC12_CCR;
*punt|=CKMODE_0;
*punt&=~(CKMODE_1); //devo mettere le parentesi davanti al negato

//abilito il regolatore di tensione
punt=ADC1_CR;
*punt&=~(ADVREGEN_1);
*punt|=ADVREGEN_0;
//devo ritardare ARR ESERCIZIO
for(int i=0;i<1000000;i++);

//selezionare acquisizione single-ended oppure differenziale
*punt&=~(ADEN);
punt=ADC1_DIFSEL;
*punt&=~(DIFSEL_1);

//eseguire la calibrazione
punt=ADC1_CR;
*punt|=ADCAL;
while(((*punt)&ADCAL)==ADCAL); //esco dal while quando la calibrazione è finita

//accendo l'ADC
*punt|=ADEN;
punt=ADC1_ISR;
while(((*punt)&ADRDY)!=ADRDY);//dobbiamo aspettare che si alzi

//imposto il time sampling
punt=ADC1_SMPR1;
*punt|=SMP1_0 | SMP1_1 | SMP1_2;

//seleziono la sequenza di conversione
punt=ADC1_SQR1;
*punt&=~(L_0);
*punt&=~(L_1);
*punt&=~(L_2);
*punt&=~(L_3);

*punt|=SQ1_0;
*punt&=~(SQ1_1);
*punt&=~(SQ1_2);
*punt&=~(SQ1_3);
*punt&=~(SQ1_4);

//configurazione timer
TIM2->ARR=N_ARR;
//quando mi arriva il clock, il counter si precaricherà ad 8000000 
//(quando succede questo, vuol dire che è passato 1 secondo)

TIM2->CEN=1;
punt=ADC1_CFGR;
*punt|=CONT;
//start of conversion
 punt=ADC1_CR;
 *punt|=ADSTART;
//aspetto end of conversion
  //punt=ADC1_ISR;
  //while(((*punt)&EOC)!=EOC);//aspetto la fine della vonersione
//leggo il data register
 while(1){
  punt=ADC1_DR;
  volt=(float) ((*punt)*3.0)/4095.0;
  printf("%.2f V\n", volt);
  
  
//devo aspettare la fine del conteggio che ho avviato (l'update event)
//devo aspettare TM2_CR_UIF=1
//poi devo resettare TM2_CR_UIF=0
while((TIM2->UIF)!=1);
TIM2->UIF=0;

}//end while

}//end main 