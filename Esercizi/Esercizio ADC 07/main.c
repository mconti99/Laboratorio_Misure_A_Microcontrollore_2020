#include "MySTM32F303VC.h"
#include <stdio.h>
#include <math.h>

#define RCC_APB1ENR (unsigned int*) 0x4002101C
#define TIM2_CR1 (unsigned int*) 0x40000000
#define TIM2_CR2 (unsigned int*) 0x40000004
#define TIM3_CR2 (unsigned int*) 0x40000404
#define TIM2_CNT (unsigned int*) 0x40000024
#define TIM2_ARR (unsigned int*) 0x4000002C
#define TIM2_SR (unsigned int*)  0x40000010

#define Tclk 0.000000125 //Tclk=1/Fclk

//Per generare segnale a 100 Hz
#define ARR2 800//per contare 100 us alla freq Fclk=8 MHz
//Registri ADC

#define ADC12_CCR (unsigned int*) 0x50000308
#define ADC1_CR (unsigned int*) 0x50000008
#define ADC1_DIFSEL (unsigned int*) 0x500000B0
#define ADC1_ISR (unsigned int*) 0x50000000
#define ADC1_SMPR1 (unsigned int*) 0x50000014
#define ADC1_SQR1 (unsigned int*) 0x50000030
#define ADC1_DR (unsigned int*) 0x50000040
#define ADC1_CFGR (unsigned int*) 0x5000000C

#define CKMODE_1 (unsigned int) 1<<17
#define CKMODE_0 (unsigned int) 1<<16

//Bit ADC1_CR

#define ADVREGEN_0 (unsigned int) 1<<28
#define ADVREGEN_1 (unsigned int) 1<<29

#define ADEN (unsigned int) 1<<0
#define DIFSEL_1 (unsigned int) 1<<1
#define ADCAL (unsigned int) 1<<31
#define ADSTART (unsigned int) 1<<2

//Bit ADC1_ISR
#define EOC (unsigned int) 1<<2
#define ADRDY (unsigned int) 1<<0

#define L_0 (unsigned int) 1<<0
#define L_1 (unsigned int) 1<<1
#define L_2 (unsigned int) 1<<2
#define L_3 (unsigned int) 1<<3

#define SQ1_0 (unsigned int) 1<<6
#define SQ1_1 (unsigned int) 1<<7
#define SQ1_2 (unsigned int) 1<<8
#define SQ1_3 (unsigned int) 1<<9
#define SQ1_4 (unsigned int) 1<<10

#define SMP1_0 (unsigned int) 1<<3
#define SMP1_1 (unsigned int) 1<<4
#define SMP1_2 (unsigned int) 1<<5

#define DMAEN (unsigned int) 1<<0
#define EXTEN (unsigned int) 1<<10
#define EXTSEL (unsigned int) 1<<8
#define DMACFG (unsigned int) 1<<1
//Registri DAC
#define DAC_CR (unsigned int*) 0x40007400
#define DAC_SWTRIGR (unsigned int*) 0x40007404
#define DAC_DHR12R1 (unsigned int*) 0x40007408

#define EN1 (unsigned int) 1<<0
#define BOFF1 (unsigned int) 1<<1
#define TEN1 (unsigned int) 1<<2
#define TSEL1_0 (unsigned int) 1<<3
#define TSEL1_1 (unsigned int) 1<<4
#define TSEL1_2 (unsigned int) 1<<5
#define DMAEN1 (unsigned int) 1<<12

#define SWTRIG1 (unsigned int) 1<<0

#define MMS_0 (unsigned int) 1<<4
#define MMS_1 (unsigned int) 1<<5
#define MMS_2 (unsigned int) 1<<6

//registri del DMA1_Channel1
#define DMA1_CPAR1 (unsigned int*) 0x40020010
#define DMA1_CMAR1 (unsigned int*) 0x40020014
#define DMA1_CCR1 (unsigned int*) 0x40020008
#define DMA1_CNDTR1 (unsigned int*) 0x4002000C
#define DMA1_ISR (unsigned int*) 0x40020000
#define DMA1_IFCR (unsigned int*) 0x40020004
//define del DMA
#define EN (unsigned int) 1<<0
#define DIR (unsigned int) 1<<4
#define CIRC (unsigned int) 1<<5
#define PINC (unsigned int) 1<<6
#define MINC (unsigned int) 1<<7
#define PSIZE_0 (unsigned int) 1<<8
#define PSIZE_1 (unsigned int) 1<<9
#define MSIZE_0 (unsigned int) 1<<10
#define MSIZE_1 (unsigned int) 1<<11
#define PL_0 (unsigned int) 1<<12
#define PL_1 (unsigned int) 1<<13
#define TCIF1 (unsigned int) 1<<1
#define CTCIF1 (unsigned int) 1<<1
//registri DMA2 channel 3
#define DMA2_CPAR3 (unsigned int*) 0x40020438
#define DMA2_CMAR3 (unsigned int*) 0x4002043C
#define DMA2_CCR3 (unsigned int*) 0x40020430
#define DMA2_CNDTR3 (unsigned int*) 0x40020434

unsigned int* punt;
float Volt;
float x1;
unsigned int y[100];
int i=0;

float x,dx;
#define pi 3.14159265358979

unsigned int v[100];

