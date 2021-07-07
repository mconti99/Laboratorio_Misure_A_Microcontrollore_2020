#include <stm32f30x.h>
#include <stdio.h>
#include <math.h>
int i=0;
void main(){
  RCC->AHBENR|=(1<<21)|(1<<17);
  GPIOE->MODER|=(1<<16)|(1<<18)|(1<<20)|(1<<22)|(1<<24)|(1<<26)|(1<<28)|(1<<30);
  NVIC->ISER[0]|=(1<<6);
  EXTI->IMR|=(1<<0);
  EXTI->RTSR|=(1<<0);
  while(1);
  
  
  
}

void EXTI0_IRQHandler(){
   EXTI->PR|=(1<<0);
   i++;
   if(i==255) i=0;
   GPIOE->ODR=(i<<8);
}