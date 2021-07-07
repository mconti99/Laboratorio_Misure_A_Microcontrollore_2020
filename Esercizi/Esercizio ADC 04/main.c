#include "MySTM32F303VC.h"
#include <stdio.h>
#include <math.h>

#define Tclk 0.000000125 //Tclk=1/Fclk

#define N_ARR 267

unsigned int* punt;
float Volt;

short int y[200];
int i=0;

float x,dx;
#define pi 3.14159265358979



void main(){

//Configurazione RCC
RCC->IOPAEN=1;
RCC->IOPEEN=1;
RCC->APB2ENR|=(1<<11);
RCC->DACEN=1;
RCC->TIM2EN=1;
RCC->ADC122=1; //Abilito ADC12

GPIOA->MODER2=Analog;//Input; //CH3_ADC
GPIOA->MODER4=Analog;//Onput;// DAC1

//Configurazione CLK ADC12
ADC12->CKMODE=1;

//Voltage Regulator
ADC1->ADVREGEN=0;
ADC1->ADVREGEN=1;

/*
  per attendere 10microsecondi
  delta_T = 10*10^-6 secondi = ARR * Tclk
  ARR = delta_T/Tclk = 80
  NOTA:non devo attendere un update event poiche' il registro ARR
  non e' bufferizzato
  */
  
  TIM1->ARR=80; 
  TIM1->CEN=1;
  while((TIM1->UIF)==0);
  TIM1->CEN=0;
  TIM1->UIF=0;

//Single-Ended
  ADC1->ADEN=0;

ADC1->DIFSEL_2=0;


//Autocalibrazione
ADC1->ADCAL=1;
while(ADC1->ADCAL==1);

//Accensione ADC
ADC1->ADEN=1;

while(ADC1->ADRDY==0);

//Tempo di Sample
ADC1->SMP1=7;

//Sequenza regolare
ADC1->L=0;

//Selezione del Canale 3 per PA2
ADC1->SQ1=3;

//CONFIGURAZIONE DAC

DAC1->BOFF1=1;
DAC1->TEN1=1;
DAC1->TSEL1=4;
DAC1->EN1=1;



x=0.0;
dx=(2*pi)/200;

for(i=0;i<200;i++){
  
  y[i]=(short int)(2047*sin(x)+2048+ (2047/3)*sin(3*x));
   //Vmax=1.5V --> 2047
 //Voffset=1.5V --> 2048
  x=x+dx;
}

i=0;


//configurazione timer
TIM2->ARR=N_ARR;
TIM2->CR2|=(1<<5);
//quando mi arriva il clock, il counter si precaricherà ad 8000000 
//(quando succede questo, vuol dire che è passato 1 secondo)
TIM2->CEN=1;
while(1){
  DAC1->DHR12R1=y[i];
  i++;  
  if(i==100) i=0; //per renderlo periodico
}
}//end main 
