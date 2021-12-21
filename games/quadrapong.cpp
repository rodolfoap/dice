#include "../circuit_desc.h"
#include "../circuit.h"

#define DEBUG
#undef DEBUG

#ifdef DEBUG
static VcdLogDesc vcd_log_desc
(
    "output_quadra.vcd",
    1, "A",
    2, "B",
    3, "C",
    4, "D",
    5, "E",
    6, "F",
    7, "G",
    8, "H",
    9, "I",
    10, "J",
    11, "K",
    12, "L",
    13, "M",
    14, "N",
    15, "O"
);
#endif

static Mono555Desc a10_555_desc(K_OHM(70.0), U_FARAD(0.1));
static Mono555Desc b10_555_desc(K_OHM(70.0), U_FARAD(0.1));
static Mono555Desc c10_555_desc(K_OHM(100.0), P_FARAD(220.0));
static Mono555Desc d10_555_desc(K_OHM(100.0), P_FARAD(220.0));
static Mono555Desc e10_555_desc(K_OHM(100.0), U_FARAD(0.1));
static Mono9602Desc a2_desc(K_OHM(47.0), U_FARAD(5.0), K_OHM(33.0), U_FARAD(47.0));

// TODO: Adjust values
static Paddle1VerticalDesc pad1_desc(17000.0, 145000.0, &a10_555_desc);
static Paddle2VerticalDesc pad2_desc(17000.0, 145000.0, &b10_555_desc);
static Paddle3HorizontalDesc pad3_desc(30000.0, 200000.0, &c10_555_desc);
static Paddle4HorizontalDesc pad4_desc(30000.0, 200000.0, &d10_555_desc);

static SeriesRCDesc c34_desc(K_OHM(1.0), U_FARAD(0.1)); // Capacitor C34, Resistor R34
static BufferDesc miss_buf_desc(DELAY_NS(15.0), DELAY_NS(15.0)); // Prevents 12 ns glitches on MISS signal from clocking score counters multiple times.
                                                                 // 74107 probably wouldn't clock on an 8 ns pulse? TODO: investigate

static DipswitchDesc dipswitch1_desc("coinage", "Coinage", 0, "1 Coin / 1 Credit", "1 Coin / 2 Credits");

static VIDEO_DESC( quadrapong )
    VIDEO_RESISTANCE(1, K_OHM(1.0))
    VIDEO_RESISTANCE(2, K_OHM(1.0))
    VIDEO_RESISTANCE(3, K_OHM(1.0))
    VIDEO_RESISTANCE(Video::HBLANK_PIN, K_OHM(1.0))
    VIDEO_CONTRAST(3.0)
VIDEO_DESC_END

static INPUT_DESC( quadrapong )
    INPUT_INFO(PADDLE1_VERTICAL_INPUT, "Move Left Paddle")
    INPUT_INFO(PADDLE2_VERTICAL_INPUT, "Move Right Paddle")
    INPUT_INFO(PADDLE3_HORIZONTAL_INPUT, "Move Top Paddle")
    INPUT_INFO(PADDLE4_HORIZONTAL_INPUT, "Move Bottom Paddle")
    INPUT_INFO(COIN_INPUT, {{ 1 }}, "Insert Coin")
    INPUT_INFO(START_INPUT, {{ 1 }}, "Start Game")
INPUT_DESC_END

CIRCUIT_LAYOUT( quadrapong )
	/**************************************************************************
	* Chips                                                                   *
	**************************************************************************/
    CHIP("XTAL", CLOCK_14_318_MHZ)

    CHIP("A1", 7474)
    CHIP("A2", 9602, &a2_desc)
    CHIP("A3", 7410)
    CHIP("A4", 7483) // Labelled 'A6' on schematic
    CHIP("A5", 7400)
    CHIP("A6", 7474)
    CHIP("A7", 7450)
    CHIP("A8", 9316)
    CHIP("A9", 7402)
    CHIP("A10", 555_Mono, &a10_555_desc)
    
    CHIP("B1", 7400)
    CHIP("B2", 7400)
    CHIP("B3", 7474)
    CHIP("B4", 9316)
    CHIP("B5", 7486)
    CHIP("B6", 7474)
    CHIP("B7", 7450)
    CHIP("B8", 9316)
    CHIP("B9", 7420)
    CHIP("B10", 555_Mono, &b10_555_desc)
    
    CHIP("C1", 7402)
    CHIP("C2", 7474)
    CHIP("C3", 7400)
    CHIP("C4", 9316)
    CHIP("C5", 7486)
    CHIP("C6", 7474)
    CHIP("C7", 7450)
    CHIP("C8", 7404)
    CHIP("C9", 7420)
    CHIP("C10", 555_Mono, &c10_555_desc)

    CHIP("D1", 7400)
    CHIP("D2", 7400)
    CHIP("D3", 7404)
    CHIP("D4", 7410)
    CHIP("D5", 7404)
    CHIP("D6", 7474)
    CHIP("D7", 7474)
    CHIP("D8", 9316)
    CHIP("D9", 7420)
    CHIP("D10", 555_Mono, &d10_555_desc)

    CHIP("E1", 7474)
    CHIP("E2", 7493)
    CHIP("E3", 7408)
    CHIP("E4", 7400)
    CHIP("E5", 7400)
    CHIP("E6", 7493)
    CHIP("E7", 7430)
    CHIP("E8", 9316)
    CHIP("E9", 7400)
    CHIP("E10", 555_Mono, &e10_555_desc)

    CHIP("F2", 7474)
    CHIP("F3", 9316)
    CHIP("F4", 7402)
    CHIP("F5", 7420)
    CHIP("F6", 7493)
    CHIP("F7", 7400)
    CHIP("F8", 7474)
    CHIP("F9", 7474)
    CHIP("F10", 7404)

    CHIP("H2", 7410)
    CHIP("H3", 9316)
    CHIP("H4", 7410)
    CHIP("H5", 7474)
    CHIP("H6", 74107)
    CHIP("H7", 7486)
    CHIP("H8", 7408)
    CHIP("H9", 7474)
    CHIP("H10", 7474)

    CHIP("J2", 7474)
    CHIP("J3", 74107)
    CHIP("J4", 7402)
    CHIP("J5", 7486)
    CHIP("J6", 7493)
    CHIP("J7", 7404)
    CHIP("J8", 7402)
    CHIP("J9", 7474)
    CHIP("J10", 7474)

    CHIP("K2", 7404)
    CHIP("K3", 7400)
    CHIP("K4", 7408)
    CHIP("K5", 7408)
    CHIP("K6", 7493)
    CHIP("K7", 9316)
    CHIP("K8", 9322)
    CHIP("K9", 7474)
    CHIP("K10", 7474)

    CHIP("L2", 74107)
    CHIP("L3", 7486)
    CHIP("L4", 7402)
    CHIP("L5", 7420)
    CHIP("L6", 7474)
    CHIP("L7", 9316)
    CHIP("L8", 9322)
    CHIP("L9", 74107)
    CHIP("L10", 74107)

    CHIP("M10", 7404)
    
    CHIP("PAD1", PADDLE1_VERTICAL_INPUT, &pad1_desc)
    PADDLE_CONNECTION("PAD1", "A10")

    CHIP("PAD2", PADDLE2_VERTICAL_INPUT, &pad2_desc)
    PADDLE_CONNECTION("PAD2", "B10")
    
    CHIP("PAD3", PADDLE3_HORIZONTAL_INPUT, &pad3_desc)
    PADDLE_CONNECTION("PAD3", "C10") 
    
    CHIP("PAD4", PADDLE4_HORIZONTAL_INPUT, &pad4_desc)
    PADDLE_CONNECTION("PAD4", "D10")

    CHIP("LATCH", LATCH)
    CHIP("COIN", COIN_INPUT)
    CHIP("START", START_INPUT)
    CHIP("DSW1", DIPSWITCH, &dipswitch1_desc)
    CHIP("C34", SERIES_RC, &c34_desc)
    CHIP("MISS_BUF", BUFFER, &miss_buf_desc)

