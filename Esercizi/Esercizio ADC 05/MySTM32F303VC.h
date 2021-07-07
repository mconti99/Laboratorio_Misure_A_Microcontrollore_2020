typedef struct{

   unsigned int CR;
   unsigned int CFGR;
   unsigned int CIR;
   unsigned int APB2RSTR;
   unsigned int APB1RSTR;
   
   union{unsigned int AHBENR;
     struct{
       unsigned DMA1EN:1;   
       unsigned DMA2EN:1;  
       unsigned SRAMEN:1;
       unsigned Res:1; //1 bit riservato 
       unsigned FLITFEN:1;   
       unsigned Res1:1;
       unsigned CRCEN:1;
       unsigned Res2:10; //10 bit consecutivi riservati
       unsigned IOPAEN:1;
       unsigned IOPBEN:1;
       unsigned IOPCEN:1;
       unsigned IOPDEN:1;
       unsigned IOPEEN:1;
       unsigned IOPFEN:1;
       unsigned Res3:1;
       unsigned TSCEN:1;
       unsigned Res4:3;
       unsigned ADC122:1;
       unsigned ADC34:1;
       unsigned Res5:2;
                     };//END STRUCT ahbenr
            };//end union AHBENR
   
   
   unsigned int APB2ENR;
   union{unsigned int APB1ENR;
    struct{
      unsigned TIM2EN:1;
      unsigned TIM3EN:1;
      unsigned TIM4EN:1;
      unsigned Res6:1;
      unsigned TIM6EN:1;
      unsigned TIM7EN:1;
      unsigned Res7:5;
      unsigned WWDGEN:1;
      unsigned Res8:2;
      unsigned SPI2EN:1;
      unsigned SPI3EN:1;
      unsigned Res9:1;
      unsigned USART2EN:1;
      unsigned USART3EN:1;
      unsigned UART4EN:1;
      unsigned UART5EN:1;
      unsigned I2C1EN:1;
      unsigned I2C2EN:1;
      unsigned USBEN:1;
      unsigned Res10:1;
      unsigned CANEN:1;
      unsigned Res11:2;
      unsigned PWREN:1;
      unsigned DACEN:1;
      unsigned Res12:2;
      };
   };
   unsigned int BDCR;
   unsigned int CSR;
   unsigned int AHBRSTR;
   unsigned int CFGR2;
   unsigned int CFGR3;

}RCC_Type;



/*---------------------------------------*/
/***********STRUTTURA DATI GPIO**********/

typedef struct{
  
  union{unsigned int MODER;
        struct{
          unsigned MODER0:2;
          unsigned MODER1:2;
          unsigned MODER2:2;
          unsigned MODER3:2;
          unsigned MODER4:2;
          unsigned MODER5:2;
          unsigned MODER6:2;
          unsigned MODER7:2;
          unsigned MODER8:2;
          unsigned MODER9:2;
          unsigned MODER10:2;
          unsigned MODER11:2;
          unsigned MODER12:2;
          unsigned MODER13:2;
          unsigned MODER14:2;
          unsigned MODER15:2;
                };
        };
  
       
       unsigned int OTYPER;
       unsigned int OSPEEDR;
       unsigned int PUPDR;
       
       
       union{   unsigned int IDR;
                struct{
                  unsigned IDR0:1;
                  unsigned IDR1:1;
                  unsigned IDR2:1;
                  unsigned IDR3:1;
                  unsigned IDR4:1;
                  unsigned IDR5:1;
                  unsigned IDR6:1;
                  unsigned IDR7:1;
                  unsigned IDR8:1;
                  unsigned IDR9:1;
                  unsigned IDR10:1;
                  unsigned IDR11:1;
                  unsigned IDR12:1;
                  unsigned IDR13:1;
                  unsigned IDR14:1;
                  unsigned IDR15:1;
                  };
             };
       
       
       
       union{     unsigned int ODR;
                  struct{
                  unsigned OR0:1;
                  unsigned ODR1:1;
                  unsigned ODR2:1;
                  unsigned ODR3:1;
                  unsigned ODR4:1;
                  unsigned ODR5:1;
                  unsigned ODR6:1;
                  unsigned ODR7:1;
                  unsigned ODR8:1;
                  unsigned ODR9:1;
                  unsigned ODR10:1;
                  unsigned ODR11:1;
                  unsigned ODR12:1;
                  unsigned ODR13:1;
                  unsigned ODR14:1;
                  unsigned ODR15:1;
                  };
                  
       };
       /*
       unsigned int BSRR;
       unsigned int LCKR;
       unsigned int AFRL;
       unsigned int AFRH;
       unsigned int BRR;
       */

}GPIO_Type;

