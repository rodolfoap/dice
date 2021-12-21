#include "../circuit_desc.h"

#define DEBUG
#undef DEBUG

static Mono555Desc b9_555_desc(K_OHM(200.0), P_FARAD(220.0));
static Mono555Desc c9_555_desc(K_OHM(100.0), P_FARAD(220.0));

static Mono9602Desc d1_desc(K_OHM(47.0), U_FARAD(5.0), K_OHM(47.0), U_FARAD(10.0));
static Mono9602Desc k1_desc(K_OHM(33.0), U_FARAD(47.0), K_OHM(47.0), U_FARAD(10.0));

static Paddle2HorizontalDesc pad2_desc(150000.0, 250000.0, &b9_555_desc);
static Paddle1HorizontalDesc pad1_desc(42000.0, 132000.0, &c9_555_desc);

static DipswitchDesc dipswitch1_desc("winning_score", "Winning Score", 0, "11", "15");
static DipswitchDesc dipswitch2_desc("coinage", "Coinage", 0, "1 Coin / 1 Credit", "1 Coin / 2 Credits");

static INPUT_DESC( rebound )
    INPUT_INFO(PADDLE1_HORIZONTAL_INPUT, "Move Left Paddle")
    INPUT_INFO(PADDLE2_HORIZONTAL_INPUT, "Move Right Paddle")
    INPUT_INFO(COIN_INPUT, {{ 1 }}, "Insert Coin")
    INPUT_INFO(START_INPUT, {{ 1 }}, "Start Game")
INPUT_DESC_END


#ifdef DEBUG
static VcdLogDesc vcd_log_desc
(
    "rebound.vcd",
    1, "CLK",
    2, "1H",
	3, "2H",
	4, "4H",
	5, "8H",
    6, "16H", 
	7, "32H",
    8, "64H",
    9, "128H",
    10, "256H",
    11, "/CLK",
    12, "F4.6",
    13, "/HRESET"
);
#endif

CIRCUIT_LAYOUT( rebound )
    /**************************************************************************
		Chips
	**************************************************************************/
	CHIP("CLOCK", CLOCK_14_318_MHZ)
    
    CHIP("A1", 7400)
		
	CHIP("B1", 7400)
	CHIP("B2", 7474)
	CHIP("B3", 74107)
	CHIP("B4", 9316)
	CHIP("B5", 7404)
	CHIP("B6", 9322)
	CHIP("B7", 9316)
	CHIP("B8", 7410)
	CHIP("B9", 555_Mono, &b9_555_desc)
	
	CHIP("C1", 7493)
	CHIP("C2", 7450)
	CHIP("C3", 7410)
	CHIP("C4", 9316)
	CHIP("C5", 7486)
	CHIP("C6", 7474)
	CHIP("C7", 9316)
	CHIP("C8", 7402)
	CHIP("C9", 555_Mono, &c9_555_desc)
	
	CHIP("D1", 9602, &d1_desc)
	CHIP("D2", 7402)
	CHIP("D3", 9316)
	CHIP("D4", 9316)
	CHIP("D5", 7400)
	CHIP("D6", 7474)
	CHIP("D7", 7400)
	CHIP("D8", 7474)
	
	CHIP("E1", 7400)
	CHIP("E2", 7483)
	CHIP("E3", 9316)
	CHIP("E4", 74193)
	CHIP("E5", 7408)
	CHIP("E6", 7402)
	CHIP("E7", 7474)
	CHIP("E8", 7474)
	CHIP("E9", 7404)
	
	CHIP("F1", 7474)
	CHIP("F2", 7408)
	CHIP("F3", 74107)
	CHIP("F4", 7420)
	CHIP("F5", 7493)
	CHIP("F6", 7410)
	CHIP("F7", 7408)
	CHIP("F8", 7410)
	CHIP("F9", 7404)
	
	CHIP("H1", 7400)
	CHIP("H2", 7410)
	CHIP("H3", 7493)
	CHIP("H4", 74107)
	CHIP("H5", 7493)
	CHIP("H6", 7427)
	CHIP("H7", 7410)
	CHIP("H8", 7430)
	CHIP("H9", 74107)
	
	CHIP("J1", 7474)
	CHIP("J2", 7474)
	CHIP("J3", 7493)
	CHIP("J4", 7474)
	CHIP("J5", 7486)
	CHIP("J6", 7400)
	CHIP("J7", 7410)
	CHIP("J8", 74153)
	CHIP("J9", 7490)
	
	CHIP("K1", 9602, &k1_desc)
	CHIP("K2", 7493)
	CHIP("K3", 9316)
	CHIP("K4", 7400)
	CHIP("K5", 7474)
	CHIP("K6", 7402)
	CHIP("K7", 7448)
	CHIP("K8", 74153)
	CHIP("K9", 7490)

    CHIP("PAD1", PADDLE1_HORIZONTAL_INPUT, &pad1_desc)    
    PADDLE_CONNECTION("PAD1", "C9")

    CHIP("PAD2", PADDLE2_HORIZONTAL_INPUT, &pad2_desc)
    PADDLE_CONNECTION("PAD2", "B9")

    CHIP("LATCH", LATCH)
    CHIP("COIN1", COIN_INPUT)
    CHIP("START", START_INPUT)

    CHIP("DSW1", DIPSWITCH, &dipswitch1_desc)
    CHIP("DSW2", DIPSWITCH, &dipswitch2_desc)

    INPUT(rebound)

