#include "chip_desc.h"

#include "chips/audio.h"
#include "chips/input.h"
#include "chips/video.h"
#include "chips/dipswitch.h"
#include "chips/vcd_log.h"
#include "chips/wav_log.h"

#include "chips/555astable.h"
#include "chips/555mono.h"
#include "chips/555pwm.h"
#include "chips/74121.h"
#include "chips/82S16.h"
#include "chips/8225.h"
#include "chips/9602.h"
#include "chips/2533.h"

#include "chips/capacitor.h"
#include "chips/diode_matrix.h"
#include "chips/rom.h"

#include "chips/mixer.h"
#include "chips/566.h"

extern CHIP_DESC( CLOCK_14_318_MHZ );
extern CHIP_DESC( CLOCK_12_096_MHZ );
extern CHIP_DESC( CLOCK_10_733_MHZ );
extern CHIP_DESC( CLOCK_8_MHZ );
extern CHIP_DESC( CLOCK_6_MHZ );

extern CHIP_DESC( 7400 );
extern CHIP_DESC( 7402 );
extern CHIP_DESC( 7404 );
extern CHIP_DESC( 74H04 );
extern CHIP_DESC( 7405 );
extern CHIP_DESC( 7407 );
extern CHIP_DESC( 7408 );
extern CHIP_DESC( 74H08 );
extern CHIP_DESC( 7410 );
extern CHIP_DESC( 7411 );
extern CHIP_DESC( 7420 );
extern CHIP_DESC( 74S20 );
extern CHIP_DESC( 7421 );
extern CHIP_DESC( 7425 );
extern CHIP_DESC( 7427 );
extern CHIP_DESC( 7430 );
extern CHIP_DESC( 7432 );
extern CHIP_DESC( 7437 );
extern CHIP_DESC( 7442 );
extern CHIP_DESC( 7448 );
extern CHIP_DESC( 7450 );
extern CHIP_DESC( 7451 );
extern CHIP_DESC( 7454 );
extern CHIP_DESC( 7474 );
extern CHIP_DESC( 7475 );
extern CHIP_DESC( 7476 );
extern CHIP_DESC( 7483 );
extern CHIP_DESC( 7485 );
extern CHIP_DESC( 7486 );
extern CHIP_DESC( 7490 );
extern CHIP_DESC( 7492 );
extern CHIP_DESC( 7493 );
extern CHIP_DESC( 7493A );
extern CHIP_DESC( 7495 );
extern CHIP_DESC( 7496 );
extern CHIP_DESC( 74107 );
extern CHIP_DESC( 74109 );
extern CHIP_DESC( 74S112 );
extern CHIP_DESC( 74116 );
extern CHIP_DESC( 74150 );
extern CHIP_DESC( 74S151 );
extern CHIP_DESC( 74151 );
extern CHIP_DESC( 74153 );
extern CHIP_DESC( 74155 );
extern CHIP_DESC( 74164 );
extern CHIP_DESC( 74165 );
extern CHIP_DESC( 74166 );
extern CHIP_DESC( 74174 );
extern CHIP_DESC( 74S174 );
extern CHIP_DESC( 74175 );
extern CHIP_DESC( 74191 );
extern CHIP_DESC( 74192 );
extern CHIP_DESC( 74193 );
extern CHIP_DESC( 74194 );
extern CHIP_DESC( 74279 );
extern CHIP_DESC( 9301 );
extern CHIP_DESC( 9310 );
extern CHIP_DESC( 9311 );
extern CHIP_DESC( 9312 );
extern CHIP_DESC( 9314 );
extern CHIP_DESC( 9316 );
extern CHIP_DESC( 9321 );
extern CHIP_DESC( 9322 );
extern CHIP_DESC( 8277 );

extern CHIP_DESC( 74S287 );
extern CHIP_DESC( 82S16 );
extern CHIP_DESC( 82S115 );
extern CHIP_DESC( 82S123 );
extern CHIP_DESC( 82S131 );
extern CHIP_DESC( TMS4800 );

extern CHIP_DESC( LATCH );

extern CHIP_DESC( CLK_GATE );
extern CHIP_DESC( CLK_GATE_n );

extern CHIP_DESC( WIRED_AND );
extern CHIP_DESC( WIRED_OR );

static CHIP_ALIAS( 7406, 7404 );
static CHIP_ALIAS( 7417, 7407 );
static CHIP_ALIAS( 7413, 7420 );
static CHIP_ALIAS( 7414, 7404 );
static CHIP_ALIAS( 7453, 7454 );
static CHIP_ALIAS( 74132, 7400 );
static CHIP_ALIAS( 74145, 7442 );
static CHIP_ALIAS( 74154, 9311 );
static CHIP_ALIAS( 74161, 9316 );
static CHIP_ALIAS( 74157, 9322 );
static CHIP_ALIAS( 8574, 74S287 );
static CHIP_ALIAS( 82S129, 74S287 );
static CHIP_ALIAS( 74S200, 82S16 );

typedef Ram82S16Desc Ram74S200Desc;