#ifdef DEBUG
	CHIP("LOG1", VCD_LOG, &vcd_log_desc)
#endif
	
    VIDEO(quadrapong)
    INPUT(quadrapong)

    /**************************************************************************
    * Signals                                                                 *
	**************************************************************************/
    #define CLOCK "D7", 5
    #define CLOCK_n "D7", 6

    #define H1 "E6", 12
    #define H2 "E6", 9
    #define H4 "E6", 8
    #define H8 "E6", 11
    #define H16 "F6", 12
    #define H32 "F6", 9
    #define H64 "F6", 8
    #define H128 "F6", 11
    #define H256 "H6", 5
    #define H256_alt "J4", 10

    #define H2_n "D5", 4
    #define H8_n "D5", 2
    #define H128_n "D5", 6
    #define H256_n "H6", 6

    #define HRESET "D7", 8
    #define HRESET_n "D7", 9

    #define HBLANK "E4", 8
    #define HBLANK_n "E4", 6

    #define HSYNC "H5", 5
    #define HSYNC_n "H5", 6


    #define V1 "K6", 12
    #define V2 "K6", 9
    #define V4 "K6", 8
    #define V8 "K6", 11
    #define V16 "J6", 12
    #define V32 "J6", 9
    #define V64 "J6", 8
    #define V128 "J6", 11
    #define V128_alt "K3", 6
    #define V256 "H6", 3

    #define V2_n "J7", 2
    #define V32_n "J7", 10
    #define V64_n "J7", 6
    #define V128_n "J7", 8
    #define V256_n "H6", 2

    #define VRESET "H5", 8
    #define VRESET_n "H5", 9

    #define VBLANK "J4", 1
    #define VBLANK_n "J4", 13


    #define AH "D4", 12
    #define AH_n "D5", 8
    #define BG "D4", 6
    #define BG_conj "L3", 11
    #define CF "D4", 8
    #define C "F4", 10
    #define F "F4", 13
    #define RH "E5", 3

    #define NS "K5", 3
    #define NS_conj "L3", 3
    #define MT "K2", 12
    #define MT_n "F7", 6
    #define RV "K2", 2
    #define RV_n "K4", 6
    #define O_n "K4", 8
    #define _4Z "K4", 3
    #define _3Z "K4", 11

    #define N_LINE "J2", 8
    #define B_LINE "J2", 6
    #define H_DISP_n "H2", 6
    #define U_n "K3", 3

    #define _1_WIN_n "L2", 5
    #define _2_WIN_n "L2", 3

    #define _1BB "A8", 13
    #define _1CC "A8", 12
    #define _1DD "A8", 11

    #define _2BB "B8", 13
    #define _2CC "B8", 12
    #define _2DD "B8", 11

    #define _3BB "D8", 13
    #define _3CC "D8", 12
    #define _3DD "D8", 11

    #define _4BB "E8", 13
    #define _4CC "E8", 12
    #define _4DD "E8", 11

    #define BALL "C1", 13
    #define BALL_n "D3", 8

    #define VA1 "C4", 14
    #define VB1 "C4", 13

    #define _1A "L9", 3
    #define _2A "L9", 5

    #define _1B "K9", 5
    #define _2B "K9", 9
    #define _3B "L10", 3
    #define _4B "L10", 5

    #define _1C "J9", 5
    #define _2C "J9", 9
    #define _3C "K10", 5
    #define _4C "K10", 9
    
    #define _1D "H9", 5
    #define _2D "H9", 9

    #define _1D_n "H9", 6
    #define _2D_n "H9", 8
    #define _3D_n "J10", 6
    #define _4D_n "J10", 8

    #define SERVE "A1", 5

    #define START "H10", 5
    #define ATTRAC_n "F9", 6 // I guess writing out that last 'T' would have been too taxing ...

    
    /**************************************************************************
    * Layout - Page 1                                                         *
	**************************************************************************/
    // Clock Generation
    CONNECTION("XTAL", 1, "F10", 5)

    CONNECTION(VCC, "D7", 4)
    CONNECTION("D7", 6, "D7", 2)
    CONNECTION("F10", 6, "D7", 3)
    CONNECTION(VCC, "D7", 1)



    // Inverted H/V Signals
    CONNECTION(H2, "D5", 3)
    CONNECTION(H8, "D5", 1)
    CONNECTION(H128, "D5", 5)
    CONNECTION(V64, "J7", 5)
    CONNECTION(V32, "J7", 11)
    CONNECTION(V128, "J7", 9)

    CONNECTION("J7", 8, "K3", 4)
    CONNECTION("J7", 8, "K3", 5)



    // HSYNC
    CONNECTION(CLOCK, "E6", 14)
    CONNECTION("E6", 12, "E6", 1)
    CONNECTION("D7", 8, "E6", 2)
    CONNECTION("D7", 8, "E6", 3)

    CONNECTION("E6", 11, "F6", 14)
    CONNECTION("F6", 12, "F6", 1)
    CONNECTION("D7", 8, "F6", 2)
    CONNECTION("D7", 8, "F6", 3)

    CONNECTION(VCC, "H6", 8)
    CONNECTION("F6", 11, "H6", 9)
    CONNECTION(VCC, "H6", 11)
    CONNECTION("D7", 9, "H6", 10)

    CONNECTION("H6", 6, "J4", 9)
    CONNECTION("H6", 6, "J4", 8)

    CONNECTION(H256, "E7", 3)
    CONNECTION(H128, "E7", 5)
    CONNECTION(H64, "E7", 6)
    CONNECTION(H4, "E7", 12)
    CONNECTION(H2, "E7", 11)
    CONNECTION(H1, "E7", 4)
    CONNECTION(VCC, "E7", 1)
    CONNECTION(VCC, "E7", 2)

    CONNECTION(VCC, "D7", 10)
    CONNECTION("E7", 8, "D7", 12)
    CONNECTION(CLOCK, "D7", 11)
    CONNECTION(VCC, "D7", 13)


    CONNECTION(H16, "E4", 2)
    CONNECTION(H64, "E4", 1)

    CONNECTION(HRESET_n, "E4", 10)
    CONNECTION("E4", 6, "E4", 9)

    CONNECTION("E4", 8, "E4", 5)
    CONNECTION("E4", 3, "E4", 4)

    CONNECTION(H64, "D3", 1)

    CONNECTION(H32, "E4", 13)
    CONNECTION("D3", 2, "E4", 12)

    CONNECTION("D3", 2, "E3", 12)
    CONNECTION(HBLANK, "E3", 13)

    CONNECTION(VCC, "H5", 4)
    CONNECTION("E4", 11, "H5", 2)
    CONNECTION(H16, "H5", 3)
    CONNECTION("E3", 11, "H5", 1)




    // VSYNC
    CONNECTION(HRESET, "K6", 14)
    CONNECTION("K6", 12, "K6", 1)
    CONNECTION("H5", 8, "K6", 2)
    CONNECTION("H5", 8, "K6", 3)

    CONNECTION("K6", 11, "J6", 14)
    CONNECTION("J6", 12, "J6", 1)
    CONNECTION("H5", 8, "J6", 2)
    CONNECTION("H5", 8, "J6", 3)

    CONNECTION(VCC, "H6", 1)
    CONNECTION("J6", 11, "H6", 12)
    CONNECTION(VCC, "H6", 4)
    CONNECTION("H5", 9, "H6", 13)

    CONNECTION("H6", 3, "C9", 5)
    CONNECTION(V4, "C9", 4)
    CONNECTION(V1, "C9", 1)
    CONNECTION(V1, "C9", 2)

    CONNECTION(VCC, "H5", 10)
    CONNECTION("C9", 6, "H5", 12)
    CONNECTION(HRESET, "H5", 11)
    CONNECTION(VCC, "H5", 13)

    CONNECTION("H5", 8, "J4", 11)
    CONNECTION("J4", 1, "J4", 12)

    CONNECTION("J4", 13, "J4", 3)
    CONNECTION(V16, "J4", 2)

    CONNECTION(V8, "K2", 11)

    CONNECTION("K2", 10, "H4", 5)
    CONNECTION(V4, "H4", 4)
    CONNECTION("J4", 1, "H4", 3)

    CONNECTION(HSYNC, "L3", 10)
    CONNECTION("H4", 6, "L3", 9)




    // HM Window
    CONNECTION(H128_n, "F5", 9)
    CONNECTION(H64, "F5", 13)
    CONNECTION(H32, "F5", 12)
    CONNECTION(H16, "F5", 10)

    CONNECTION(H128, "E5", 12)
    CONNECTION(H8, "E5", 13)

    CONNECTION("F5", 8, "E5", 4)
    CONNECTION("E5", 8, "E5", 5)

    CONNECTION("E5", 6, "E5", 9)
    CONNECTION("E5", 11, "E5", 10)

    CONNECTION(H32, "D1", 2)
    CONNECTION(H8, "D1", 1)

    CONNECTION(VCC, "E1", 10)
    CONNECTION(VCC, "E1", 12)
    CONNECTION("E5", 8, "E1", 11)
    CONNECTION("D1", 3, "E1", 13)

    CONNECTION(H256, "D1", 12)
    CONNECTION("E5", 6, "D1", 13)

    CONNECTION("E1", 9, "D1", 5)
    CONNECTION(H256_n, "D1", 4)

    CONNECTION("D1", 11, "D1", 10)
    CONNECTION("D1", 6, "D1", 9)

    CONNECTION(H16, "L3", 5)
    CONNECTION(H256, "L3", 4)

    CONNECTION("D1", 8, "K5", 12)
    CONNECTION(H4, "K5", 13)

    CONNECTION("L3", 6, "D5", 13)

    CONNECTION("D1", 8, "C1", 5)
    CONNECTION("D1", 8, "C1", 6)

    CONNECTION("D1", 8, "E5", 2)
    CONNECTION(H256, "E5", 1)

    CONNECTION("L3", 6, "D4", 13)
    CONNECTION(H8_n, "D4", 2)
    CONNECTION("K5", 11, "D4", 1)

    CONNECTION("K5", 11, "D4", 3)
    CONNECTION(H8, "D4", 4)
    CONNECTION(H16, "D4", 5)

    CONNECTION("K5", 11, "D4", 11)
    CONNECTION("D5", 12, "D4", 9)
    CONNECTION(H8_n, "D4", 10)

    CONNECTION("D4", 12, "D5", 9)

    CONNECTION("D4", 6, "L3", 13)
    CONNECTION(H256, "L3", 12)

    CONNECTION("D4", 8, "F4", 8)
    CONNECTION(H256, "F4", 9)

    CONNECTION("D4", 8, "F4", 11)
    CONNECTION(H256_n, "F4", 12)




    // VM Window ?
    CONNECTION(V4, "J5", 4)
    CONNECTION(V128_n, "J5", 5)

    CONNECTION(V128, "J5", 10)
    CONNECTION(V64_n, "J5", 9)

    CONNECTION(V128_n, "J5", 12)
    CONNECTION(V16, "J5", 13)

    CONNECTION(V128, "J5", 2)
    CONNECTION(V8, "J5", 1)

    CONNECTION(V32, "K5", 4)
    CONNECTION("J5", 8, "K5", 5)

    CONNECTION("J5", 11, "J4", 6)
    CONNECTION("J5", 3, "J4", 5)

    CONNECTION("J5", 6, "K5", 10)
    CONNECTION("K5", 6, "K5", 9)

    CONNECTION("K5", 8, "K3", 13)
    CONNECTION("J5", 11, "K3", 12)

    CONNECTION("K3", 11, "K2", 5)

    CONNECTION("J5", 3, "F7", 9)
    CONNECTION("J5", 3, "F7", 10)

    CONNECTION("K5", 8, "K4", 2)
    CONNECTION(V128, "K4", 1)

    CONNECTION("K5", 8, "K4", 12)
    CONNECTION(V128_n, "K4", 13)

    CONNECTION("K2", 6, "K5", 1)
    CONNECTION("J5", 3, "K5", 2)

    CONNECTION("K2", 6, "F7", 4)
    CONNECTION("F7", 8, "F7", 5)

    CONNECTION("K4", 3, "K4", 4)
    CONNECTION("J4", 4, "K4", 5)

    CONNECTION("K4", 11, "K4", 10)
    CONNECTION("J4", 4, "K4", 9)

    CONNECTION(V128_n, "L3", 2)
    CONNECTION("K5", 3, "L3", 1)

    CONNECTION("F7", 6, "K2", 13)

    CONNECTION("K4", 6, "K2", 1)
	


    // Latch
    CONNECTION("E9", 3, "LATCH", 1)
    CONNECTION("F9", 8, "LATCH", 2)    

    CONNECTION("LATCH", 3, "C1", 8)
    CONNECTION("LATCH", 3, "C1", 9)



    // Paddles
    CONNECTION(V256_n, "A10", 2)
    CONNECTION(RV, "A10", 4)

    CONNECTION(V256_n, "B10", 2)
    CONNECTION(RV, "B10", 4)

    CONNECTION(HSYNC_n, "C10", 2)
    CONNECTION(RH, "C10", 4)

    CONNECTION(HSYNC_n, "D10", 2)
    CONNECTION(RH, "D10", 4)


    CONNECTION(B_LINE, "A9", 8)
    CONNECTION("A10", 3, "A9", 9)

    CONNECTION(B_LINE, "A9", 6)
    CONNECTION("B10", 3, "A9", 5)

    CONNECTION(N_LINE, "A9", 3)
    CONNECTION("C10", 3, "A9", 2)

    CONNECTION(N_LINE, "A9", 11)
    CONNECTION("D10", 3, "A9", 12)


    CONNECTION(GND, "A8", 3)
    CONNECTION(GND, "A8", 4)
    CONNECTION(GND, "A8", 5)
    CONNECTION(GND, "A8", 6)
    CONNECTION(VCC, "A8", 10)
    CONNECTION("C8", 10, "A8", 7)
    CONNECTION(HSYNC, "A8", 2)
    CONNECTION(VCC, "A8", 1)
    CONNECTION("A9", 10, "A8", 9)

    CONNECTION("A8", 15, "C8", 11)

    CONNECTION(_1D_n, "B9", 13)
    CONNECTION(C, "B9", 12)
    CONNECTION("C8", 10, "B9", 9)
    CONNECTION("A9", 10, "B9", 10)


    CONNECTION(GND, "B8", 3)
    CONNECTION(GND, "B8", 4)
    CONNECTION(GND, "B8", 5)
    CONNECTION(GND, "B8", 6)
    CONNECTION(VCC, "B8", 10)
    CONNECTION("C8", 12, "B8", 7)
    CONNECTION(HSYNC, "B8", 2)
    CONNECTION(VCC, "B8", 1)
    CONNECTION("A9", 4, "B8", 9)

    CONNECTION("B8", 15, "C8", 13)

    CONNECTION(_2D_n, "B9", 1)
    CONNECTION(F, "B9", 2)
    CONNECTION("C8", 12, "B9", 5)
    CONNECTION("A9", 4, "B9", 4)


    CONNECTION(GND, "D8", 3)
    CONNECTION(GND, "D8", 4)
    CONNECTION(GND, "D8", 5)
    CONNECTION(GND, "D8", 6)
    CONNECTION(VCC, "D8", 10)
    CONNECTION("C8", 2, "D8", 7)
    CONNECTION(CLOCK_n, "D8", 2)
    CONNECTION(VCC, "D8", 1)
    CONNECTION("A9", 1, "D8", 9)

    CONNECTION("D8", 15, "C8", 1)

    CONNECTION(_3D_n, "D9", 12)
    CONNECTION(O_n, "D9", 10)
    CONNECTION("C8", 2, "D9", 9)
    CONNECTION("A9", 1, "D9", 13)


    CONNECTION(GND, "E8", 3)
    CONNECTION(GND, "E8", 4)
    CONNECTION(GND, "E8", 5)
    CONNECTION(GND, "E8", 6)
    CONNECTION(VCC, "E8", 10)
    CONNECTION("C8", 4, "E8", 7)
    CONNECTION(CLOCK_n, "E8", 2)
    CONNECTION(VCC, "E8", 1)
    CONNECTION("A9", 13, "E8", 9)

    CONNECTION("E8", 15, "C8", 3)

    CONNECTION(_4D_n, "D9", 1)
    CONNECTION(RV_n, "D9", 2)
    CONNECTION("C8", 4, "D9", 4)
    CONNECTION("A9", 13, "D9", 5)


    


    // Scores
    CONNECTION(_1_WIN_n, "K8", 2)
    CONNECTION(_2_WIN_n, "K8", 3)
    CONNECTION(_4C, "K8", 5)
    CONNECTION(_3C, "K8", 6)
    CONNECTION(_1D, "K8", 14)
    CONNECTION(_2D, "K8", 13)
    CONNECTION(_4B, "K8", 11)
    CONNECTION(_3B, "K8", 10)
    CONNECTION(H256, "K8", 1)
    CONNECTION(GND, "K8", 15)

    CONNECTION(_3D_n, "L8", 2)
    CONNECTION(_4D_n, "L8", 3)
    CONNECTION(_1C, "L8", 5)
    CONNECTION(_2C, "L8", 6)
    CONNECTION(_1A, "L8", 14)
    CONNECTION(_2A, "L8", 13)
    CONNECTION(_1B, "L8", 11)
    CONNECTION(_2B, "L8", 10)
    CONNECTION(V128, "L8", 1)
    CONNECTION(GND, "L8", 15)

    CONNECTION("K7", 15, "J7", 3)

    CONNECTION("K8", 9, "K7", 3)
    CONNECTION("K8", 7, "K7", 4)
    CONNECTION(VCC, "K7", 5)
    CONNECTION(VCC, "K7", 6)
    CONNECTION("H8", 8, "K7", 10)
    CONNECTION("J7", 4, "K7", 7)
    CONNECTION(H2_n, "K7", 2)
    CONNECTION("H8", 8, "K7", 9)
    CONNECTION(VCC, "K7", 1)

    CONNECTION("L8", 4, "L6", 10)
    CONNECTION("K7", 15, "L6", 12)
    CONNECTION(H2_n, "L6", 11)
    CONNECTION(VCC, "L6", 13)

    CONNECTION("H8", 8, "L5", 2)
    CONNECTION("L6", 8, "L5", 1)
    CONNECTION(H2, "L5", 4)
    CONNECTION(MT, "L5", 5)

    CONNECTION("L7", 15, "J7", 13)

    CONNECTION(V2, "J7", 1)

    CONNECTION("L8", 12, "L7", 3)
    CONNECTION("L8", 9, "L7", 4)
    CONNECTION("L8", 7, "L7", 5)
    CONNECTION(VCC, "L7", 6)
    CONNECTION(V64, "L7", 10)
    CONNECTION("J7", 12, "L7", 7)
    CONNECTION(V2_n, "L7", 2)
    CONNECTION(V64, "L7", 9)
    CONNECTION(VCC, "L7", 1)

    CONNECTION(VCC, "L6", 4)
    CONNECTION("L7", 15, "L6", 2)
    CONNECTION(V2_n, "L6", 3)
    CONNECTION("L7", 15, "L6", 1)

    CONNECTION("K8", 12, "L4", 5)
    CONNECTION(V2, "L4", 6)

    CONNECTION(V128, "H7", 9)
    CONNECTION(H256, "H7", 10)

    CONNECTION("H7", 8, "L4", 3)
    CONNECTION("L6", 5, "L4", 2)

    CONNECTION(H64, "H8", 10)
    CONNECTION("H7", 8, "H8", 9)

    CONNECTION("L4", 1, "L5", 10)
    CONNECTION("L4", 4, "L5", 12)
    CONNECTION(V64, "L5", 9)
    CONNECTION(AH_n, "L5", 13)



    
    // Playfield
    CONNECTION(VCC, "J2", 10)
    CONNECTION("J2", 8, "J2", 12)
    CONNECTION(BG_conj, "J2", 11)
    CONNECTION(HRESET_n, "J2", 13)

    CONNECTION(H64, "D3", 3)

    CONNECTION("L8", 4, "E3", 4)
    CONNECTION(ATTRAC_n, "E3", 5)

    CONNECTION("E3", 6, "J3", 8)
    CONNECTION("D3", 4, "J3", 9)
    CONNECTION("E3", 6, "J3", 11)
    CONNECTION("J2", 9, "J3", 10)

    CONNECTION(NS, "H2", 3)
    CONNECTION("J3", 6, "H2", 5)
    CONNECTION("J2", 9, "H2", 4)

    CONNECTION(VCC, "J2", 4)
    CONNECTION("J2", 6, "J2", 2)
    CONNECTION(NS_conj, "J2", 3) // Schematic says NS, but then corners are wrong? Also N Line uses BG*
    CONNECTION(V256_n, "J2", 1)

    CONNECTION(V64, "H4", 2)
    CONNECTION(V32_n, "H4", 13)
    CONNECTION(V16, "H4", 1)

    CONNECTION(_1D_n, "L2", 8)
    CONNECTION("H4", 12, "L2", 9)
    CONNECTION(_1D_n, "L2", 11)
    CONNECTION("J2", 5, "L2", 10)

    CONNECTION(_2D_n, "L2", 1)
    CONNECTION("H4", 12, "L2", 12)
    CONNECTION(_2D_n, "L2", 4)
    CONNECTION("J2", 5, "L2", 13)

    CONNECTION("K8", 4, "H8", 5)
    CONNECTION(ATTRAC_n, "H8", 4)

    CONNECTION("J2", 6, "F4", 2)
    CONNECTION("H8", 6, "F4", 3)

    CONNECTION(BG, "K2", 9)

    CONNECTION("K2", 8, "K3", 2)
    CONNECTION("F4", 1, "K3", 1)

    CONNECTION("K3", 3, "K3", 10)
    CONNECTION("H2", 6, "K3", 9)




    /**************************************************************************
    * Layout - Page 2                                                         *
	**************************************************************************/
    // Vertical Ball Control
    CONNECTION(H256, "A7", 1)
    CONNECTION(_2BB, "A7", 13)
    CONNECTION(H256_n, "A7", 9)
    CONNECTION(_1BB, "A7", 10)

    CONNECTION(H256, "A7", 3)
    CONNECTION(_2CC, "A7", 2)
    CONNECTION(H256_n, "A7", 5)
    CONNECTION(_1CC, "A7", 4)

    CONNECTION(H256, "B7", 1)
    CONNECTION(_2DD, "B7", 13)
    CONNECTION(H256_n, "B7", 9)
    CONNECTION(_1DD, "B7", 10)

    CONNECTION(SERVE, "A6", 10)
    CONNECTION("A7", 8, "A6", 12)
    CONNECTION("A5", 8, "A6", 11)
    CONNECTION("D2", 6, "A6", 13)

    CONNECTION(ATTRAC_n, "A6", 4)
    CONNECTION("A7", 6, "A6", 2)
    CONNECTION("A5", 8, "A6", 3)
    CONNECTION(SERVE, "A6", 1)

    CONNECTION(ATTRAC_n, "B6", 10)
    CONNECTION("B7", 8, "B6", 12)
    CONNECTION("A5", 8, "B6", 11)
    CONNECTION(SERVE, "B6", 13)

    CONNECTION(VCC, "D6", 10)
    CONNECTION("B5", 3, "D6", 12)
    CONNECTION("L4", 13, "D6", 11)
    CONNECTION("A5", 8, "D6", 13)

    CONNECTION(H_DISP_n, "H8", 12) // Pin # not shown
    CONNECTION("A5", 6, "H8", 13) // Pin # not shown

    CONNECTION("B9", 8, "A5", 12)
    CONNECTION("B9", 6, "A5", 13)
        
    CONNECTION("B6", 9, "B5", 2)
    CONNECTION(V128, "B5", 1)

    CONNECTION(BALL_n, "L4", 11)
    CONNECTION("H8", 11, "L4", 12) // Pin # not shown

    CONNECTION(BALL, "A5", 9)
    CONNECTION("A5", 11, "A5", 10)

    CONNECTION("A6", 9, "B5", 10)
    CONNECTION("D6", 9, "B5", 9)

    CONNECTION("A6", 5, "B5", 13)
    CONNECTION("D6", 9, "B5", 12)

    CONNECTION("B6", 9, "B5", 5)
    CONNECTION("D6", 9, "B5", 4)

    CONNECTION("B5", 6, "D5", 11)

    // Schematic calls this chip A6
    CONNECTION("B5", 8, "A4", 10)
    CONNECTION("B5", 11, "A4", 8)
    CONNECTION("B5", 6, "A4", 3)
    CONNECTION(GND, "A4", 1)
    CONNECTION("D5", 10, "A4", 11)
    CONNECTION(VCC, "A4", 7)
    CONNECTION(VCC, "A4", 4)
    CONNECTION(GND, "A4", 16)
    CONNECTION(GND, "A4", 13)




    // Horizontal Ball Control
    CONNECTION(V128, "B7", 5)
    CONNECTION(_4BB, "B7", 4)
    CONNECTION(V128_n, "B7", 3)
    CONNECTION(_3BB, "B7", 2)

    CONNECTION(V128, "C7", 9)
    CONNECTION(_4CC, "C7", 10)
    CONNECTION(V128_n, "C7", 1)
    CONNECTION(_3CC, "C7", 13)

    CONNECTION(V128, "C7", 5)
    CONNECTION(_4DD, "C7", 4)
    CONNECTION(V128_n, "C7", 3)
    CONNECTION(_3DD, "C7", 2)

    CONNECTION(SERVE, "B6", 4)
    CONNECTION("B7", 6, "B6", 2)
    CONNECTION("A5", 6, "B6", 3)
    CONNECTION(VCC, "B6", 1)

    CONNECTION(ATTRAC_n, "C6", 10)
    CONNECTION("C7", 8, "C6", 12)
    CONNECTION("A5", 6, "C6", 11)
    CONNECTION(SERVE, "C6", 13)

    CONNECTION(ATTRAC_n, "C6", 4)
    CONNECTION("C7", 6, "C6", 2)
    CONNECTION("A5", 6, "C6", 3)
    CONNECTION(SERVE, "C6", 1)

    CONNECTION(VCC, "D6", 4)
    CONNECTION("C5", 3, "D6", 2)
    CONNECTION("L4", 10, "D6", 3)
    CONNECTION("A5", 6, "D6", 1)

    CONNECTION(U_n, "E3", 9)
    CONNECTION("A5", 8, "E3", 10)

    CONNECTION("D9", 8, "A5", 2)
    CONNECTION("D9", 6, "A5", 1)

    CONNECTION("C6", 5, "C5", 1)
    CONNECTION(H256, "C5", 2)

    CONNECTION(BALL_n, "L4", 9)
    CONNECTION("E3", 8,  "L4", 8)

    CONNECTION(BALL, "A5", 5)
    CONNECTION("A5", 3, "A5", 4)

    CONNECTION("B6", 5, "C5", 10)
    CONNECTION("C6", 5, "C5", 9)

    CONNECTION("C6", 9, "C5", 13)
    CONNECTION("C6", 5, "C5", 12)

    CONNECTION("C6", 5, "C5", 4)
    CONNECTION("D6", 6, "C5", 5)

    CONNECTION("C5", 8, "D3", 11)

    CONNECTION("C5", 11, "D3", 13)

    CONNECTION("C5", 8, "C3", 2)
    CONNECTION("C5", 11, "C3", 1)

    CONNECTION("C3", 3, "C3", 4)
    CONNECTION(VRESET, "C3", 5)

    CONNECTION("D3", 12, "B3", 10)
    CONNECTION("C3", 6, "B3", 12)
    CONNECTION("H4", 8, "B3", 11)
    CONNECTION(VCC, "B3", 13)

    CONNECTION("D3", 10, "B3", 4)
    CONNECTION("B3", 9, "B3", 2)
    CONNECTION("H4", 8, "B3", 3)
    CONNECTION(VCC, "B3", 1)

    CONNECTION("C3", 6, "A3", 3)
    CONNECTION("B3", 9, "A3", 4)
    CONNECTION("B3", 5, "A3", 5)

    // Horizontal Ball Control, Continued
    CONNECTION(VCC, "F2", 4)
    CONNECTION("A3", 6, "F2", 2)
    CONNECTION("H4", 8, "F2", 3)
    CONNECTION("E2", 11, "F2", 1)

    CONNECTION(VCC, "F2", 10)
    CONNECTION("F2", 5, "F2", 12)
    CONNECTION("H4", 8, "F2", 11)
    CONNECTION("E2", 8, "F2", 13)

    CONNECTION("A3", 6, "K2", 3)

    CONNECTION("F2", 8, "H2", 9)
    CONNECTION("F2", 6, "H2", 10)
    CONNECTION("K2", 4, "H2", 11)

    CONNECTION("E2", 11, "D2", 4)
    CONNECTION("E2", 8, "D2", 5)

    CONNECTION("D2", 6, "D2", 2)
    CONNECTION("C2", 8, "D2", 1)

    CONNECTION("D2", 3, "E2", 14)
    CONNECTION("E2", 12, "E2", 1)
    CONNECTION("A2", 10, "E2", 2)
    CONNECTION("A2", 10, "E2", 3)

    

    // Horizontal Ball Counter
    CONNECTION("H2", 8, "D3", 5)

    CONNECTION("C5", 6, "F4", 5)
    CONNECTION("D3", 6, "F4", 6)

    CONNECTION("D3", 6, "F3", 3)
    CONNECTION("F4", 4, "F3", 4)
    CONNECTION(GND, "F3", 5)
    CONNECTION(VCC, "F3", 6)
    CONNECTION(HBLANK_n, "F3", 10)
    CONNECTION(VCC, "F3", 7)
    CONNECTION(CLOCK, "F3", 2)
    CONNECTION("H4", 8, "F3", 9)
    CONNECTION(SERVE, "F3", 1)

    CONNECTION(GND, "H3", 3)
    CONNECTION(GND, "H3", 4)
    CONNECTION(GND, "H3", 5)
    CONNECTION(VCC, "H3", 6)
    CONNECTION(VCC, "H3", 10)
    CONNECTION("F3", 15, "H3", 7)
    CONNECTION(CLOCK, "H3", 2)
    CONNECTION("H4", 8, "H3", 9)
    CONNECTION(SERVE, "H3", 1)

    CONNECTION(VCC, "J3", 1)
    CONNECTION("H3", 15, "J3", 12)
    CONNECTION(VCC, "J3", 4)
    CONNECTION(SERVE, "J3", 13)

    CONNECTION("F3", 15, "H4", 9)
    CONNECTION("H3", 15, "H4", 10)
    CONNECTION("J3", 3, "H4", 11)

    CONNECTION("J3", 3, "F5", 1)
    CONNECTION("H3", 15, "F5", 2)
    CONNECTION("F3", 12, "F5", 5)
    CONNECTION("F3", 11, "F5", 4)



    // Vertical Ball Counter
    CONNECTION("A4", 9, "B4", 3)
    CONNECTION("A4", 6, "B4", 4)
    CONNECTION("A4", 2, "B4", 5)
    CONNECTION("A4", 15, "B4", 6)
    CONNECTION(VBLANK_n, "B4", 10)
    CONNECTION(VCC, "B4", 7)
    CONNECTION(HSYNC_n, "B4", 2)
    CONNECTION("C3", 11, "B4", 9)
    CONNECTION(SERVE, "B4", 1)

    CONNECTION(GND, "C4", 3)
    CONNECTION(GND, "C4", 4)
    CONNECTION(GND, "C4", 5)
    CONNECTION(GND, "C4", 6)
    CONNECTION(VCC, "C4", 10)
    CONNECTION("B4", 15, "C4", 7)
    CONNECTION(HSYNC_n, "C4", 2)
    CONNECTION("C3", 11, "C4", 9)
    CONNECTION(SERVE, "C4", 1)

    CONNECTION("B4", 15, "C3", 12)
    CONNECTION("C4", 15, "C3", 13)

    CONNECTION("C4", 15, "A3", 13)
    CONNECTION("B4", 11, "A3", 2)
    CONNECTION("B4", 12, "A3", 1)

    CONNECTION("F5", 6, "C1", 12)
    CONNECTION("A3", 12, "C1", 11)

    CONNECTION("C1", 13, "D3", 9)




    // Coin / Start Logic
    CONNECTION("COIN", 1, "C34", 1)
    CONNECTION(VCC, "C34", 2)

    CONNECTION(VCC, "E10", 4)
    CONNECTION("C34", 3, "E10", 2)
    
    CONNECTION("COIN", 1, "F10", 11)

    CONNECTION("E10", 3, "F10", 9)

    CONNECTION(VCC, "F9", 10)
    CONNECTION("F10", 10, "F9", 12)
    CONNECTION("F10", 8, "F9", 11)
    CONNECTION("F10", 10, "F9", 13)

    CONNECTION("F8", 8, "DSW1", 1)
    CONNECTION(VCC, "DSW1", 2)

    CONNECTION(VCC, "F8", 4)
    CONNECTION("DSW1", 3, "F8", 2)
    CONNECTION("F9", 9, "F8", 3)
    CONNECTION(VCC, "F8", 1)

    CONNECTION("C1", 10, "F8", 10)
    CONNECTION(VCC, "F8", 12)
    CONNECTION("F9", 5, "F8", 11)
    CONNECTION("F9", 8, "F8", 13)

    CONNECTION("F8", 9, "E9", 2)
    CONNECTION("F9", 5, "E9", 1)

    CONNECTION("F8", 8, "E9", 5)
    CONNECTION(START, "E9", 4)

    CONNECTION("C1", 10, "F9", 4)
    CONNECTION(VCC, "F9", 2)
    CONNECTION("H7", 6, "F9", 3)
    CONNECTION("E9", 6, "F9", 1)

    CONNECTION(VCC, "H10", 10)
    CONNECTION("F9", 5, "H10", 12)
    CONNECTION("START", 1, "H10", 11)
    CONNECTION("C1", 1, "H10", 13)

    CONNECTION(VCC, "H10", 4)
    CONNECTION("H10", 9, "H10", 2)
    CONNECTION(V256, "H10", 3)
    CONNECTION("C1", 10, "H10", 1)

    CONNECTION("H10", 6, "M10", 3)

    CONNECTION("C1", 10, "M10", 1)

    CONNECTION("M10", 4, "C1", 3)
    CONNECTION("M10", 2, "C1", 2)

    CONNECTION(_1D_n, "H7", 12)
    CONNECTION(_2D_n, "H7", 13)

    CONNECTION(_1D_n, "J8", 9)
    CONNECTION(_2D_n, "J8", 8)

    CONNECTION(_3D_n, "H7", 1)
    CONNECTION(_4D_n, "H7", 2)

    CONNECTION(_3D_n, "J8", 5)
    CONNECTION(_4D_n, "J8", 6)

    CONNECTION("J8", 10, "F7", 13)
    CONNECTION("H7", 3, "F7", 12)

    CONNECTION("H7", 11, "F7", 2)
    CONNECTION("J8", 4, "F7", 1)

    CONNECTION("F7", 11, "H7", 5)
    CONNECTION("F7", 3, "H7", 4)
    



    // Miss, Hit, Serve, Sound logic
    CONNECTION(AH, "B1", 1)
    CONNECTION(MT_n, "B1", 2)

    CONNECTION(BALL, "B1", 9)
    CONNECTION("B1", 3, "B1", 10)

    CONNECTION("B1", 8, "MISS_BUF", 1)
    CONNECTION("MISS_BUF", 2, "C8", 9)
    //CONNECTION("B1", 8, "C8", 9)

    CONNECTION("A5", 8, "E3", 2)
    CONNECTION("A5", 6, "E3", 1)

    CONNECTION(VCC, "C2", 10)
    CONNECTION("E3", 3, "C2", 12)
    CONNECTION("C4", 15, "C2", 11)
    CONNECTION("E3", 3, "C2", 13)

    CONNECTION(GND, "A2", 4)
    CONNECTION("B1", 8, "A2", 5)
    CONNECTION(VCC, "A2", 3)

    CONNECTION(START, "A2", 12)
    CONNECTION("A2", 7, "A2", 11)
    CONNECTION(VCC, "A2", 13)

    CONNECTION(VCC, "A1", 10)
    CONNECTION("A2", 9, "A1", 12)
    CONNECTION(V128, "A1", 11)
    CONNECTION("A2", 9, "A1", 13)

    CONNECTION(VCC, "A1", 4)
    CONNECTION("A1", 9, "A1", 2)
    CONNECTION(C, "A1", 3)
    CONNECTION(VCC, "A1", 1)

    CONNECTION("A2", 6, "B2", 9)
    CONNECTION(V32, "B2", 10)

    CONNECTION(VA1, "B2", 4)
    CONNECTION("C2", 8, "B2", 5)

    CONNECTION(BALL, "C3", 10)
    CONNECTION("K3", 8, "C3", 9)

    CONNECTION("C3", 8, "D2", 13)
    CONNECTION("D2", 8, "D2", 12)

    CONNECTION("D2", 11, "D2", 10)
    CONNECTION(VRESET_n, "D2", 9)

    CONNECTION(VCC, "C2", 4)
    CONNECTION("D2", 11, "C2", 2)
    CONNECTION(V256, "C2", 3)
    CONNECTION(VCC, "C2", 1)

    CONNECTION(VB1, "B2", 1)
    CONNECTION("C2", 5, "B2", 2)

    CONNECTION("B2", 8, "A3", 9)
    CONNECTION("B2", 6, "A3", 10)
    CONNECTION("B2", 3, "A3", 11)

    CONNECTION(ATTRAC_n, "B2", 12)
    CONNECTION("A3", 8, "B2", 13)

    CONNECTION("AUDIO", 1, "B2", 11)
    CONNECTION("AUDIO", i1, "AUDIO", Audio::OUTPUT_MONO)

    


    // Score Counters
    CONNECTION(H256, "J8", 2)
    CONNECTION("C1", 4, "J8", 3)

    CONNECTION("C1", 4, "J8", 11)
    CONNECTION(H256_n, "J8", 12)

    CONNECTION("J8", 1, "L9", 1)
    CONNECTION("C8", 8, "L9", 12)
    CONNECTION("J8", 1, "L9", 4)
    CONNECTION("H8", 3, "L9", 13)

    CONNECTION(VCC, "K9", 4)
    CONNECTION("K9", 6, "K9", 2)
    CONNECTION("L9", 2, "K9", 3)
    CONNECTION("H8", 3, "K9", 1)

    CONNECTION("E9", 8, "J9", 4)
    CONNECTION("J9", 6, "J9", 2)
    CONNECTION("K9", 6, "J9", 3)
    CONNECTION("E9", 11, "J9", 1)

    CONNECTION(VCC, "H9", 4)
    CONNECTION("H9", 6, "H9", 2)
    CONNECTION("J9", 6, "H9", 3)
    CONNECTION("H8", 3, "H9", 1)


    CONNECTION("J8", 13, "L9", 8)
    CONNECTION("C8", 8, "L9", 9)
    CONNECTION("J8", 13, "L9", 11)
    CONNECTION("H8", 3, "L9", 10)

    CONNECTION(VCC, "K9", 10)
    CONNECTION("K9", 8, "K9", 12)
    CONNECTION("L9", 6, "K9", 11)
    CONNECTION("H8", 3, "K9", 13)

    CONNECTION("E9", 8, "J9", 10)
    CONNECTION("J9", 8, "J9", 12)
    CONNECTION("K9", 8, "J9", 11)
    CONNECTION("E9", 11, "J9", 13)

    CONNECTION(VCC, "H9", 10)
    CONNECTION("H9", 8, "H9", 12)
    CONNECTION("J9", 8, "H9", 11)
    CONNECTION("H8", 3, "H9", 13)

    CONNECTION("F8", 5, "C8", 5)

    CONNECTION("C8", 6, "E9", 12)
    CONNECTION(START, "E9", 13)

    CONNECTION("F8", 5, "E9", 9)
    CONNECTION(START, "E9", 10)

    CONNECTION("E9", 11, "H8", 1)
    CONNECTION("E9", 8, "H8", 2)


    CONNECTION(_3Z, "L10", 1)
    CONNECTION("C8", 8, "L10", 12)
    CONNECTION(_3Z, "L10", 4)
    CONNECTION("E9", 8, "L10", 13)

    CONNECTION(VCC, "K10", 4)
    CONNECTION("K10", 6, "K10", 2)
    CONNECTION("L10", 2, "K10", 3)
    CONNECTION("E9", 8, "K10", 1)

    CONNECTION("E9", 11, "J10", 4)
    CONNECTION("J10", 6, "J10", 2)
    CONNECTION("K10", 6, "J10", 3)
    CONNECTION("E9", 8, "J10", 1)


    CONNECTION(_4Z, "L10", 8)
    CONNECTION("C8", 8, "L10", 9)
    CONNECTION(_4Z, "L10", 11)
    CONNECTION("E9", 8, "L10", 10)

    CONNECTION(VCC, "K10", 10)
    CONNECTION("K10", 8, "K10", 12)
    CONNECTION("L10", 6, "K10", 11)
    CONNECTION("E9", 8, "K10", 13)

    CONNECTION("E9", 11, "J10", 10)
    CONNECTION("J10", 8, "J10", 12)
    CONNECTION("K10", 8, "J10", 11)
    CONNECTION("E9", 8, "J10", 13)



    // Video Summing
    CONNECTION("B9", 8, "C9", 9)
    CONNECTION("B9", 6, "C9", 13)
    CONNECTION("D9", 8, "C9", 10)
    CONNECTION("D9", 6, "C9", 12)

    CONNECTION("C9", 8, "B1", 12)
    CONNECTION(ATTRAC_n, "B1", 13)

    CONNECTION("B1", 11, "H2", 13)
    CONNECTION("L5", 8, "H2", 2)
    CONNECTION("L5", 6, "H2", 1)
    
    CONNECTION("VIDEO", 1, "K3", 8)
    CONNECTION("VIDEO", 2, BALL)
    CONNECTION("VIDEO", 3, "H2", 12)

    CONNECTION("VIDEO", Video::HBLANK_PIN, HBLANK)
    CONNECTION("VIDEO", Video::VBLANK_PIN, VBLANK)


#ifdef DEBUG
    CONNECTION("LOG1", 1, "J8", 13)
    CONNECTION("LOG1", 2, _2A)
    CONNECTION("LOG1", 3, _2B)
    CONNECTION("LOG1", 4, _2C)
    CONNECTION("LOG1", 5, _2D)
    CONNECTION("LOG1", 6, "C8", 8)
    CONNECTION("LOG1", 7, "B1", 8)
    CONNECTION("LOG1", 8, "MISS_BUF", 2)
#endif

CIRCUIT_LAYOUT_END

