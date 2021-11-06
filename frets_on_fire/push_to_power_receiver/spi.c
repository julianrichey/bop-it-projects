
#include <msp430.h>
#include "spi.h"


 void spi_init(void)
{

     P1SEL1 |= BIT6 + BIT7;
     P2SEL1 |= BIT2;


     //UCB1CTLW0 = UCCKPH | UCMST | UCMSB | UCSYNC | UCSSEL_2;
     //UCB1BRW = 0x01;

     // Configure the eUSCI_B0 module for 3-pin SPI at 1 MHz
     UCB0CTLW0 |= UCSWRST;
     // Use SMCLK at 1 MHz;
     UCB0CTLW0 |= UCSSEL__SMCLK + UCMODE_0 + UCMST + UCSYNC + UCMSB + UCCKPH;
     //UCB0BR0 |= 0x02; //Divide SMCLK by 2 to clock at 500 kHz
     UCB0BR0 |= 0x01; //1 MHz SPI clock
     UCB0BR1 |= 0x00;
     UCB0CTLW0 &= ~UCSWRST;
}


uint8_t spi_transfer(uint8_t inb)
{
    UCB0TXBUF = inb;
    while ( !(UCB0IFG & UCRXIFG) )  // Wait for RXIFG indicating remote byte received via SOMI
     ;
    return UCB0RXBUF;
}


uint16_t spi_transfer16(uint16_t inw)
{
     uint16_t retw;
     uint8_t *retw8 = (uint8_t *)&retw, *inw8 = (uint8_t *)&inw;

     UCB0TXBUF = inw8[1];
     while ( !(UCB0IFG & UCRXIFG) )
         ;
     retw8[1] = UCB0RXBUF;
     UCB0TXBUF = inw8[0];
     while ( !(UCB0IFG & UCRXIFG) )
         ;
     retw8[0] = UCB0RXBUF;
     return retw;
}



