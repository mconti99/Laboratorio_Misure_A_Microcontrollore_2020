#include "MySTM32F303VC.h"
//misura del tempo
#define RCC_APB1ENR (unsigned int*) 0x4002101C
#define TIM2_CR (unsigned int*) 0x40000000
#define TM2_CNT (unsigned int*) 0x40000024

#define Tclk 0.000000125 //Tclk=1/Fclk

float delta_T;
unsigned int* punt;
void main(){

RCC->IOPAEN=1;
RCC->IOPEEN=1;
punt=RCC_APB1ENR;
*punt|=1; // abilito timer 2
// abilito il clock del timer2


GPIOE->MODER9=Output;
GPIOA->MODER0=Input;

while(1){
//start misura
while(GPIOA->IDR0==0); 
while(GPIOA->IDR0==1); 

punt=TIM2_CR;
*punt|=1; //avvio il timer

GPIOE->ODR9=1; //Accendo il led
//stop misura del tempo
while(GPIOA->IDR0==0); 
while(GPIOA->IDR0==1); 

   GPIOE->ODR9=0; //Spengo il led
*punt&=~1; //spengo il timer
punt=TM2_CNT;

delta_T= (float) (*punt)*Tclk; //N è il numero di conteggi
*punt=0; //serve per far azzerare il timer

printf("Tempo: %f\n", delta_T);
}//end while

}//end main