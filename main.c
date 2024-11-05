
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

//void printString(char * string);
char readChar(void);
void printChar(char);

//shifting from pa0 to pb0, pa1 to pb1
void GPIO_F_Interrupt(void);

char c = 'a' ;
char d;
int main(void)
{
    SYSCTL_RCGCUART_R |= ( 1 << 0);

    SYSCTL_RCGC2_R |= (1 << 0);
    GPIO_PORTA_LOCK_R = 0x4C4F434B;
    GPIO_PORTA_CR_R = (1 << 1) | ( 1 << 0);
    GPIO_PORTA_DEN_R = (1 << 1) | ( 1 << 0);
    GPIO_PORTA_AFSEL_R = (1 << 1) | ( 1 << 0);
    GPIO_PORTA_PCTL_R = (1 << 0) | ( 1 << 4);




    UART0_CTL_R &= ~(1 << 0);
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 0x11;
    UART0_LCRH_R = (0x3 << 5) ;//|(1 << 1)|(1 << 7);
    UART0_CC_R = 0x0;

    UART0_CTL_R = (1 << 0)|(1 << 8)|(1 << 9);

    SYSCTL_RCGC2_R |= (1 << 5);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = (1 << 1) | ( 1 << 2)|(1 << 3)|( 1 << 0)|(1 << 4);
    GPIO_PORTF_DEN_R = (1 << 1) | ( 1 << 2)|(1 << 3)|( 1 << 0)|(1 << 4);
    GPIO_PORTF_DIR_R = (1 << 1) | ( 1 << 2)|(1 << 3);
    GPIO_PORTF_DATA_R &= ~((1 << 1)|( 1 << 2)|(1 << 3));
    GPIO_PORTF_PUR_R = 0x11;

    GPIO_PORTF_IS_R &= ~0x11;     // Make PF0 edge-sensitive
             GPIO_PORTF_IBE_R &= ~0x11;    // Disable interrupt on both edges
             GPIO_PORTF_IEV_R &= ~0x11;    // Interrupt on falling edge (when button is pressed)
             GPIO_PORTF_IM_R |= 0x11;  // Unmask the interrupt for PF0

             NVIC_EN0_R |= (1 << 30);  // Enable interrupt for Port F (bit 30 for Port F)

    while(1){





       d = readChar();



        switch(d)
        {


       case 'R':
           GPIO_PORTF_DATA_R = (1 << 1);
        break;

        case 'B':
            GPIO_PORTF_DATA_R = (1 << 2);
         break;

       case 'G':
                GPIO_PORTF_DATA_R = (1 << 3);
         break;

        default:
           GPIO_PORTF_DATA_R &= ~((1 << 1)|( 1 << 2)|(1 << 3));
         break;
       }

    }


}

char readChar(void){

    char a ;
    while((UART0_FR_R & (1 << 4)) != 0);  // Fix parentheses

    a =  UART0_DR_R;
    return a;

}

void printChar(char c){

    while(( UART0_FR_R & (1 << 5) != 0));
    UART0_DR_R = c;

}



void GPIO_F_Interrupt(void){
    if (GPIO_PORTF_RIS_R & 0x10) {  // Interrupt for PF4 //sw1
        GPIO_PORTF_ICR_R |= 0x10;
        c = 'G';
        printChar(c);
    }

    if (GPIO_PORTF_RIS_R & 0x01) {  // Interrupt for PF0 //sw2
           GPIO_PORTF_ICR_R |= 0x01;
           c = 'R';
           printChar(c);
    }

}


