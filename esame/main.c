#include <stm32f30x.h>
#include <stdio.h>
#include <math.h>

#define Fclk 72000000 
#define Tclk 0.0000000139 // 1/72MHz
#define T_max 15

float Tclk1;
float tempo;
int flag;
unsigned int* punt;
float v[100];
int contatore=0;
float media, somma, SD1, SD2;
void main(){
  RCC->AHBENR|=(1<<17)|(1<<21); //abilito PORT A e PORT E
  RCC->APB1ENR|=(1<<1); //abilito periferica TIMER3
  RCC->APB2ENR|=(1<<0);
  
  GPIOE->MODER|=(1<<16)|(1<<18);
  TIM3->PSC=(unsigned int) (T_max*Fclk)/65535;
  Tclk1=(TIM3->PSC +1)*Tclk;
  TIM3->ARR=(unsigned int) T_max/Tclk1;
  
  //ciclo di boot del timer3
  TIM3->CR1|=(1<<0);
  while((TIM3->SR & (1<<0))!=(1<<0));
  TIM3->SR &= ~(1<<0);
  TIM3->CR1 &= ~(1<<0);
  
  NVIC->ISER[0]|=(1<<29) |(1<<8);
  TIM3->DIER|=(1<<0);
  punt=(unsigned int*) 0x40010008; //accedo a SYSCFG_EXTICR1
  *punt|=(1<<10)|(1<<8);
  EXTI->IMR|=(1<<2);
  EXTI->RTSR|=(1<<2);
  EXTI->FTSR|=(1<<2);
  flag=1;
  while(1){
    /*
    while((GPIOA->IDR & (1<<0))!=(1<<0)); //aspetto fin tanto che il tasto non è premuto, la prima volta
    GPIOE->ODR &= ~(1<<8);
    GPIOE->ODR &= ~(1<<9);
    //premo il tasto, inizio a contare
    TIM3->CR1|=(1<<0);
    //aspetto che rilascio il tasto la prima volta
    while((GPIOA->IDR & (1<<0))==(1<<0));
    //aspetto che premo e rilascio il tasto, per fermare il conteggio
    while((GPIOA->IDR & (1<<0))!=(1<<0));
    while((GPIOA->IDR & (1<<0))==(1<<0));
    //posso elaboare
    TIM3->CR1 &= ~(1<<0); //spengo CEN
    GPIOE->ODR|=(1<<8); //accendo LED8
    tempo= (float) ((TIM3->CNT)*Tclk1);
    printf("il tempo misurato correttamente è %.2f\n", tempo);
    //annullo CNT, per resettare il conteggio
    TIM3->CNT=0;

 */
}
}

void TIM3_IRQHandler(){ //quando entro qui, vuol dire che si è alzato UIF
  TIM3->SR &= ~(1<<0); //abbasso UIF (causa dell'interrupt)
  TIM3->CR1 &= ~(1<<0); //spengo CEN
  if((GPIOA->IDR & (1<<0))==(1<<0)){
    GPIOE->ODR|=(1<<9); //accendo LED9
    printf("ERRORE: tempo massimo superato\n");
  }
  flag=1;
}

void EXTI2_TS_IRQHandler(){
  EXTI->PR|=(1<<2);
  if(flag==1){
    if((GPIOE->ODR & (1<<9))!=(1<<9)){
    GPIOE->ODR &= ~(1<<8);
    GPIOE->ODR &= ~(1<<9);
    //premo il tasto, inizio a contare
    TIM3->CR1|=(1<<0); //setto CEN
    printf("tasto premuto prima volta\n");
    for(int i=0;i<7200000;i++);
    flag=2;
    }else{
      GPIOE->ODR &= ~(1<<9);
    }
    }
  else if(flag==2){
    printf("tasto rilasciato prima volta\n");
    flag=3;
  }
  else if(flag==3){
    printf("tasto premuto seconda volta\n");
    for(int i=0;i<7200000;i++);
    flag=4;
  }
  else{
    printf("tasto rilasciato seconda volta\n");
    TIM3->CR1 &= ~(1<<0); //spengo CEN
    GPIOE->ODR|=(1<<8); //accendo LED8
    tempo= (float) ((TIM3->CNT)*Tclk1);
    printf("il tempo misurato correttamente è %.2f\n", tempo);
    
    v[contatore]=tempo;
    contatore++;
    somma=0.0;
    media=0.0;
    SD1=0.0;
    SD2=0.0;
    for(int i=0;i<contatore;i++) somma+=v[i];
    media=somma/contatore;
    for(int i=0;i<contatore;i++) SD1 += pow(v[i]-media,2);
    SD2=sqrt(SD1/contatore);
    printf("MISURA EFFETTUATA: media = %.2f | standard deviation = %.2f\n", media, SD2);
    
    //annullo CNT, per resettare il conteggio
    TIM3->CNT=0;
    flag=1;
  }
}