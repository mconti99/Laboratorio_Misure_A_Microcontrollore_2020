#include "MySTM32F303VC.h"

//alzare PE9

void main(){
  
  //punt=(unsigned int*) RCC_AHBENR;//devo puntare ad "RCC_AHBENR"
  //x signfica esadecimale
  //*punt|= IOPEN | (1<<17);// abilito PORTA A //(0x00200000); 
  	//RCC->AHBENR|=IOPEN | (1<<17);
  RCC->IOPAEN=1;
  RCC->IOPEEN=1;
  
  //punt =(unsigned int*) GPIOE_MODER; //devo puntare a GPIOE MODER(il primo che incotriamo senza offset)
  //*punt|= OUTPUT9; //0x00040000; //PE9 come uscita
  
  GPIOE->MODER|=OUTPUT9;
  GPIOE->MODER9=1; //deve essere di uscita
  
  //punt= (unsigned int *) GPIOA_MODER; //DEVO PUNTARE A GPIOA_MODER
  //*punt|= 3;
  //*punt&= ~(3<<0);//ho resettato GPIOA_MODER, per sicurezza
  
 // GPIOA->MODER&=~(3<<0);
  GPIOA->MODER0=0;
  while(1){
  //punt= (unsigned int *) GPIOA_IDR;
  //attesa che PA0 passi da 0->1
  //while((*punt&(1<<0))!=(1<<0)); //aspetto fino a che PA0=0
  //while((*punt&(1<<0))==(1<<0));//aspetto fino a che PA0=1
      //while((GPIOA->IDR&(1<<0)!=(1<<0));
      //while((GPIOA->IDR&(1<<0)==(1<<=));
  while(GPIOA->IDR0==0);
  while(GPIOA->IDR0==1);
   // punt =(unsigned int*) GPIOE_ODR; //devo puntare a GPIOE_ODR
   // if((*punt&(1<<9))!=(1<<9)) //vuol dire che prima era spento, quindi ora lo devo accendere
      //if((GPIOE->ODR&(1<<9)) != (1<<9))
  if(GPIOE->ODR9==1)
  //*punt|=P9; //0x00000200; //ALTO PE9
  //GPIOE->ODR|=P9;
  GPIOE->ODR9=0;
  else
  //*punt&=~P9; //0x00000200; //BASSO PE9
  //GPIOE->ODR&=~P9;
  GPIOE->ODR9=1;
  }//end while
} //end main
