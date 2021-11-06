//sender
//msp430fr5994
#include <msp430.h>
#include <stdint.h>
#include "msprf24.h"
#include "nrf_userconfig.h"

#if 1 //Energy harvesting (otherwise, plugged in)

#define NUM_BUTTONS 4

volatile uint8_t* const button_read_ports[] =
{
    // button 0
    (uint8_t*)P4_BASE,         // down: P4.4
    (uint8_t*)P5_BASE,         // up:   P5.7

    // button 1
    (uint8_t*)P7_BASE,         // down: P7.3
    (uint8_t*)P5_BASE,         // up:   P5.3

    // button 2
    (uint8_t*)P8_BASE,         // down: P8.3
    (uint8_t*)P2_BASE,         // up:   P2.5

    // button 3
    (uint8_t*)P8_BASE,         // down: P8.1
    (uint8_t*)P8_BASE          // up:   P8.2
};

const uint8_t button_read_pins[] =
{
    // button 0
    4,               // down
    7,               // up

    // button 1
    3,               // down
    3,               // up

    // button 2
    3,
    5,

    // button 3
    1,
    2
};

const uint8_t button_read_mmio_offsets[] =
{
    OFS_P2IN,
    OFS_P1IN,

    OFS_P1IN,
    OFS_P1IN,

    OFS_P2IN,
    OFS_P2IN,

    OFS_P2IN,
    OFS_P2IN
};

volatile uint8_t* const button_clear_ports[] =
{
    // button 0
    (uint8_t*)P3_BASE,         // down: P3.6
    (uint8_t*)P3_BASE,         // up:   P3.7

    // button 1
    (uint8_t*)P3_BASE,         // down: P3.4
    (uint8_t*)P3_BASE,         // up:   P3.5

    // button 2
    (uint8_t*)P4_BASE,         // down: P4.3
    (uint8_t*)P2_BASE,         // up:   P2.6

    // button 3
    (uint8_t*)P4_BASE,         // down: P4.1
    (uint8_t*)P4_BASE          // up:   P4.2
};

const uint8_t button_clear_pins[] =
{
    // button 0
    6,               // down
    7,               // up

    // button 1
    4,               // down
    5,               // up

    // button 2
    3,               // down
    6,               // up

    // button 3
    1,               // down
    2                // up
};

const uint8_t button_clear_mmio_offsets[] =
{
    OFS_P1IN,
    OFS_P1IN,

    OFS_P1IN,
    OFS_P1IN,

    OFS_P2IN,
    OFS_P2IN,

    OFS_P2IN,
    OFS_P2IN,
};


int main(void)
{
    WDTCTL = WDTHOLD | WDTPW;
    PM5CTL0 &= ~LOCKLPM5;
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // make button read pins inputs; make button write pins outputs and initialize the output to 0
    int i;
    for (i = 0; i < (2 * NUM_BUTTONS); i++)
    {
        *(button_read_ports[i]  + button_read_mmio_offsets[i]  + OFS_P1DIR) &= ~(1 << button_read_pins[i]);
        *(button_clear_ports[i] + button_clear_mmio_offsets[i] + OFS_P1OUT) &= ~(1 << button_clear_pins[i]);
        *(button_clear_ports[i] + button_clear_mmio_offsets[i] + OFS_P1DIR) |=  (1 << button_clear_pins[i]);
    }

    uint8_t char_map[] = { 1, 5, 2, 6, 3, 7, 4, 8 };
    uint8_t char_to_send = 0;

    // figure out which button was pressed
    for (i = 0; i < (2 * NUM_BUTTONS); i++)
    {
        if (!(*(button_read_ports[i] + button_read_mmio_offsets[i] + OFS_P1IN) & (1 << button_read_pins[i])))
            char_to_send = char_map[i];
        *(button_clear_ports[i] + button_clear_mmio_offsets[i] + OFS_P1OUT) |= (1 << button_clear_pins[i]);
    }

    //Straight shot: if fails no resend, so no attention paid to rf_irq
    rf_crc         = RF24_EN_CRC;
    rf_addr_width  = 3;
    rf_speed_power = RF24_SPEED_2MBPS | RF24_POWER_MIN;
    rf_channel     = 120;
    uint8_t addr[3] = {0xAA, 0xBB, 0xCC};
    uint8_t payload[] = {char_to_send}; //Hope char_to_send is not 0. If it is, receiver flashes red
    msprf24_init();
    w_tx_addr(addr);
    w_tx_payload_noack(sizeof(payload), payload);
    msprf24_activate_tx();
    __delay_cycles(1000); //There should probably be some delay to allow tx before turning on the LED?
    P1OUT |= BIT0; //Consume power, indicate send
    while(1) {}
}

#else //Plugged in

void red(void) {P1OUT |= BIT0;}
void green(void) {P1OUT |= BIT1;}
void clear_leds(void) {P1OUT &= ~BIT0; P1OUT &= ~BIT1;}

int main(void)
{
    WDTCTL = WDTHOLD | WDTPW;
    PM5CTL0 &= ~LOCKLPM5;
    P1DIR |= (BIT1 | BIT0);
    P1OUT &= ~(BIT1 | BIT0);

    uint8_t addr[] = {0xAA, 0xBB, 0xCC};
    uint8_t data[] = {0x01};

    rf_crc          = RF24_EN_CRC;
    rf_addr_width   = sizeof(addr);
    rf_speed_power  = RF24_SPEED_2MBPS | RF24_POWER_MIN;
    rf_channel      = 120;

    msprf24_init();
    msprf24_standby();
    w_tx_addr(addr);

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

    while(1) {
        __delay_cycles(500000);
        w_tx_payload_noack(sizeof(data), data);
        msprf24_activate_tx();
        LPM4;

        if (rf_irq & RF24_IRQ_FLAGGED) {
            rf_irq &= ~RF24_IRQ_FLAGGED;

            msprf24_get_irq_reason();

            green();
            __delay_cycles(100000);
            clear_leds();

            msprf24_irq_clear(rf_irq);
        }
    }
}

#endif
