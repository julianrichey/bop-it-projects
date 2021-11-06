

#ifndef _spi_h_
#define _spi_h_

#include <stdint.h>

void spi_init();
uint8_t spi_transfer(uint8_t);
uint16_t spi_transfer16(uint16_t);

#endif
