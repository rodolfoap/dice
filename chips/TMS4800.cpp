#include "../chip_desc.h"
#include "rom.h"

/*
TMS 4800 (MK28000P?)
16384-Bit PMOS ROM (2048x8)

     +----------+
 VCC |1       24| OE1
  A0 |2       23| D0
  A1 |3   T   22| D1
  A2 |4   M   21| D2
  A3 |5   S   20| D3
  A4 |6   4   19| D4
  A5 |7   8   18| D5
  A9 |8   0   17| D6
-12V |9   0   16| D7
  A8 |10      15| A10
  A7 |11      14| OE2
  A6 |12      13| AR (Address Read)
     +----------+
*/

// Requires clock on Address Read pin.
// Address and CE signals are latched approx 50-250 ns after AR goes high.

// TODO: Add output enables. Omitted since this thing is confusing enough already...

static const uint8_t ADDR_PINS[] = { 2, 3, 4, 5, 6, 7, 12, 11, 10, 8, 15 };

static CHIP_LOGIC( TMS4800_AR_BUF )
{
    pin[i1] = pin[13];
}

template <int BIT>
CHIP_LOGIC( TMS4800_ADDR_LATCH )
{
    int P = ADDR_PINS[BIT];

    if(POS_EDGE_PIN(i1))
        pin[i2 + BIT] = pin[P];
    else
        pin[i2 + BIT] = prev_pin[P];
}

template <int BIT>
CHIP_LOGIC( TMS4800 )
{
    const RomDesc* rom = (const RomDesc*)custom_data;

    int addr = (pin[i12] << 10) | (pin[i11] << 9) | (pin[i10] << 8) | (pin[i9] << 7) | (pin[i8] << 6) |
               (pin[i7] << 5) | (pin[i6] << 4) | (pin[i5] << 3) | (pin[i4] << 2) | (pin[i3] << 1) | pin[i2];

    int P = 23 - BIT;
    
    pin[P] = (RomDesc::get_data(rom, addr) >> BIT) & 1;
}

#define CHIP_TMS4800_ADDR_LATCH( BIT ) \
    CHIP_START( TMS4800_ADDR_LATCH<BIT> ) \
        INPUT_PINS( i1, ADDR_PINS[BIT] ) \
        EVENT_PINS( i1 ) \
        OUTPUT_PIN( i2 + BIT ) \
        PREV_OUTPUT_PIN( i2 + BIT ) \
        OUTPUT_DELAY_NS( 100.0, 100.0 )

#define CHIP_TMS4800_OUTPUT( BIT ) \
    CHIP_START( TMS4800<BIT> ) \
        INPUT_PINS( i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12 ) \
        OUTPUT_PIN(  23 - BIT ) \
        OUTPUT_DELAY_NS( 450.0, 450.0 )

CHIP_DESC( TMS4800 ) = 
{
    CHIP_START( TMS4800_AR_BUF )
        INPUT_PINS( 13, 14, 24 )
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_NS( 50.0, 50.0 ),

    CHIP_TMS4800_ADDR_LATCH(0),
    CHIP_TMS4800_ADDR_LATCH(1),
    CHIP_TMS4800_ADDR_LATCH(2),
    CHIP_TMS4800_ADDR_LATCH(3),
    CHIP_TMS4800_ADDR_LATCH(4),
    CHIP_TMS4800_ADDR_LATCH(5),
    CHIP_TMS4800_ADDR_LATCH(6),
    CHIP_TMS4800_ADDR_LATCH(7),
    CHIP_TMS4800_ADDR_LATCH(8),
    CHIP_TMS4800_ADDR_LATCH(9),
    CHIP_TMS4800_ADDR_LATCH(10),

    CHIP_TMS4800_OUTPUT(0),
    CHIP_TMS4800_OUTPUT(1),
    CHIP_TMS4800_OUTPUT(2),
    CHIP_TMS4800_OUTPUT(3),
    CHIP_TMS4800_OUTPUT(4),
    CHIP_TMS4800_OUTPUT(5),
    CHIP_TMS4800_OUTPUT(6),
    CHIP_TMS4800_OUTPUT(7),

   	CHIP_DESC_END
};

