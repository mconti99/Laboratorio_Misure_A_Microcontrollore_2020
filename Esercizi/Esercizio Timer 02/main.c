#include "MySTM32F303VC.h"
//misura del tempo
#include <stdio.h>
#include <math.h>
#define Tclk 0.000000125 //Tclk=1/Fclk
#define Nmax = 65535
unsigned int T_max=10;
float tempo;
float Tclk1;
void main(){

RCC->IOPAEN=1;
RCC->IOPEEN=1;
RCC->TIM3EN=1;


GPIOE->MODER9=Output;
GPIOE->MODER8=Output;
GPIOE->MODER11=Output;
GPIOA->MODER0=Input;


//necessario generare un update event per aggiornare il prescaler
TIM3->CEN=1; //accendo il timer
/* NOTA: Il calcolo del PSC è "PSC=(Tmax/(Nmax*tclk))-1", tuttavia è stato aggiunto un +1
per garantire una approssimazione per eccesso, utile per non sottostimare il valore
del PSC.*/
TIM3->PSC=(unsigned int) (T_max/(65535*Tclk));
Tclk1=((TIM3->PSC)+1)*Tclk;
TIM3->ARR=(unsigned int) (T_max/(((TIM3->PSC)+1)*Tclk));


while(TIM3->UIF==0); //aspetto che il timer arriva fino ad ARR
TIM3->UIF=0;
TIM3->CEN=0;

while(1){
//start misura
while(GPIOA->IDR0==1); //serve per bloccare il ciclo precedente (se continuo a tenere premuto il tasto)
while(GPIOA->IDR0==0); //una volta che premo, esco fuori da questo while
TIM3->CEN=1;
if(GPIOE->ODR8==1) GPIOE->ODR8=0;
if(GPIOE->ODR9==1) GPIOE->ODR9=0;
GPIOE->ODR11=1; //aggiungo luce verde da accendere
while((GPIOA->IDR0==1) && (TIM3->UIF==0));
//se rilascio il tasto o raggiungo il T_max
//dopo 25 sec UIF diventa alto
TIM3->CEN=0;

if(TIM3->UIF==1){ 
	TIM3->UIF=0;
        GPIOE->ODR11=0;
	GPIOE->ODR9=1; //attivo led rosso
	
	printf("tempo max superato\n");
}else{ //ho rilasciato il tasto in tempo desiderato
	GPIOE->ODR11=0;
        GPIOE->ODR8=1; //attivo led blu
	tempo=(float) ((TIM3->CNT)*Tclk1);
	printf("delta_T= %.2f sec\n", tempo);
}

TIM3->CNT=0;
}
}//end main