// STRUTTURA DATI TIMER 1
typedef struct{
  union{unsigned int CR1;
    struct{
      unsigned CEN:1;
      unsigned UDIS:1;
      unsigned URS:1;
      unsigned OPM:1;
      unsigned DIR:1;
      unsigned CMS:2;
      unsigned ARPE:1;
      unsigned CKD:2;
      unsigned Res13:1;
      unsigned UIREMAP:1;
      unsigned Res14:4;

    };
  };
  unsigned int CR2;
  unsigned int SMCR;
  unsigned int DIER;
  union{unsigned int SR;
    struct{
      unsigned UIF:1;
      unsigned CC1IF:1;
      unsigned CC2IF:1;
      unsigned CC3IF:1;
      unsigned CC4IF:1;
      unsigned Res15:1;
      unsigned TIF:1;
      unsigned Res16:2;
      unsigned CC1OF:1;
      unsigned CC2OF:1;
      unsigned CC3OF:1;
      unsigned CC4OF:1;
      unsigned Res17:3;

    };
  };
  unsigned int EGR;
  unsigned int CCMR1;
  unsigned int CCMR2;
  unsigned int CCER;
  unsigned int CNT;
  unsigned int PSC;
  unsigned int ARR;
  unsigned int CCR1;
  unsigned int CCR2;
  unsigned int CCR3;
  unsigned int CCR4;
  unsigned int DCR;
  unsigned int DMAR;
}TIM_Type;

typedef struct{
  unsigned int CSR;
  unsigned int Res;
  union{unsigned int CCR;
  struct{
    unsigned DUAL:5;
    unsigned Res18:3;
    unsigned DELAY: 4;
    unsigned Res19:1;
    unsigned DMACFG:1;
    unsigned MDMA:2;
    unsigned CKMODE:2;
    unsigned Res20:4;
    unsigned VREFEN:1;
    unsigned TSEN:1;
    unsigned VBATEN:1;
    unsigned Res21:7;
   };
  };
  unsigned int CDR;
}ADC_Common_Type;

