#include "MySTM32F303VC.h"
#include <stdio.h>

#define RCC_APB1ENR (unsigned int*) 0x4002101C
#define TIM2_CR (unsigned int*) 0x40000000
#define TIM2_CNT (unsigned int*) 0x40000024
#define TIM2_ARR (unsigned int*) 0x4000002C
#define TIM2_SR (unsigned int*) 0x40000010

#define Tclk 0.000000125 //Tclk=1/Fclk
#define N_ARR 1000000 //più lo abbasso, più conto velocemente

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

#define SWTRIG1 (unsigned int) 1<<0


unsigned int* punt;
float Volt;

unsigned int v[4096];
unsigned int cont=0;

void main(){

//Configurazione RCC
RCC->IOPAEN=1;
RCC->IOPEEN=1;
RCC->TIM2EN=1;
punt=RCC_APB1ENR;
*punt|=(1<<29);//abilito timer2// ABILITO IL dac


RCC->ADC12=1; //Abilito ADC12

//Configurazione I/O

GPIOE->MODER9=Output;
GPIOA->MODER0=Analog;//Input;
GPIOA->MODER1=Analog;//Input;

GPIOA->MODER2=Analog;//Input; //CH3_ADC
GPIOA->MODER4=Analog;//Ouput;// DAC1

//Configurazione CLK ADC12
punt=ADC12_CCR;
*punt|=CKMODE_0;
*punt&=~(CKMODE_1);

//Voltage Regulator
punt=ADC1_CR;
*punt&=~(ADVREGEN_1);
*punt|=ADVREGEN_0;
for(int i=0;i<1000000;i++);

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
*punt|=TEN1; 
*punt|=TSEL1_0|TSEL1_1|TSEL1_2; 
*punt|=EN1;

/*punt=DAC_DHR12R1;
*punt=2048;
*punt=1024; //voglio 0.75v in uscita
*punt=4095; //voglio 3v in uscita

punt=DAC_SWTRIGR;
*punt|=SWTRIG1;
while(((*punt)&SWTRIG1)==SWTRIG1);
//DO UN CODICE IN ENTRATA DAL DAC E OTTENGO LA Vout ANALOGICA IN USCITA
*/


for(int i=0;i<4096;i++) v[i]=i; //un vettore con ogni elemento che aumenta sempre

//Configurazione TIMER2 
TIM2->ARR=N_ARR;

TIM2->CEN=1;

while(1){
  
  
punt=DAC_DHR12R1;
*punt=v[cont]; //assegno un codice che aumenta sempre 
cont++;
if(cont==4096)cont=0;

punt=DAC_SWTRIGR;
*punt|=SWTRIG1; //do il software trigger
while(((*punt)&SWTRIG1)==SWTRIG1);
 
punt=ADC1_CR;
*punt|=ADSTART;

punt=ADC1_ISR;
while(((*punt)&EOC)!=EOC);

punt=ADC1_DR;

Volt=(float) (((*punt)*3.0)/4095.0);

printf("%.4f V\n",Volt);

while((TIM2->UIF)!=1);
TIM2->UIF=0;
}//end while

}//end main 
