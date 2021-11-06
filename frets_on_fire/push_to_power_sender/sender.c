////sender
////msp430fr5994
//#include <msp430.h>
//#include "sender.h"
//#include "spi.h"
//
//#define CSN_EN nrfCSNportout &= ~nrfCSNpin
//#define CSN_DIS nrfCSNportout |= nrfCSNpin
//#define CE_EN nrfCEportout |= nrfCEpin
//#define CE_DIS nrfCEportout &= ~nrfCEpin
//
//void red(void)
//{
//    P1OUT |= BIT0;
//    __delay_cycles(200000);
//    P1OUT &= ~BIT0;
//}
//
//void green(void)
//{
//    P1OUT |= BIT1;
//    __delay_cycles(200000);
//    P1OUT &= ~BIT1;
//}
//
//void both(void)
//{
//    P1OUT |= BIT0 | BIT1;
//    __delay_cycles(900000);
//    P1OUT &= ~(BIT0 | BIT1);
//    __delay_cycles(100000);
//}
//
//int main(void)
//{
//    WDTCTL = WDTHOLD | WDTPW;
//    PM5CTL0 &= ~LOCKLPM5;
//
//    __bis_SR_register(GIE);
//    _EINT(); //??
//
//    CSCTL0_H = CSKEY_H; //unlock
//    CSCTL1 = DCOFSEL_0; //1MHz
//    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
//    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1; //keep everything at 1MHz
//    CSCTL0_H = 0; //lock
//
//    P1REN &= ~(BIT1 | BIT0); //leds for debugging
//    P1DIR |= BIT1 | BIT0;
//    P1OUT &= ~(BIT1 | BIT0);
//
//    uint8_t payload[] = {0x55};
//    uint8_t addr[3] = {0x11, 0x22, 0x33};
//
//    spi_init();
//
//    P4DIR &= ~nrfIRQpin;
//    P4OUT |= nrfIRQpin;
//    P4REN |= nrfIRQpin;
//    P4IES |= nrfIRQpin;
//    P4IFG &= ~nrfIRQpin;
//    P4IE |= nrfIRQpin;
//
//    P1DIR |= nrf_csn_pin;
//    CSN_DIS;
//
//    P1DIR |= nrf_ce_pin;
//    CE_DIS;
//
//    __delay_cycles(100000);
//
//
//
//
//}
//
//#pragma vector = PORT4_VECTOR
//__interrupt void P5_IRQ (void) {
//    if (P5IFG & nrfIRQpin) {
////        __bic_SR_register_on_exit(LPM4_bits);
////        rf_irq |= RF24_IRQ_FLAGGED;
//        P5IFG &= ~nrfIRQpin;
//        green();
//    }
//}
//
//
