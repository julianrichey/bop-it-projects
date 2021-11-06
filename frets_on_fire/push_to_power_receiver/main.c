//receiver
//msp430fr5969 (oops)
#include <msp430.h>
#include <stdint.h>
#include "msprf24.h"
#include "nrf_userconfig.h"

void red(void) {P4OUT |= BIT6;}
void green(void) {P1OUT |= BIT0;}
void clear_leds(void) {P4OUT &= ~BIT6; P1OUT &= ~BIT0;}

int main(void)
{
    PM5CTL0 &= ~LOCKLPM5;
    WDTCTL = WDTHOLD | WDTPW;

    //LEDs
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    P4DIR |= BIT6;
    P4OUT &= ~BIT6;

    //button presses
    P4DIR |= BIT2;      // button 0
    P4OUT &= ~BIT2;

    P2DIR |= BIT6;      // button 1
    P2OUT &= ~BIT6;

    P3DIR |= BIT5;      // button 2
    P3OUT &= ~BIT5;

    P3DIR |= BIT6;      // button 3
    P3OUT &= ~BIT6;

    //button releases
    P2DIR |= BIT5;      // button 0
    P2OUT &= ~BIT5;

    P4DIR |= BIT3;      // button 1
    P4OUT &= ~BIT3;

    P2DIR |= BIT4;      // button 2
    P2OUT &= ~BIT4;

    P3DIR |= BIT4;      // button 3
    P3OUT &= ~BIT4;

    uint8_t addr[] = {0xAA, 0xBB, 0xCC};
    uint8_t data[1];
    uint8_t pipe = 0x01;

    rf_crc          = RF24_EN_CRC;
    rf_addr_width   = sizeof(addr);
    rf_speed_power  = RF24_SPEED_2MBPS | RF24_POWER_MAX;
    rf_channel      = 120;

    msprf24_init();
    msprf24_set_pipe_packetsize(pipe, sizeof(data));
    msprf24_open_pipe(pipe, 0);
    w_rx_addr(pipe, addr);

    while (!msprf24_is_alive())
    {
        red();
        __delay_cycles(100000);
        clear_leds();
        __delay_cycles(100000);
    }
    red();
    green();
    __delay_cycles(900000);
    clear_leds();
    __delay_cycles(100000);

    msprf24_activate_rx();

    while (1)
    {
        if (rf_irq & RF24_IRQ_FLAGGED)
        {
            rf_irq &= ~RF24_IRQ_FLAGGED;
            msprf24_get_irq_reason();

            if (rf_irq & RF24_IRQ_RX || msprf24_rx_pending())
            {
                r_rx_payload(sizeof(data), data);
                green();
                switch (data[0])
                {
                    case 1:
                        P4OUT |= BIT2;
                        break;
                    case 2:
                        P2OUT |= BIT6;
                        break;
                    case 3:
                        P3OUT |= BIT5;
                        break;
                    case 4:
                        P3OUT |= BIT6;
                        break;
                    case 5:
                        P2OUT |= BIT5;
                        break;
                    case 6:
                        P4OUT |= BIT3;
                        break;
                    case 7:
                        P2OUT |= BIT4;
                        break;
                    case 8:
                        P3OUT |= BIT4;
                        break;
                    default:
                        clear_leds();   //jk
                        red();          //red
                        break;
                }
            }
            msprf24_irq_clear(RF24_IRQ_RX);
            __delay_cycles(5000);
        }
        P4OUT &= ~BIT2;
        P2OUT &= ~BIT6;
        P3OUT &= ~BIT5;
        P3OUT &= ~BIT6;
        P2OUT &= ~BIT5;
        P4OUT &= ~BIT3;
        P2OUT &= ~BIT4;
        P3OUT &= ~BIT4;
        clear_leds();
        __delay_cycles(5000); //Delays should make loop slow enough for Arduino to sample reliably. Would have to time release+press+print.
    }

}
