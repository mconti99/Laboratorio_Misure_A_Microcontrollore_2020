#include "MySTM32F303VC.h"
#include <math.h>

#define RCC_APB1ENR (unsigned int*) 0x4002101C
#define TIM2_CR (unsigned int*) 0x40000000
#define TM2_CNT (unsigned int*) 0x40000024
#define TM2_ARR (unsigned int*) 0x4000002C
#define TM2_SR (unsigned int*) 0x40000010

#define Tclk 0.000000125 //Tclk=1/Fclk (8MHz)
#define N_ARR 8000000//per contare 1s alla frequenza Fclk di 8MHz
//se voglio fare mezzo secondo, metto N a 4 milioni

unsigned int* punt;
void main(){
//configurazione RCC
RCC->IOPAEN=1;
RCC->IOPEEN=1;
//punt=RCC_APB1ENR;
//*punt|=1; // abilito timer 2
RCC->TIM2EN=1;

//configurazione I/O
GPIOE->MODER9=Output;
GPIOA->MODER0=Input;

//configurazione timer
//punt=TM2_ARR; //precarico l'ARR
//*punt=N_ARR;
TIM2->ARR=N_ARR;
//quando mi arriva il clock, il counter si precaricherà ad 8000000 
//(quando succede questo, vuol dire che è passato 1 secondo)

//punt=TIM2_CR;
//*punt|=1; //avvio il timer
TIM2->CEN=1;

//punt=TM2_SR;
while(1){

GPIOE->ODR9=1; //Accendo il led
//devo aspettare la fine del conteggio che ho avviato (l'update event)
//devo aspettare TM2_CR_UIF=1
//poi devo resettare TM2_CR_UIF=0
//while(((*punt)&1)!=1); //aspetto UIF=1
//*punt&=~1; //UIF=0;
while((TIM2->UIF)!=1);
TIM2->UIF=0;

GPIOE->ODR9=0; //Spengo il led
//devo aspettare la fine del conteggio che ho avviato (l'update event)
//devo aspettare TM2_CR_UIF=1
//poi devo resettare TM2_CR_UIF=0
//while(((*punt)&1)!=1); //aspetto UIF=1
//*punt&=~1; //UIF=0;
while((TIM2->UIF)!=1);
TIM2->UIF=0;
}//end while

}//end main