void main(){
//configurazione RCC
RCC->IOPAEN=1;
RCC->TIM2EN=1;
RCC->TIM3EN=1;
RCC->DACEN=1;
//abilito ADC12 da RCC
RCC->ADC12=1;
RCC->DMA2EN=1;
RCC->DMA1EN=1;


GPIOA->MODER4=Analog; //output DAC1 (canale 1)
GPIOA->MODER2=Analog;
//generazione della forma d'onda
x=0.0; //punto di partenza della sinusoide
dx=(2*pi)/100; //la sinusoide è fatta da 100 campioni
for( i=0;i<100;i++){ 
 y[i]=(unsigned int)(2047*sin(x)+2048);
 x=x+dx;
}
i=0;

//CANALE 1 DMA 1
punt=DMA1_CPAR1;
*punt=(unsigned int) ADC1_DR; //devo mettere l'indirizzo di DHR12R1
//devo mettere un valore, non un puntatore, quindi faccio un type-casting ad unsigned int
punt=DMA1_CMAR1;
*punt=(unsigned int) &v[0]; //lo posso verificare col live-watch
punt=DMA1_CNDTR1;
*punt=100; //il vettore è di 100 elementi
punt=DMA1_CCR1;
*punt|=PL_1|PL_0; //PRIORITA' MASSIMA
*punt&=~(MSIZE_0); //MSIZE[1:0]=10 32 bit
*punt|=MSIZE_1;
*punt&=~(PSIZE_0); //PSIZE[1:0]=10 32 bit
*punt|=PSIZE_1;
*punt|=MINC; //INCREMENTO LA MEMORIA
//punt|=CIRC; //BUFFER CIRCOLARE
*punt&=~(DIR); //DIREZIONE PERIFERICA-MEMORIA
*punt|=EN; //Abilito il DMA

//configurazione DMA2
punt=DMA2_CPAR3;
*punt=(unsigned int) DAC_DHR12R1;
punt=DMA2_CMAR3;
*punt=(unsigned int) &y[0]; //lo posso verificare col live-watch
punt=DMA2_CNDTR3;
*punt=100; //il vettore è di 100 elementi
punt=DMA2_CCR3;
*punt|=PL_1|PL_0; //PRIORITA' MASSIMA
*punt&=~(MSIZE_0); //MSIZE[1:0]=10 32 bit
*punt|=MSIZE_1;
*punt&=~(PSIZE_0); //PSIZE[1:0]=10 32 bit
*punt|=PSIZE_1;
*punt|=MINC; //INCREMENTO LA MEMORIA
*punt&=~(PINC); //non incremento nella periferica, posso anche non metterlo
*punt|=CIRC; //BUFFER CIRCOLARE
*punt|=DIR; //DIREZIONE 
*punt|=EN; //Abilito il DMA

//configurazione timer
TIM2->ARR=ARR2;
punt=TIM2_CR2;
*punt&=~(MMS_0); //generazione dell'evento di trigger all'update event MMS[2:0]=010
*punt|=MMS_1;
*punt&=~(MMS_2);

//configurazione del DAC
punt=DAC_CR;
*punt|=(BOFF1); //amplificatore di uscita disattivato
*punt|=TEN1;
*punt&=~(TSEL1_0);//TRIGGER DA TIMER2 100
*punt&=~(TSEL1_1);
*punt|=TSEL1_2;
*punt|=DMAEN1; //ABILITAZIONE DEL DMA
*punt|=EN1; //abilito il DAC

//PARTE IL TIMER2
TIM2->CEN=1;

//configurazione timer3
TIM3->ARR=ARR2; //mi serve un altro timer per ADC
punt=TIM3_CR2;
*punt&=~(MMS_0); //generazione dell'evento di trigger all'update event MMS[2:0]=010
*punt|=MMS_1;
*punt&=~(MMS_2);

//configurazione CKMODE
punt=ADC12_CCR;
*punt|=CKMODE_0;
*punt&=~(CKMODE_1); //devo mettere le parentesi davanti al negato

//abilito il regolatore di tensione
punt=ADC1_CR;
*punt&=~(ADVREGEN_1);
*punt|=ADVREGEN_0;

for(int i=0;i<800000;i++); //brutto

//selezionare acquisizione single-ended oppure differenziale
*punt&=~(ADEN);
punt=ADC1_DIFSEL;
*punt&=~(DIFSEL_1);

//eseguire la calibrazione
punt=ADC1_CR;
*punt|=ADCAL;
while(((*punt)&ADCAL)==ADCAL); //esco dal while quando la calibrazione è finita

//accendo l'ADC
*punt|=ADEN;
punt=ADC1_ISR;
while(((*punt)&ADRDY)!=ADRDY);//dobbiamo aspettare che si alzi

//imposto il time sampling
punt=ADC1_SMPR1;
*punt|=SMP1_0 | SMP1_1 | SMP1_2;

//seleziono la sequenza di conversione
punt=ADC1_SQR1;
*punt&=~(L_0);
*punt&=~(L_1);
*punt&=~(L_2);
*punt&=~(L_3);

//selezione del canale 3 per acquisire PA6 SQ1=00011
*punt|=SQ1_0;
*punt|=SQ1_1;
*punt&=~(SQ1_2);
*punt&=~(SQ1_3);
*punt&=~(SQ1_4);

punt=ADC1_CFGR;
*punt|=EXTEN |EXTSEL;
*punt|=DMAEN | DMACFG;

punt=ADC1_CR;
*punt|=ADSTART;

TIM3->CEN=1;

for(int i = 0;i <800000;i++); // aspetto adready

while(1){
         
           punt=DMA1_ISR;
           int t;
           do{
             t=(int)*punt;
           }while(t&(1<<1) != (1<<1));
           
           TIM3->CEN=0;
           punt=DMA1_IFCR;
           *punt|=(CTCIF1);
           *punt&=~(CTCIF1);
           x1=0.0;
           for(i=0;i<100;i++) {
             x1+=(pow(v[i]*(3.0/4095.0), 2));
           }
           x1=sqrt(x1/100.0);
           printf("veff: %.2f\n", x1);
           TIM3->CEN=1;
         }

}//end main 