#ifdef DEBUG
	CHIP("LOG1", VCD_LOG, &vcd_log_desc)
#endif

	/**************************************************************************
		Signals
	**************************************************************************/	
    #define CLK "J4", 5
    #define CLK_n "J4", 6

	#define H1 "H5", 12
	#define H2 "H5", 9
	#define H4 "H5", 8
	#define H8 "H5", 11

	#define H16 "F5", 12
	#define H32 "F5", 9
	#define H64 "F5", 8
	#define H128 "F5", 11
	
	#define H256 "H4", 3
	#define H256_n "H4", 2
	
	#define HRESET "J4", 8
	#define HRESET_n "J4", 9
	
    #define HBLANK "K5", 5
	#define HBLANK_n "K5", 6
	
	#define HSYNC "K5", 9
	#define HSYNC_n "K5", 8

	#define V1 "J3", 12
	#define V2 "J3", 9
	#define V4 "J3", 8
	#define V8 "J3", 11

	#define V16 "H3", 12
	#define V32 "H3", 9
	#define V64 "H3", 8
	#define V128 "H3", 11

	#define V256 "H4", 5
	#define V256_n "H4", 6
	
	#define VRESET "J2", 8
	#define VRESET_n "J2", 9
	
	#define VSYNC_n "H1", 11
	
	#define NET_n "B8", 12
	#define PADDLE_12 "K6", 10
	#define NET_HEIGHT_n "B5", 10
	
	#define STOP1_n "F7", 8
	#define STOP2_n "F7", 6
	
	#define DISP_PAD1_n "B8", 6
	#define DISP_PAD2_n "B8", 8
	
	#define BACKWARDS_n "D6", 5
	#define DP "C6", 5
	#define CP_n "D6", 8
	#define BP_n "C6", 8
	
	#define V64_n "C8", 13
	
	#define HIT "E6", 10
	#define HIT_n "D5", 3
	
	#define SERVE "J1", 8
	#define SERVE_n "J1", 9
	#define SERVE_TIME "K1", 6
	#define SERVE_TIME_n "K1", 7
	
	#define RIGHT_SCORE "A1", 11
	#define LEFT_SCORE "A1", 3
	
	#define BALL_RETURN "J5", 3
	#define MISS_n "D2", 1
	
	#define BALL "E6", 4
	#define BALL_n "E6", 1
	
	#define LSB1 "K9", 12
	#define LSB2 "J9", 12
	#define STOP "J6", 8
	
	#define ATTRACT "E8", 5
	#define ATTRACT_n "E8", 6 
	
	#define START "E7", 9 
	#define START_n "E7", 8

	
	/**************************************************************************
		Layout
	**************************************************************************/
	/* HRESET Circuit */	
	CONNECTION("CLOCK", 1, "J4", 3)
    CONNECTION("J4", 6, "J4", 2)
    CONNECTION(VCC, "J4", 4)
    CONNECTION(VCC, "J4", 1)
    
    CONNECTION(CLK, "H5", 14)
	CONNECTION("J4", 8, "H5", 2)
	CONNECTION("J4", 8, "H5", 3)
	CONNECTION("H5", 12, "H5", 1)

	CONNECTION("H5", 11, "F5", 14)
	CONNECTION("J4", 8, "F5", 2)
	CONNECTION("J4", 8, "F5", 3)
	CONNECTION("F5", 12, "F5", 1)
	
	CONNECTION("F5", 11, "H4", 12)
	CONNECTION("J4", 9, "H4", 13)
	CONNECTION(VCC, "H4", 1)
	CONNECTION(VCC, "H4", 4)
	
	CONNECTION(H256, "F4", 5)
	CONNECTION(H128, "F4", 2)
	CONNECTION(H64, "F4", 1)
	CONNECTION(H4, "F4", 4)
	
	CONNECTION(CLK_n, "J4", 11)
	CONNECTION(VCC, "J4", 10)
	CONNECTION(VCC, "J4", 13)
	CONNECTION("F4", 6, "J4", 12)
	
	
	
	/* HBLANK/HSYNC Circuit */
	CONNECTION(H16, "K4", 2)
	CONNECTION(H64, "K4", 1)
	
	CONNECTION(HRESET_n, "K5", 4)
	CONNECTION(GND, "K5", 2)
	CONNECTION(GND, "K5", 3)
	CONNECTION("K4", 3, "K5", 1)
	
	CONNECTION(H64, "B5", 3)
	
	CONNECTION(H32, "K4", 13)
	CONNECTION("B5", 4, "K4", 12)
	
	CONNECTION("B5", 4, "F7", 1)
	CONNECTION("K5", 5, "F7", 2)
	
	CONNECTION(VCC, "K5", 10)
	CONNECTION("K4", 11, "K5", 12)
	CONNECTION(H16, "K5", 11)
	CONNECTION("F7", 3, "K5", 13)
	
	
	
	/* VRESET Circuit */
	CONNECTION("J4", 8, "J3", 14)
	CONNECTION("J2", 8, "J3", 2)
	CONNECTION("J2", 8, "J3", 3)
	CONNECTION("J3", 12, "J3", 1)

	CONNECTION("J3", 11, "H3", 14)
	CONNECTION("J2", 8, "H3", 2)
	CONNECTION("J2", 8, "H3", 3)
	CONNECTION("H3", 12, "H3", 1)	
	
	CONNECTION("H3", 11, "H4", 9)
	CONNECTION("J2", 9, "H4", 10)
	CONNECTION(VCC, "H4", 8)
	CONNECTION(VCC, "H4", 11)
	
	CONNECTION(V256, "H2", 1)
	CONNECTION(V4, "H2", 2)
	CONNECTION(V1, "H2", 13)

	CONNECTION("J4", 8, "J2", 11)
	CONNECTION(VCC, "J2", 10)
	CONNECTION(VCC, "J2", 13)
	CONNECTION("H2", 12, "J2", 12)
	
	
	
	/* VSYNC Circuit */
	CONNECTION(V8, "H2", 3)
	CONNECTION(V4, "H2", 4)
	CONNECTION(V2, "H2", 5)	
	
	CONNECTION("J2", 9, "H1", 1)
	CONNECTION("H1", 11, "H1", 2)
	
	CONNECTION("H1", 3, "H1", 12)
	CONNECTION("H2", 6, "H1", 13)
	
	
	
	/* Net Circuit */
	CONNECTION(SERVE_n, "K1", 11)
	CONNECTION(BALL_RETURN, "K1", 12)
	CONNECTION(VCC, "K1", 13)
	
	CONNECTION("J1", 6, "J1", 2)
	CONNECTION("K1", 9, "J1", 3)
	CONNECTION(VCC, "J1", 4)
	CONNECTION("H1", 6, "J1", 1)
	
	CONNECTION(SERVE, "K2", 2)
	CONNECTION(SERVE, "K2", 3)
	CONNECTION("J1", 5, "K2", 14)
	CONNECTION("K2", 12, "K2", 1)
	
	CONNECTION("K2", 9, "H1", 4)
	CONNECTION("K2", 11, "H1", 5)
	
	CONNECTION(V128, "K3", 9)
	CONNECTION(VCC, "K3", 3)
	CONNECTION("K2", 9, "K3", 4)
	CONNECTION("K2", 8, "K3", 5)
	CONNECTION("K2", 11, "K3", 6)
	CONNECTION("K4", 8, "K3", 7)
	CONNECTION(VCC, "K3", 10)
	CONNECTION(V4, "K3", 2)
	CONNECTION(VCC, "K3", 1)	//Not shown on schematic
	
	CONNECTION(V64, "H2", 11)
	CONNECTION(V32, "H2", 10)
	CONNECTION(V128, "H2", 9)
	
	CONNECTION(V16, "B1", 12)
	CONNECTION(V8, "B1", 13)
	
	CONNECTION("H2", 8, "K6", 9)
	CONNECTION("B1", 11, "K6", 8)
	
	CONNECTION(V128, "K4", 10)
	CONNECTION("K3", 15, "K4", 9)
	
	CONNECTION("H2", 8, "K4", 4)
	CONNECTION("K4", 8, "K4", 5)
	
	CONNECTION("K4", 6, "B5", 11)
	
	CONNECTION("K4", 6, "B2", 10)
	CONNECTION(H256, "B2", 12)
	CONNECTION(H2, "B2", 11)
	CONNECTION(VCC, "B2", 13)
	
	CONNECTION(V4, "B8", 2)
	CONNECTION(H256, "B8", 1)
	CONNECTION("B2", 8, "B8", 13)
	


	/* Stop Circuit */
	CONNECTION(H256, "F6", 3)
	CONNECTION(H128, "F6", 5)
	CONNECTION(H16,  "F6", 4)
	
	CONNECTION(H64, "F6", 11)
	CONNECTION(H128, "F6", 9)
	CONNECTION(H32, "F6", 10)
	
	CONNECTION("F6", 6, "F7", 5)
	CONNECTION(ATTRACT_n, "F7", 4)
	
	CONNECTION("F6", 8, "F7", 9)
	CONNECTION(ATTRACT_n, "F7", 10)	
	
	
	
	/* Paddle 1 */
	CONNECTION(HRESET_n, "C9", 2)
	CONNECTION(STOP1_n, "C9", 4)
	
	CONNECTION("K5", 5, "C8", 6)
	CONNECTION("C9", 3, "C8", 5)
	
	CONNECTION(VCC, "C7", 10)
	CONNECTION("D7", 11, "C7", 7)
	CONNECTION(H1, "C7", 2)
	CONNECTION(GND, "C7", 3)
	CONNECTION(GND, "C7", 4)
	CONNECTION(GND, "C7", 5)
	CONNECTION(GND, "C7", 6)
	CONNECTION(VCC, "C7", 1)
	CONNECTION("C8", 4, "C7", 9)
	
	CONNECTION(ATTRACT_n, "D7", 12)
	CONNECTION("C7", 15, "D7", 13)
	
	CONNECTION("D7", 11, "B8", 4)
	CONNECTION(PADDLE_12, "B8", 3)
	CONNECTION("C8", 4, "B8", 5)
	
	
	
	/* Paddle 2 */
	CONNECTION(HRESET_n, "B9", 2)
	CONNECTION(STOP2_n, "B9", 4)	

	CONNECTION(H256_n, "C8", 8)
	CONNECTION("B9", 3, "C8", 9)

	CONNECTION(VCC, "B7", 10)
	CONNECTION("D7", 8, "B7", 7)
	CONNECTION(H1, "B7", 2)
	CONNECTION(GND, "B7", 3)
	CONNECTION(GND, "B7", 4)
	CONNECTION(GND, "B7", 5)
	CONNECTION(GND, "B7", 6)
	CONNECTION(VCC, "B7", 1)
	CONNECTION("C8", 10, "B7", 9)

	CONNECTION(ATTRACT_n, "D7", 10)
	CONNECTION("B7", 15, "D7", 9)
	
	CONNECTION("D7", 8, "B8", 10)
	CONNECTION(PADDLE_12, "B8", 11)
	CONNECTION("C8", 10, "B8", 9)
	


	/* Ball Direction and Speed Circuit */
	CONNECTION("C7", 11, "B6", 2)
	CONNECTION("B7", 11, "B6", 3)
	CONNECTION("C7", 12, "B6", 5)
	CONNECTION("B7", 12, "B6", 6)
	CONNECTION("C7", 13, "B6", 14)
	CONNECTION("B7", 13, "B6", 13)
	CONNECTION("B8", 6, "B6", 11)
	CONNECTION("B8", 8, "B6", 10)
	CONNECTION(H256, "B6", 1)
	CONNECTION(GND, "B6", 15)
	
	CONNECTION("B6", 9, "B5", 1)

	CONNECTION("B5", 2, "D5", 2)
	CONNECTION(BALL, "D5", 1)
	
	CONNECTION(H1, "E6", 9)
	CONNECTION("D5", 3, "E6", 8)
	
	CONNECTION("E6", 10, "E6", 11)
	CONNECTION(GND, "E6", 12)
	
	CONNECTION("E6", 13, "E5", 13)
	CONNECTION(SERVE_TIME_n, "E5", 12)	
	
	CONNECTION(H256_n, "C5", 5)
	CONNECTION("B6", 4, "C5", 4)
	
	CONNECTION(LSB1, "C5", 9)
	CONNECTION(LSB2, "C5", 10)
	
	CONNECTION(SERVE, "D5", 10)
	CONNECTION("C5", 8, "D5", 9)
	
	CONNECTION("C5", 8, "B5", 5)
	
	CONNECTION(SERVE, "D5", 12)
	CONNECTION("B5", 6, "D5", 13)

	CONNECTION(VCC, "D6", 4)
	CONNECTION("C5", 6, "D6", 2)
	CONNECTION("E6", 10, "D6", 3)
	CONNECTION(SERVE_TIME_n, "D6", 1)
	
	CONNECTION("D5", 8, "C6", 4)
	CONNECTION("B6", 4, "C6", 2)
	CONNECTION("E6", 10, "C6", 3)
	CONNECTION("D5", 11, "C6", 1)
	
	CONNECTION("D5", 11, "D6", 10)
	CONNECTION("B6", 7, "D6", 12)
	CONNECTION("E6", 10, "D6", 11) 
	CONNECTION("D5", 8, "D6", 13)
	
	CONNECTION("D5", 8, "C6", 10) 
	CONNECTION("B6", 12, "C6", 12)
	CONNECTION("E6", 10, "C6", 11)
	CONNECTION("D5", 11, "C6", 13)
	
	CONNECTION("D6", 9, "C5", 1)
	CONNECTION("C6", 5, "C5", 2)
	
	CONNECTION("C6", 9, "C5", 13)
	CONNECTION("C6", 5, "C5", 12)
	
	CONNECTION("C5", 3, "E5", 4)
	CONNECTION(SERVE_n, "E5", 5)
	
	CONNECTION("B4", 15, "B5", 9)
	
	CONNECTION(VCC, "B4", 10)
	CONNECTION(VCC, "B4", 7)
	CONNECTION(V64, "B4", 2)
	CONNECTION(VCC, "B4", 3)
	CONNECTION(VCC, "B4", 4)
	CONNECTION(GND, "B4", 5)
	CONNECTION(GND, "B4", 6)
	CONNECTION("B5", 8, "B4", 9)
	CONNECTION("D5", 3, "B4", 1)
	
	CONNECTION(VCC, "E4", 5)
	CONNECTION("B4", 15, "E4", 4)
	CONNECTION("E5", 11, "E4", 11)
	CONNECTION("C5", 11, "E4", 15)
	CONNECTION("E5", 6, "E4", 1)
	CONNECTION(SERVE_n, "E4", 10)
	CONNECTION(VCC, "E4", 9)
	CONNECTION(GND, "E4", 14)


    /* Score Window Circuit */
	CONNECTION(V64, "C8", 11)
	CONNECTION(V64, "C8", 12)
	
	CONNECTION(V128, "F9", 5)
	
	CONNECTION(V32, "F7", 12)
	CONNECTION("F9", 6, "F7", 13)

	CONNECTION(H256_n, "J5", 9)
	CONNECTION(H64, "J5", 10)
	
	CONNECTION(H256, "J5", 13)
	CONNECTION(H128, "J5", 12)
	
	CONNECTION("C8", 13, "J6", 2)
	CONNECTION("F7", 11, "J6", 1)
	
	CONNECTION("J5", 8, "J6", 5)
	CONNECTION("J5", 11, "J6", 4)
	
	CONNECTION("J6", 3, "K6", 5)
	CONNECTION("J6", 6, "K6", 6)
	
	
	
	/* Score Decoder Circuit */	
	CONNECTION("K6", 4, "K7", 4)
	CONNECTION(VCC, "K7", 3) //Not on schematic
	CONNECTION(VCC, "K7", 5) //Not on schematic
	CONNECTION("K8", 7, "K7", 7)
	CONNECTION("K8", 9, "K7", 1)	
	CONNECTION("J8", 7, "K7", 2)
	CONNECTION("J8", 9, "K7", 6)
	
	CONNECTION(H4, "J6", 13)
	CONNECTION(H8, "J6", 12)
	
	CONNECTION(H16, "K6", 2)
	CONNECTION(GND, "K6", 3)
	
	CONNECTION(V4, "F6", 2)
	CONNECTION(V8, "F6", 1)
	CONNECTION(H16, "F6", 13)
	
	CONNECTION(H8, "H6", 5)
	CONNECTION(H4, "H6", 4)
	CONNECTION("K6", 1, "H6", 3)
	
	CONNECTION(V16, "F9", 3)
	
	CONNECTION("J6", 11, "K6", 11)
	CONNECTION("K6", 1, "K6", 12)
	
	CONNECTION("K6", 1, "H6", 2)
	CONNECTION(V8, "H6", 1)
	CONNECTION(V4, "H6", 13)
	
	CONNECTION("F6", 12, "F9", 9)
	
	CONNECTION("K7", 15, "J7", 2)
	CONNECTION("H6", 6, "J7", 13)
	CONNECTION("F9", 4, "J7", 1)
	
	CONNECTION("K7", 9, "J7", 9)
	CONNECTION("H6", 6, "J7", 10)
	CONNECTION(V16, "J7", 11)
	
	CONNECTION("F9", 4, "H7", 3)
	CONNECTION("K6", 13, "H7", 4)
	CONNECTION("K7", 12, "H7", 5)	
	
	CONNECTION(V16, "J7", 3)
	CONNECTION("K6", 13, "J7", 4)
	CONNECTION("K7", 11, "J7", 5)	
	
	CONNECTION("K7", 13, "F8", 9)
	CONNECTION("F9", 4, "F8", 10)
	CONNECTION("H6", 12, "F8", 11)	

	CONNECTION("F9", 4, "H7", 1)
	CONNECTION("K7", 14, "H7", 2)
	CONNECTION("F9", 8, "H7", 13)
	
	CONNECTION("K7", 10, "H7", 9)
	CONNECTION(V16, "H7", 10)
	CONNECTION("F9", 8, "H7", 11)
	
	CONNECTION("J7", 12, "H8", 4)
	CONNECTION("J7", 8, "H8", 6)
	CONNECTION("H7", 6, "H8", 3)
	CONNECTION("J7", 6, "H8", 5)
	CONNECTION(NET_n, "H8", 1)
	CONNECTION("F8", 8, "H8", 12)
	CONNECTION("H7", 12, "H8", 11)
	CONNECTION("H7", 8, "H8", 2)


	/* Score Counters Circuit */
	CONNECTION(LEFT_SCORE, "K9", 14)
	CONNECTION("K9", 1, "K9", 12)
	CONNECTION(GND, "K9", 6)
	CONNECTION(GND, "K9", 7)
	CONNECTION(START, "K9", 2)
	CONNECTION(START, "K9", 3)
	
	CONNECTION(VCC, "H9", 8)
	CONNECTION("K9", 11,  "H9", 9)
	CONNECTION(VCC, "H9", 11)
	CONNECTION(START_n, "H9", 10)
	
	CONNECTION(RIGHT_SCORE, "J9", 14)
	CONNECTION("J9", 1, "J9", 12)
	CONNECTION(GND, "J9", 6)
	CONNECTION(GND, "J9", 7)
	CONNECTION(START, "J9", 2)
	CONNECTION(START, "J9", 3)	
		
	CONNECTION(VCC, "H9", 1)
	CONNECTION("J9", 11,  "H9", 12)
	CONNECTION(VCC, "H9", 4)
	CONNECTION(START_n, "H9", 13)
	
    CONNECTION(VCC, "DSW1", 1)
    CONNECTION("K9", 8, "DSW1", 2)

	CONNECTION("H9", 5, "F8", 4)
	CONNECTION("K9", 12, "F8", 5)
	CONNECTION("DSW1", 3, "F8", 3)

    CONNECTION(VCC, "DSW1", 4)
    CONNECTION("J9", 8, "DSW1", 5)

	CONNECTION("H9", 3, "F8", 2)
	CONNECTION("J9", 12, "F8", 1)
	CONNECTION("DSW1", 6, "F8", 13)	
	
	CONNECTION("F8", 6, "J6", 10)
	CONNECTION("F8", 12, "J6", 9)
	
	CONNECTION(H32, "K8", 14)
	CONNECTION(H64, "K8", 2)
	CONNECTION("K9", 12, "K8", 5)
	CONNECTION("K9", 9, "K8", 11)
	CONNECTION(VCC, "K8", 6)
	CONNECTION(VCC, "K8", 4)
	CONNECTION("J9", 12, "K8", 3)
	CONNECTION("J9", 9, "K8", 13)
	CONNECTION("H9", 2, "K8", 12)
	CONNECTION("H9", 6, "K8", 10)
	CONNECTION(GND, "K8", 1)
	CONNECTION(GND, "K8", 15)

	CONNECTION(H32, "J8", 14)
	CONNECTION(H64, "J8", 2)
	CONNECTION("K9", 8, "J8", 5)
	CONNECTION("K9", 11, "J8", 11)
	CONNECTION("H9", 6, "J8", 6)
	CONNECTION("H9", 2, "J8", 4)
	CONNECTION("J9", 8, "J8", 3)
	CONNECTION("J9", 11, "J8", 13)
	CONNECTION("H9", 2, "J8", 12)
	CONNECTION("H9", 6, "J8", 10)
	CONNECTION(GND, "J8", 1)
	CONNECTION(GND, "J8", 15)	



	/* Serve Circuit */
	CONNECTION(H128, "C3", 10)
	CONNECTION(H64, "C3", 11)
	CONNECTION(H256_n, "C3", 9)
	
	CONNECTION(V64_n, "H6", 10)
	CONNECTION(V128, "H6", 9)
	CONNECTION("C3", 8, "H6", 11)
	
	CONNECTION(START, "K1", 4)
	CONNECTION("D1", 7, "K1", 5)
	CONNECTION(VCC, "K1", 3)
	
	CONNECTION(VCC, "J1", 10)
	CONNECTION("K1", 7, "J1", 12)
	CONNECTION("H6", 8, "J1", 11)
	CONNECTION("K1", 7, "J1", 13)
	
	
	
	/* Out-of-Bounds Circuit */
	CONNECTION(H256, "B3", 1)
	CONNECTION(BALL, "B3", 12)
	CONNECTION(H256_n, "B3", 4)
	CONNECTION(SERVE_n, "B3", 13)
	
	CONNECTION(SERVE_n, "J2", 4)
	CONNECTION("B3", 3, "J2", 2)
	CONNECTION(H2, "J2", 3)
	CONNECTION(VCC, "J2", 1)
	
	CONNECTION("B3", 3, "J5", 2)
	CONNECTION("J2", 5, "J5", 1)
	
	CONNECTION(VCC, "D1", 11)
	CONNECTION("F1", 8, "D1", 12)
	CONNECTION(VCC, "D1", 13)
		
	CONNECTION("J5", 3, "C1", 2)
	CONNECTION("J5", 3, "C1", 3)
	CONNECTION("D1", 9, "C1", 14)
	CONNECTION("C1", 12, "C1", 1)
	
	CONNECTION(HBLANK_n, "B1", 9)
	CONNECTION(NET_HEIGHT_n, "B1", 10)
	
	CONNECTION("J5", 3, "E5", 1)
	CONNECTION("B1", 8, "E5", 2)
	
	CONNECTION("C1", 8, "D2", 8)
	CONNECTION("E5", 3, "D2", 9)
	
	CONNECTION("D2", 10, "D2", 11)
	CONNECTION(SERVE, "D2", 12)
	
	CONNECTION(BALL_n, "D2", 5)
	CONNECTION(V256_n, "D2", 6)
	
	CONNECTION("D2", 4, "D2", 3)
	CONNECTION("D2", 13, "D2", 2)
	
	CONNECTION(VCC, "B2", 4)
	CONNECTION(H256_n, "B2", 2)
	CONNECTION(HIT, "B2", 3)
	CONNECTION(VCC, "B2", 1)
	
	CONNECTION("D2", 4, "C2", 1)
	CONNECTION(H256_n, "C2", 13)
	CONNECTION("D2", 13, "C2", 10)
	CONNECTION("B2", 5, "C2", 9)
	
	CONNECTION("D2", 4, "C2", 2)
	CONNECTION(H256, "C2", 3)
	CONNECTION("D2", 13, "C2", 4)
	CONNECTION("B2", 6, "C2", 5)
	
	CONNECTION("C2", 8, "A1", 13)
	CONNECTION(ATTRACT_n, "A1", 12)
	
	CONNECTION(ATTRACT_n, "A1", 2)
	CONNECTION("C2", 6, "A1", 1)
	
	
	
	/* Horizontal Ball-Motion Circuit */
	CONNECTION(VCC, "F1", 4)
	CONNECTION("F1", 6, "F1", 2)
	CONNECTION(VRESET, "F1", 3)
	CONNECTION(BACKWARDS_n, "F1", 1)
	
	CONNECTION("F1", 6, "F2", 5)
	CONNECTION(VRESET, "F2", 4)
	
	CONNECTION(BP_n, "F2", 1)
	CONNECTION("F2", 6, "F2", 2)
	
	CONNECTION(CP_n, "F2", 13)
	CONNECTION("F2", 6, "F2", 12)
	
	CONNECTION(DP, "H1", 9)
	CONNECTION("F2", 6, "H1", 10)
	
	CONNECTION("H1", 8, "E1", 12)
	CONNECTION("H1", 8, "E1", 13)
	
	CONNECTION("F2", 3, "E2", 10)
	CONNECTION("F2", 11, "E2", 8)
	CONNECTION("H1", 8, "E2", 3)
	CONNECTION(GND, "E2", 1)
	CONNECTION("E1", 11, "E2", 11)
	CONNECTION(VCC, "E2", 7)
	CONNECTION(VCC, "E2", 4)
	CONNECTION(GND, "E2", 16)
	CONNECTION(GND, "E2", 13)
	
	CONNECTION("E2", 15, "E3", 6)
	CONNECTION("E2", 2, "E3", 5)
	CONNECTION("E2", 6, "E3", 4)
	CONNECTION("E2", 9, "E3", 3)
	CONNECTION("C3", 6, "E3", 9)
	CONNECTION(SERVE_n, "E3", 1)
	CONNECTION(VCC, "E3", 10)
	CONNECTION(VCC, "E3", 7)
	CONNECTION(CLK, "E3", 2)
	
	CONNECTION(GND, "D3", 6)
	CONNECTION(GND, "D3", 5)
	CONNECTION(VCC, "D3", 4)
	CONNECTION(VCC, "D3", 3)
	CONNECTION("C3", 6, "D3", 9)
	CONNECTION(SERVE_n, "D3", 1)
	CONNECTION(VCC, "D3", 10)
	CONNECTION("E3", 15, "D3", 7)
	CONNECTION(CLK, "D3", 2)
	
	CONNECTION(VCC, "F3", 1)
	CONNECTION("D3", 15, "F3", 12)
	CONNECTION(VCC, "F3", 4)
	CONNECTION(SERVE_n, "F3", 13)
	
	CONNECTION("E3", 15, "C3", 5)
	CONNECTION("D3", 15, "C3", 3)
	CONNECTION("F3", 3, "C3", 4)
	
	CONNECTION("F3", 3, "F4", 9)
	CONNECTION("D3", 15, "F4", 10)
	CONNECTION("E3", 11, "F4", 12)
	CONNECTION("E3", 12, "F4", 13)
	
	
	
	/* Vertical Ball-Motion Circuit */
	CONNECTION("E4", 7, "D4", 6)
	CONNECTION("E4", 6, "D4", 5)
	CONNECTION("E4", 2, "D4", 4)
	CONNECTION("E4", 3, "D4", 3)
	CONNECTION("D5", 6, "D4", 9)
	CONNECTION(SERVE_n, "D4", 1)
	CONNECTION(VSYNC_n, "D4", 10)
	CONNECTION(VCC, "D4", 7)
	CONNECTION(HSYNC_n, "D4", 2)
	
	CONNECTION(GND, "C4", 6)
	CONNECTION(GND, "C4", 5)
	CONNECTION(GND, "C4", 4)
	CONNECTION(GND, "C4", 3)
	CONNECTION("D5", 6, "C4", 9)
	CONNECTION(SERVE_n, "C4", 1)
	CONNECTION(VCC, "C4", 10)
	CONNECTION("D4", 15, "C4", 7)
	CONNECTION(HSYNC_n, "C4", 2)
	
	CONNECTION("D4", 15, "D5", 5)
	CONNECTION("C4", 15, "D5", 4)
	
	CONNECTION("C4", 15, "C3", 13)
	CONNECTION("D4", 11, "C3", 2)
	CONNECTION("D4", 12, "C3", 1)
	
	CONNECTION("F4", 8, "E6", 5)
	CONNECTION("C3", 12, "E6", 6)
	
	CONNECTION("E6", 4, "E6", 3)
	CONNECTION("E6", 4, "E6", 2)
	
	
		
	/* Sound Circuits */
	CONNECTION("D2", 1, "D1", 5)
	CONNECTION(GND, "D1", 4)
	CONNECTION(VCC, "D1", 3)
	
	CONNECTION(V32, "E1", 10)
	CONNECTION("D1", 6, "E1", 9)
	
	CONNECTION("J1", 9, "F1", 10)
	CONNECTION(HIT_n, "F1", 12)
	CONNECTION("C4", 15, "F1", 11)
	CONNECTION(HIT_n, "F1", 13)
	
	CONNECTION("F1", 8, "E1", 2)
	CONNECTION("C4", 14, "E1", 1)
	
	CONNECTION(ATTRACT_n, "F2", 10)
	CONNECTION("E1", 8, "F2", 9)
	
	CONNECTION("F2", 8, "E1", 5)
	CONNECTION("E1", 3, "E1", 4)



	/* Coin / Start Circuit */
	// Not completely accurate
	CONNECTION("START", 1, "E9", 13)
	CONNECTION("COIN1", 1, "E9", 11)
	CONNECTION("LATCH", 3, "E9", 3)
	
    CONNECTION(GND, "DSW2", 1)
    CONNECTION(VCC, "DSW2", 2)

	CONNECTION(VCC, "D8", 2)
	CONNECTION("DSW2", 3, "D8", 4)
	CONNECTION("COIN1", 1, "D8", 1)
	CONNECTION(ATTRACT, "D8", 3)

	CONNECTION("D8", 5, "D8", 12)
	CONNECTION("E9", 4, "D8", 10)
	CONNECTION("COIN1", 1, "D8", 13)
	CONNECTION(ATTRACT, "D8", 11)
	
	CONNECTION("D8", 9, "C8", 3)
	CONNECTION("E9", 10, "C8", 2)

	CONNECTION("E9", 12, "A1", 5)
	CONNECTION("C8", 1, "A1", 4)

	CONNECTION(VCC, "E7", 4)
	CONNECTION("E8", 5, "E7", 2)
	CONNECTION("A1", 6, "E7", 3)
	CONNECTION("E7", 8, "E7", 1)

	CONNECTION(VCC, "E7", 10)
	CONNECTION("E7", 5, "E7", 12)
	CONNECTION(V256, "E7", 11)
	CONNECTION("C8", 1, "E7", 13)
	
	CONNECTION("E7", 9, "D7", 5)
	CONNECTION("D8", 8, "D7", 4)
	
	CONNECTION("E9", 4, "E8", 4)
	CONNECTION(VCC, "E8", 2)
	CONNECTION(STOP, "E8", 3)
	CONNECTION("D7", 6, "E8", 1)
	
	CONNECTION("D8", 9, "D7", 2)
	CONNECTION(ATTRACT, "D7", 1)
	
	CONNECTION("LATCH", 1, "D7", 3)
	CONNECTION("LATCH", 2, "COIN1", 1)




    /* Video Summing */
    CONNECTION(V4, "E5", 9)
	CONNECTION("B5", 2, "E5", 10)
    
    CONNECTION("VIDEO", 1, "E5", 8)
    CONNECTION("VIDEO", 2, "H8", 8)
    CONNECTION("VIDEO", 3, BALL)

    CONNECTION("VIDEO", Video::HBLANK_PIN, HBLANK)
    CONNECTION("VIDEO", Video::VBLANK_PIN, "H1", 3)

    // Audio Summing
    CONNECTION("AUDIO", 1, "E1", 6)
    CONNECTION("AUDIO", i1, "AUDIO", Audio::OUTPUT_MONO)


#ifdef DEBUG
    CONNECTION("LOG1", 1, CLK)
    CONNECTION("LOG1", 2, "H5", 12)
    CONNECTION("LOG1", 3, "H5", 9)
    CONNECTION("LOG1", 4, "H5", 8)
    CONNECTION("LOG1", 5, "H5", 11)
    CONNECTION("LOG1", 6, "F5", 12)
    CONNECTION("LOG1", 7, "F5", 9)
    CONNECTION("LOG1", 8, "F5", 8)
    CONNECTION("LOG1", 9, "F5", 11)
    CONNECTION("LOG1", 10, "H4", 3)
    CONNECTION("LOG1", 11, CLK_n)
    CONNECTION("LOG1", 12, "F4", 6)
    CONNECTION("LOG1", 13, "J4", 9)
#endif


CIRCUIT_LAYOUT_END

