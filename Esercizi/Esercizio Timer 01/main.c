#include "MySTM32F303VC.h"
//base dei timer col TIMER3
#include <stdio.h>
#define Tclk 0.000000125 //Tclk=1/Fclk
#define N_ARR 65535
float delta_T;
unsigned int* punt;
void main(){

RCC->IOPAEN=1;
RCC->IOPEEN=1;

RCC->TIM3EN=1;

GPIOE->MODER9=Output;
GPIOA->MODER0=Input;

//configurazione timer
//quando mi arriva il clock, il counter si precaricherà a 65040
//(quando succede questo, vuol dire che è passato 1 secondo)

TIM3->PSC=(unsigned int) 1.0/(65535*Tclk);
TIM3->ARR=(unsigned int)(1.0/(((TIM3->PSC)+1)*Tclk));
TIM3->CEN=1;
while(TIM3->UIF==0); //per caricare prescaler
TIM3->CEN=0;
TIM3->UIF=0;

while(1){
TIM3->CEN=1;
GPIOE->ODR9=1; //Accendo il led
//devo aspettare la fine del conteggio che ho avviato (l'update event)
//devo aspettare TM3_CR_UIF=1
//poi devo resettare TM3_CR_UIF=0

while((TIM3->UIF)!=1);
TIM3->UIF=0;

GPIOE->ODR9=0; //Spengo il led
//devo aspettare la fine del conteggio che ho avviato (l'update event)
//devo aspettare TM3_CR_UIF=1
//poi devo resettare TM3_CR_UIF=0

while((TIM3->UIF)!=1);
TIM3->UIF=0;
TIM3->CEN=0;
}//end while

}//end main

