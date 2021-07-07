#include "MySTM32F303VC.h"
#include <stdio.h>
#include <math.h>

#define RCC_APB1ENR (unsigned int*) 0x4002101C
#define TIM2_CR1 (unsigned int*) 0x40000000
#define TIM2_CR2 (unsigned int*) 0x40000004
#define TIM2_CNT (unsigned int*) 0x40000024
#define TIM2_ARR (unsigned int*) 0x4000002C
#define TIM2_SR (unsigned int*) 0x40000010

#define Tclk 0.000000125 //Tclk=1/Fclk

#define N_ARR_1 313
#define N_ARR_2 800


//Registri ADC

#define ADC12_CCR (unsigned int*) 0x50000308
#define ADC1_CR (unsigned int*) 0x50000008
#define ADC1_DIFSEL (unsigned int*) 0x500000B0
#define ADC1_ISR (unsigned int*) 0x50000000
#define ADC1_SMPR1 (unsigned int*) 0x50000014
#define ADC1_SQR1 (unsigned int*) 0x50000030
#define ADC1_DR (unsigned int*) 0x50000040

#define CKMODE_1 (unsigned int) 1<<17
#define CKMODE_0 (unsigned int) 1<<16

//Bit ADC1_CR

#define ADVREGEN_0 (unsigned int) 1<<28
#define ADVREGEN_1 (unsigned int) 1<<29

#define ADEN (unsigned int) 1<<0
#define DIFSEL_1 (unsigned int) 1<<1
#define ADCAL (unsigned int) 1<<31
#define ADSTART (unsigned int) 1<<2

//Bit ADC1_ISR
#define EOC (unsigned int) 1<<2
#define ADRDY (unsigned int) 1<<0

#define L_0 (unsigned int) 1<<0
#define L_1 (unsigned int) 1<<1
#define L_2 (unsigned int) 1<<2
#define L_3 (unsigned int) 1<<3

#define SQ1_0 (unsigned int) 1<<6
#define SQ1_1 (unsigned int) 1<<7
#define SQ1_2 (unsigned int) 1<<8
#define SQ1_3 (unsigned int) 1<<9
#define SQ1_4 (unsigned int) 1<<10

#define SMP1_0 (unsigned int) 1<<3
#define SMP1_1 (unsigned int) 1<<4
#define SMP1_2 (unsigned int) 1<<5

//Registri DAC
#define DAC_CR (unsigned int*) 0x40007400
#define DAC_SWTRIGR (unsigned int*) 0x40007404
#define DAC_DHR12R1 (unsigned int*) 0x40007408

#define EN1 (unsigned int) 1<<0
#define BOFF1 (unsigned int) 1<<1
#define TEN1 (unsigned int) 1<<2
#define TSEL1_0 (unsigned int) 1<<3
#define TSEL1_1 (unsigned int) 1<<4
#define TSEL1_2 (unsigned int) 1<<5
#define DMAEN1 (unsigned int) 1<<12

#define SWTRIG1 (unsigned int) 1<<0
#define MMS_0 (unsigned int) 1<<4
#define MMS_1 (unsigned int) 1<<5
#define MMS_2 (unsigned int) 1<<6
#define DMA2_CPAR3 (unsigned int*) 0x40020438
#define DMA2_CMAR3 (unsigned int*) 0x4002043C
#define DMA2_CCR3 (unsigned int*) 0x40020430
#define DMA2_CNDTR3 (unsigned int*) 0x40020434
#define DMA2_ISR (unsigned int*) 0x40020400
#define DMA2_IFCR (unsigned int*) 0x40020404
//define del DMA
#define EN (unsigned int) 1<<0
#define DIR (unsigned int) 1<<4
#define CIRC (unsigned int) 1<<5
#define PINC (unsigned int) 1<<6
#define MINC (unsigned int) 1<<7
#define PSIZE_0 (unsigned int) 1<<8
#define PSIZE_1 (unsigned int) 1<<9
#define MSIZE_0 (unsigned int) 1<<10
#define MSIZE_1 (unsigned int) 1<<11
#define PL_0 (unsigned int) 1<<12
#define PL_1 (unsigned int) 1<<13
unsigned int* punt;
float Volt;

unsigned int y[256];
int i=0;

float x,dx;
#define pi 3.14159265358979

float v[256];
float x1;
float z[20];

