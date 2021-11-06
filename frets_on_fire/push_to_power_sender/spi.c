
#include <msp430.h>
#include "spi.h"


//todo change to 6.0,6.1,6.2 for spi or change to these to 5.x
void spi_init()
{
    /* Configure ports on MSP430FR5994 device for USCI_B1 */
    P5SEL0 |= (BIT0 | BIT1 | BIT2);
    P5SEL1 &= ~(BIT0 | BIT1 | BIT2);

    /* USCI_B specific SPI setup */
    UCB1CTLW0 |= UCSWRST;
    UCB1CTLW0 = UCCKPH | UCMST | UCMSB | UCSYNC | UCSSEL_2 | UCSWRST;
    UCB1BRW = 0x01;
    UCB1CTLW0 &= ~UCSWRST;
}

uint8_t spi_transfer(uint8_t inb)
{
    UCB1TXBUF = inb;
    while ( !(UCB1IFG & UCRXIFG) )  // Wait for RXIFG indicating remote byte received via SOMI
        ;
    return UCB1RXBUF;
}

uint16_t spi_transfer16(uint16_t inw)
{
    uint16_t retw;
    uint8_t *retw8 = (uint8_t *)&retw, *inw8 = (uint8_t *)&inw;

    UCB1TXBUF = inw8[1];
    while ( !(UCB1IFG & UCRXIFG) )
        ;
    retw8[1] = UCB1RXBUF;
    UCB1TXBUF = inw8[0];
    while ( !(UCB1IFG & UCRXIFG) )
        ;
    retw8[0] = UCB1RXBUF;
    return retw;
}
