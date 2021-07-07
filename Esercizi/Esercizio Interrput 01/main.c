#include <stm32f30x.h>
//includo libreria
#include <math.h>
#include <stdio.h>
//premo tasto-->accendo luce

unsigned int* punt;
void main(){
//configurazione RCC
RCC->AHBENR|=(1<<17)|(1<<21)|(1<<22);
RCC->APB2ENR=(1<<0);
//configurazione I/O
GPIOE->MODER|=(1<<18); //moder9 output
SYSCFG->EXTICR|=(1<<10)|(1<<8); //SYSCFG PF2
NVIC->ISER[0]|=(1<<8);
//devo andare a smascherare all'NVIC
EXTI->IMR|=(1<<2);
EXTI->RTSR|=(1<<2);

while(1);

}//end main

void EXTI2_TS_IRQHandler(){
EXTI->PR|=(1<<2);

  //voglio accendere e spegnere il LEAD
  GPIOE->ODR^=(1<<9); //fa una NOT
  
}//fine mia ISR, a cui salto quando ho la mia causa di interrupt