typedef struct{
  union{unsigned int ISR;
   struct{
     unsigned ADRDY:1;
     unsigned EOSMP:1;
     unsigned EOC:1;
     unsigned EOS:1;
     unsigned OVR:1;
     unsigned JEOC:1;
     unsigned JEOS:1;
     unsigned AWD1:1;
     unsigned AWD2:1;
     unsigned AWD3:1;
     unsigned JQOVF:1;
     unsigned Res25:21;
   };
  };
    
  unsigned int IER;
  union{unsigned int CR;
    struct{
      unsigned ADEN:1;
      unsigned ADDIS:1;
      unsigned ADSTART:1;
      unsigned JADSTART:1;
      unsigned ADSTP:1;
      unsigned JADSTP:1;
      unsigned Res22:22;
      unsigned ADVREGEN:2;
      unsigned ADCALDIF:1;
      unsigned ADCAL:1;
    };
  };
  
  unsigned int CFGR;
  unsigned int Pippo;
  union{unsigned int SMPR1;
  struct{
    unsigned Res26:3;
    unsigned SMP1:3;
    unsigned SMP2:3;
    unsigned SMP3:3;
    unsigned SMP4:3;
    unsigned SMP50:1;
    unsigned SMP5:2;
    unsigned SMP6:3;
    unsigned SMP7:3;
    unsigned SMP8:3;
    unsigned SMP9:3;
    unsigned Res27:2;
  };
  };
  unsigned int SMPR2;
  unsigned int Pippo2;
  unsigned int TR1;
  unsigned int TR2;
  unsigned int TR3;
  unsigned int PippoCoca;
  union{unsigned int SQR1;
  struct{
    unsigned L:4;
    unsigned Res28:2;
    unsigned SQ1:5;
    unsigned Res29:1;
    unsigned SQ2:5;
    unsigned Res30:1;
    unsigned SQ3:5;
    unsigned Res31:1;
    unsigned SQ4:5;
    unsigned Res32:3;
  };
  };
  unsigned int SQR2;
  unsigned int SQR3;
  unsigned int SQR4;
  unsigned int DR;
  unsigned int vett[2];
  unsigned int JSQR;
  unsigned int PIPPOZZOZZONI[24];
  union{unsigned int DIFSEL;
  struct{
    unsigned Res23:1;
    unsigned DIFSEL_1:1;
    unsigned DIFSEL_2:1;
    unsigned DIFSEL_3:1;
    unsigned DIFSEL_4:1;
    unsigned DIFSEL_5:1;
    unsigned DIFSEL_6:1;
    unsigned DIFSEL_7:1;
    unsigned DIFSEL_8:1;
    unsigned DIFSEL_9:1;
    unsigned DIFSEL_10:1;
    unsigned DIFSEL_11:1;
    unsigned DIFSEL_12:1;
    unsigned DIFSEL_13:1;
    unsigned DIFSEL_14:1;
    unsigned DIFSEL_15:1;
    unsigned DIFSEL1:3; //strano
    unsigned Res24:13;
  };
  };
 // unsigned int CALFACT;
}ADC_Type;

typedef struct{
  union{unsigned int CR;
  struct{
    unsigned EN1:1;
    unsigned BOFF1:1;
    unsigned TEN1:1;
    unsigned TSEL1:3;
    unsigned WAVE1:2;
    unsigned MAMP1:4;
    unsigned DMAEN1:1;
    unsigned DMAUDRIE1:1;
    unsigned Res25:2;
    unsigned EN2:1;
    unsigned BOFF2:1;
    unsigned TEN2:1;
    unsigned TSEL2:3;
    unsigned WAVE2:2;
    unsigned MAMP2:4;
    unsigned DMAEN2:1;
    unsigned DMAUDRIE2:1;
    unsigned Res26:2;
  };
  };
  union{unsigned int SWTRIGR;
  struct{
    unsigned SWTRIG1:1;
    unsigned SWTRIG2:1;
    unsigned Res27:30;
  };
  };
  unsigned int DHR12R1;
  unsigned int DHR12L1;
  unsigned int DHR8R1;
  unsigned int DHR12R2;
  unsigned int DHR12L2;
  unsigned int DHR8R2;
  unsigned int DHR12RD;
  unsigned int DHR12LD;
  unsigned int DHR8RD;
  unsigned int DOR1;
  unsigned int DOR2;
  unsigned int SR;
}DAC_Type;
#define Input           0
#define Output          1
#define Alternate       2
#define Analog          3

#define DAC1 ((DAC_Type*) 0x40007400)
#define ADC1 ((ADC_Type*) 0x50000000)
#define ADC12 ((ADC_Common_Type*) 0x50000300)
#define TIM3 ((TIM_Type*) 0x40000400)
#define TIM2 ((TIM_Type*) 0x40000000)
#define RCC ((RCC_Type*) 0x40021000)
#define GPIOA ((GPIO_Type*) 0x48000000)
#define GPIOE ((GPIO_Type*) 0x48001000)
