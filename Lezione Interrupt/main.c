#include <math.h>
#include <stdio.h>
//voglio generare una base dei tempi senza guardare UIF
#include <stm32f30x.h>
//se voglio fare mezzo secondo, metto N a 4 milioni
//includo la libreria
unsigned int* punt;
void main(){
//configurazione RCC
RCC->AHBENR|=(1<<17)|(1<<21); //abiliito PORTA A E PORTA E
RCC->APB1ENR|=(1<<0);

//configurazione I/O
GPIOE->MODER|=(1<<18);

//configurazione timer
TIM2->ARR=72000000;
//ABILITO INTERRUPT DEL TIMER
TIM2->DIER|=(1<<0);
NVIC->ISER[0]=(1<<28);
//devo andare a smascherare all'NVIC

TIM2->CR1|=(1<<0); //accendo il timer

while(1);

}//end main

void TIM2_IRQHandler(){
  //eliminare la causa che ha generato l'interrput
  TIM2->SR&=~(1<<0);
  //voglio accendere e spegnere il LEAD
  GPIOE->ODR^=(1<<9); //fa una XOR 
  
}//fine mia ISR, a cui salto quando ho la mia causa di interrupt