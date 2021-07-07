#include <stm32f30x.h>
//includo libreria
#include <math.h>
#include <stdio.h>
//premo tasto-->accendo luce

unsigned int* punt;
void main(){
//configurazione RCC
RCC->AHBENR|=(1<<17)|(1<<21);

//configurazione I/O
GPIOE->MODER|=(1<<18); //moder9 output


NVIC->ISER[0]|=(1<<6);
//devo andare a smascherare all'NVIC
EXTI->IMR|=(1<<0);
EXTI->RTSR|=(1<<0);

while(1);

}//end main

void EXTI0_IRQHandler(void){
EXTI->PR|=(1<<0);

  //voglio accendere e spegnere il LEAD
  GPIOE->ODR^=(1<<9); //fa una NOT
  
}//fine mia ISR, a cui salto quando ho la mia causa di interrupt