void main(){

//Configurazione RCC
RCC->IOPAEN=1;
RCC->IOPEEN=1;
RCC->TIM2EN=1;
punt=RCC_APB1ENR;
*punt|=(1<<29);//abilito timer2// ABILITO IL dac


RCC->ADC12=1; //Abilito ADC12
RCC->DMA2EN=1;
//Configurazione I/O

GPIOA->MODER0=Analog;//Input;
GPIOA->MODER1=Analog;//Input;

GPIOA->MODER2=Analog;//Input; //CH3_ADC
GPIOA->MODER4=Analog;//Onput;// DAC1

//Configurazione CLK ADC12
punt=ADC12_CCR;
*punt|=CKMODE_0;
*punt&=~(CKMODE_1);

//Voltage Regulator
punt=ADC1_CR;
*punt&=~(ADVREGEN_1);
*punt|=ADVREGEN_0;
/*
  per attendere 10microsecondi
  delta_T = 10*10^-6 secondi = ARR * Tclk
  ARR = delta_T/Tclk = 80
  NOTA:non devo attendere un update event poiche' il registro ARR
  non e' bufferizzato
  */
  
  TIM2->ARR=80; 
  TIM2->CEN=1;
  while((TIM2->UIF)==0);
  TIM2->CEN=0;
  TIM2->UIF=0;

//Single-Ended
*punt&=~(ADEN);
punt=ADC1_DIFSEL;
*punt&=~(DIFSEL_1);

//Autocalibrazione
punt=ADC1_CR;
*punt|=ADCAL;
while(((*punt)&ADCAL)==ADCAL);

//Accensione ADC
*punt|=ADEN;
punt=ADC1_ISR;
while(((*punt)&ADRDY)!=ADRDY);

//Tempo di Sample
punt=ADC1_SMPR1;
*punt|=SMP1_2|SMP1_1|SMP1_0;

//Sequenza regolare
punt=ADC1_SQR1;
*punt&=~(L_0); //1 Conversione
*punt&=~(L_1);
*punt&=~(L_2);
*punt&=~(L_3);


//Selezione del Canale 3 per PA2
*punt|=SQ1_0;
*punt|=SQ1_1;
*punt&=~(SQ1_2);
*punt&=~(SQ1_3);
*punt&=~(SQ1_4);

//CONFIGURAZIONE DAC

punt=DAC_CR;
*punt|=(BOFF1);// Amplificatore di uscita disattivato
*punt|=TEN1; 
*punt&=~(TSEL1_0);//TRIGGER DA TIMER2 100
*punt&=~(TSEL1_1);
*punt|=TSEL1_2;
*punt|=DMAEN1;
*punt|=EN1;



x=0.0;
dx=(2*pi)/256;

for(i=0;i<256;i++){
  
  y[i]=(unsigned int)(2047*sin(x)+2048);
   //Vmax=1.5V --> 2047
 //Voffset=1.5V --> 2048
  x=x+dx;
}

//configurazione DMA2
//set registro CPAR
punt=DMA2_CPAR3;
*punt=(unsigned int) DAC_DHR12R1; //devo mettere l'indirizzo di DHR12R1
//devo mettere un valore, non un puntatore, quindi faccio un type-casting ad unsigned int


punt=DMA2_CMAR3;
*punt=(unsigned int) &y[0]; //lo posso verificare col live-watch

punt=DMA2_CNDTR3;
*punt=256; //il vettore è di 100 elementi

punt=DMA2_CCR3;
*punt|=PL_1|PL_0; //PRIORITA' MASSIMA
*punt&=~(MSIZE_0); //MSIZE[1:0]=10 32 bit
*punt|=MSIZE_1;

*punt&=~(PSIZE_0); //PSIZE[1:0]=10 32 bit
*punt|=PSIZE_1;

*punt|=MINC; //INCREMENTO LA MEMORIA
*punt&=~(PINC); //non incremento nella periferica, posso anche non metterlo

*punt|=CIRC; //BUFFER CIRCOLARE
*punt|=DIR; //DIREZIONE 

*punt|=EN; //Abilito il DMA

//Configurazione TIMER2 
TIM2->ARR=N_ARR_1;
TIM3->ARR=N_ARR_2;

punt=TIM2_CR2;
*punt&=~(MMS_0); //generazione dell'evento di trigger all'update event MMS[2:0]=010
*punt|=MMS_1;
*punt&=~(MMS_2);
TIM2->CEN=1;
for(int j=0;j<20;j++){

do{

 
if(i==0) TIM3->CEN=1;
         
         
         punt=ADC1_CR;
         *punt|=ADSTART;
         
         punt=ADC1_ISR;
         while(((*punt)&EOC)!=EOC);

         punt=ADC1_DR;

         v[i]=(float) (((*punt)*3.0)/4095.0);
         
         while((TIM3->UIF)!=1);
		 TIM3->UIF=0;
         
        
i++;

}while(i<256);
TIM3->CEN=0;
x1=0.0;
for(i=0;i<256;i++) x1+=((v[i])*(v[i]));
z[j]=sqrt(x1/256);
}

float somma;
float SD;
for(int k=0;k<20;k++) somma+= z[k];
float media=somma/20;
for (int k = 0; k < 20; k++)
        SD += pow(z[k] - media, 2);
float deviazione= sqrt(SD / 19);
}//end main
