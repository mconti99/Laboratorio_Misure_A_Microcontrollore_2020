#include <stm32f30x.h>
//misura del tempo
#include <stdio.h>
#include <math.h>
#define Tclk 0.0000000139 //Tclk=1/Fclk(72MHz)
#define Nmax = 65535
unsigned int T_max=10;
float tempo;
float Tclk1;
int flag;
void main(){

RCC->AHBENR|=(1<<17)|(1<<21);
RCC->APB1ENR|=(1<<1);

GPIOE->MODER|=(1<<18);
GPIOE->MODER|=(1<<16);
GPIOE->MODER|=(1<<22);
GPIOA->MODER&=~(1<<0);
GPIOA->MODER&=~(1<<1);


//necessario generare un update event per aggiornare il prescaler
TIM3->CR1|=(1<<0); //accendo il timer
/* NOTA: Il calcolo del PSC è "PSC=(Tmax/(Nmax*tclk))-1", tuttavia è stato aggiunto un +1
per garantire una approssimazione per eccesso, utile per non sottostimare il valore
del PSC.*/
TIM3->PSC=(unsigned int) (T_max/(65535*Tclk));
Tclk1=((TIM3->PSC)+1)*Tclk;
TIM3->ARR=(unsigned int) (T_max/Tclk1);


while(((TIM3->SR)&(1<<0))!=(1<<0)); //aspetto che il timer arriva fino ad ARR
TIM3->SR&=~(1<<0);
TIM3->CR1&=~(1<<0);

TIM3->DIER|=(1<<0);

NVIC->ISER[0]|=(1<<6) |(1<<29);

EXTI->IMR|=(1<<0);
EXTI->RTSR|=(1<<0);
EXTI->FTSR|=(1<<0);
flag=1;


while(1){};
//start misura

}//end main

void EXTI0_IRQHandler(){
EXTI->PR|=(1<<0);
if(flag){
  if(((GPIOE->ODR) &(1<<9))!=(1<<9)){
  printf("tasto premuto\n");
  TIM3->CR1|=(1<<0);
if(((GPIOE->ODR)&(1<<8))==(1<<8)) GPIOE->ODR&=~(1<<8);
if(((GPIOE->ODR)&(1<<9))==(1<<9)) GPIOE->ODR&=~(1<<9);

GPIOE->ODR|=(1<<11); //aggiungo luce verde da accendere
flag=0;
//interrupt timer 3
  }
  else{
    GPIOE->ODR &= ~(1<<9);
  }
}else{
  
  printf("ho rilasciato il tasto in tempo desiderato\n");
  TIM3->CR1 &=~(1<<0);
	GPIOE->ODR&=~(1<<11); //spengo luce verde
        GPIOE->ODR|=(1<<8); //attivo led blu
	tempo=(float) ((TIM3->CNT)*Tclk1);
	printf("delta_T= %.2f sec\n", tempo);

TIM3->CNT=0;
flag=1;
}
}//fine mia ISR, a cui salto quando ho la mia causa di interrupt

void TIM3_IRQHandler(){
  	TIM3->SR&=~(1<<0);
        TIM3->CR1 &=~(1<<0);
        GPIOE->ODR&=~(1<<11); //spengo luce verde
	GPIOE->ODR|=(1<<9); //attivo led rosso
	
	printf("tempo max superato\n");
        flag=1;
}
