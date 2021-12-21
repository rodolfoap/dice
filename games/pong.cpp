#include "../circuit_desc.h"

#define DEBUG
#undef DEBUG

#ifdef DEBUG
static VcdLogDesc vcd_log_desc
(
    "output.vcd",
    1, "CLK",
    2, "/HBLANK",
    3, "H1",
    4, "H2",
    5, "H4",
    6, "H8",
    7, "RCO",
    8, "/LD",
    9, "H16",
    10, "H32",
    11, "H64",
    12, "H128",
    13, "RCO2",
    14, "H256"    
);
#endif

static Mono555Desc a9_555_desc(K_OHM(70.0), U_FARAD(0.1));
static Mono555Desc b9_555_desc(K_OHM(70.0), U_FARAD(0.1));

static Mono555Desc f4_555_desc(K_OHM(330.0), U_FARAD(4.7));
static Mono555Desc g4_555_desc(K_OHM(220.0), U_FARAD(1.0));

static Paddle1VerticalDesc pad1_desc(17000.0, 145000.0, &b9_555_desc);
static Paddle2VerticalDesc pad2_desc(17000.0, 145000.0, &a9_555_desc);

static DipswitchDesc dipswitch1_desc("winning_score", "Winning Score", 0, "11", "15");

static INPUT_DESC( pong )
    INPUT_INFO(PADDLE1_VERTICAL_INPUT, "Move Left Paddle")
    INPUT_INFO(PADDLE2_VERTICAL_INPUT, "Move Right Paddle")
    INPUT_INFO(COIN_INPUT, {{ 1 }}, "Insert Coin and Start Game")
INPUT_DESC_END

static VIDEO_DESC( pong )
    VIDEO_RESISTANCE(1, K_OHM(1.0))
    VIDEO_RESISTANCE(2, K_OHM(1.2))
    VIDEO_RESISTANCE(Video::HBLANK_PIN, K_OHM(22.0))
    VIDEO_CONTRAST(2.0)
VIDEO_DESC_END

CIRCUIT_LAYOUT( pong ) 
	CHIP("CLOCK", CLOCK_14_318_MHZ)
    
    CHIP("F8", 7493)
	CHIP("F9", 7493)
	CHIP("F6", 74107)
	CHIP("F7", 7430)
	CHIP("E7", 7474)

	CHIP("E8", 7493)
	CHIP("E9", 7493)
    CHIP("D9", 74107)
    CHIP("D8", 7410)

    CHIP("G5", 7410)
    CHIP("H5", 7400)
    CHIP("F5", 7402)

    CHIP("F3", 74107)
    CHIP("G3", 7400)
    CHIP("G2", 7427)

    CHIP("E3", 7427)
	CHIP("E2", 7410)
	CHIP("D2", 7402)
	CHIP("G1", 7402)
	CHIP("F2", 7425)
   	CHIP("C5", 7448)
	CHIP("C6", 74153)
	CHIP("D6", 74153)

	CHIP("C3", 7400)
	CHIP("E4", 7404)
	CHIP("E5", 7427)
	CHIP("C4", 7410)
	CHIP("D4", 7410)
	CHIP("D5", 7410)
	CHIP("D3", 7430)

    CHIP("B9", 555_Mono, &b9_555_desc)   
	CHIP("C9", 7404)
	CHIP("B8", 7493)
	CHIP("B7", 7400)
	CHIP("A7", 7420)
    
    CHIP("A9", 555_Mono, &a9_555_desc)
	CHIP("A8", 7493)

	CHIP("H3", 7474)
	
	CHIP("B2", 7400)
	CHIP("D1", 7404)

	CHIP("G7", 9316)
	CHIP("H7", 9316)
	CHIP("G6", 74107)
	CHIP("H6", 7420)
	CHIP("E1", 7400)
	
	CHIP("B3", 9316)
	CHIP("A3", 9316)	
	CHIP("B4", 7483)

	CHIP("B5", 7474)
	CHIP("H4", 7400)
	CHIP("E6", 7400)
    CHIP("F4", 555_Mono, &f4_555_desc)
    
    CHIP("G4", 555_Mono, &g4_555_desc)
	CHIP("C2", 7474)
	CHIP("C1", 7400)

    CHIP("F1", 7493)
	CHIP("H1", 7400)
	CHIP("H2", 74107)
	
	CHIP("A6", 7450)
	CHIP("B6", 7450)
	CHIP("A5", 7474)
	CHIP("A4", 7486)
	CHIP("A2", 74107)
	
	CHIP("C7", 7490)
	CHIP("D7", 7490)
    CHIP("C8", 74107)

    CHIP("PAD1", PADDLE1_VERTICAL_INPUT, &pad1_desc)
    PADDLE_CONNECTION("PAD1", "B9")

    CHIP("PAD2", PADDLE2_VERTICAL_INPUT, &pad2_desc)
    PADDLE_CONNECTION("PAD2", "A9")

    CHIP("LATCH", LATCH)
    CHIP("COIN1", COIN_INPUT)
    CHIP("DIPSW1", DIPSWITCH, &dipswitch1_desc)

    VIDEO(pong)
    INPUT(pong)

#ifdef DEBUG
	CHIP("LOG1", VCD_LOG, &vcd_log_desc)
#endif

    // HRESET Circuit
    CONNECTION("CLOCK", 1, "F6", 12)
    CONNECTION(VCC, "F6", 1)
    CONNECTION(VCC, "F6", 4)
    CONNECTION(VCC, "F6", 13)

    /*NET("TEST1", GND)

    CONNECTION("CLOCK", 1, "F6", 12)
    NET("VCC1", VCC)
    NET("VCC1", "F6", 1)
    NET("VCC1", "F6", 4)
    NET("VCC1", "F6", 13)*/

    #define CLK "F6", 3

	CONNECTION("F8", 12, "F8", 1)
	CONNECTION(CLK, "F8", 14)
	CONNECTION("E7", 8, "F8", 2)
	CONNECTION("E7", 8, "F8", 3)

	CONNECTION("F9", 12, "F9", 1)
	CONNECTION("F8", 11, "F9", 14)
	CONNECTION("E7", 8, "F9", 2)
	CONNECTION("E7", 8, "F9", 3)

	CONNECTION(VCC, "F6", 8)
	CONNECTION(VCC, "F6", 11)
	CONNECTION("F9", 11, "F6", 9)
	CONNECTION("E7", 9, "F6", 10)

	CONNECTION("F6", 5, "F7", 1)
	CONNECTION(VCC, "F7", 2)
	CONNECTION(VCC, "F7", 3)
	CONNECTION(VCC, "F7", 4)
	CONNECTION("F8", 8, "F7", 5)
	CONNECTION("F8", 9, "F7", 6)
	CONNECTION("F9", 8, "F7", 11)
	CONNECTION("F9", 11, "F7", 12)

	CONNECTION(CLK, "E7", 11)
	CONNECTION("F7", 8, "E7", 12)
	CONNECTION(VCC, "E7", 13)
	CONNECTION(VCC, "E7", 10)

    #define H1 "F8", 12
    #define H2 "F8", 9
    #define H4 "F8", 8
    #define H8 "F8", 11
    #define H16 "F9", 12
    #define H32 "F9", 9
    #define H64 "F9", 8
    #define H128 "F9", 11
    #define H256 "F6", 5
    #define H256_n "F6", 6
    #define HRESET "E7", 8
    #define HRESET_n "E7", 9


    // VRESET Circuit
	CONNECTION("E8", 12, "E8", 1)
    CONNECTION(HRESET, "E8", 14)
	CONNECTION("E7", 6, "E8", 2)
	CONNECTION("E7", 6, "E8", 3)

	CONNECTION("E9", 12, "E9", 1)
	CONNECTION("E8", 11, "E9", 14)
	CONNECTION("E7", 6, "E9", 2)
	CONNECTION("E7", 6, "E9", 3)

	CONNECTION(VCC, "D9", 8)
	CONNECTION(VCC, "D9", 11)
	CONNECTION("E9", 11, "D9", 9)
	CONNECTION("E7", 5, "D9", 10)

	CONNECTION("D9", 5, "D8", 9)
	CONNECTION("E8", 12, "D8", 10)
	CONNECTION("E8", 8, "D8", 11)

	CONNECTION(HRESET, "E7", 3)
	CONNECTION("D8", 8, "E7", 2)
	CONNECTION(VCC, "E7", 1)
	CONNECTION(VCC, "E7", 4)

    #define V1 "E8", 12
    #define V2 "E8", 9
    #define V4 "E8", 8
    #define V8 "E8", 11
    #define V16 "E9", 12
    #define V32 "E9", 9
    #define V64 "E9", 8
    #define V128 "E9", 11
    #define V256 "D9", 5
    #define V256_n "D9", 6
    #define VRESET "E7", 6
    #define VRESET_n "E7", 5


    // HSync Logic
    CONNECTION(H16, "G5", 3)
    CONNECTION(H64, "G5", 4)
    CONNECTION(H64, "G5", 5)

    CONNECTION("G5", 6, "H5", 4)
    CONNECTION(HRESET_n, "H5", 10)
    CONNECTION("H5", 6, "H5", 9)
    CONNECTION("H5", 8, "H5", 5)

    CONNECTION("H5", 8, "H5", 12)
    CONNECTION(H32, "H5", 13)

    #define HBLANK "H5", 8
    #define HBLANK_n "H5", 6

    // VSync Logic
    CONNECTION(VRESET, "F5", 8)
    CONNECTION(V16, "F5", 12)
    CONNECTION("F5", 10, "F5", 11)
    CONNECTION("F5", 13, "F5", 9)

    CONNECTION(V8, "H5", 1)
    CONNECTION(V8, "H5", 2)

    CONNECTION("F5", 13, "G5", 1)
    CONNECTION(V4, "G5", 2)
    CONNECTION("H5", 3, "G5", 13) 
    
    #define VBLANK "F5", 13
    #define VBLANK_n "F5", 10


    // Net circuit
    CONNECTION(H256, "F3", 8)
    CONNECTION(H256_n, "F3", 11)
    CONNECTION(CLK, "F3", 9)
    CONNECTION(VCC, "F3", 10)
    
    CONNECTION("F3", 6, "G3", 5)
    CONNECTION(H256, "G3", 4)

    CONNECTION(V4, "G2", 3)
    CONNECTION(VBLANK, "G2", 4)
    CONNECTION("G3", 6, "G2", 5)

    #define NET "G2", 6


    // Score decoding circuit
	CONNECTION("C8", 2, "D6", 6)
	CONNECTION("C7", 8, "D6", 5)
	CONNECTION("C8", 6, "D6", 4)
	CONNECTION("D7", 8, "D6", 3)
	CONNECTION("C8", 2, "D6", 10)
	CONNECTION("C7", 11, "D6", 11)
	CONNECTION("C8", 6, "D6", 12)
	CONNECTION("D7", 11, "D6", 13)
	CONNECTION(H32, "D6", 14)
	CONNECTION(H64, "D6", 2)
	CONNECTION(GND, "D6", 1)
	CONNECTION(GND, "D6", 15)

	CONNECTION(VCC, "C6", 6)
	CONNECTION("C7", 12, "C6", 5)
	CONNECTION(VCC, "C6", 4)
	CONNECTION("D7", 12, "C6", 3)
	CONNECTION("C8", 2, "C6", 10)
	CONNECTION("C7", 9, "C6", 11)
	CONNECTION("C8", 6, "C6", 12)
	CONNECTION("D7", 9, "C6", 13)
	CONNECTION(H32, "C6", 14)
	CONNECTION(H64, "C6", 2)
	CONNECTION(GND, "C6", 1)
	CONNECTION(GND, "C6", 15)

    CONNECTION(H128, "E3", 1)
	CONNECTION(H128, "E3", 2)
	CONNECTION(H128, "E3", 13)
	
	CONNECTION("E3", 12, "E3", 5)
	CONNECTION(H64, "E3", 4)
	CONNECTION(H256, "E3", 3)

	CONNECTION("E3", 12, "E2", 9)
	CONNECTION(H64, "E2", 10)
	CONNECTION(H256, "E2", 11)
	
	CONNECTION("E2", 8, "E3", 9)
	CONNECTION("E2", 8, "E3", 10)
	CONNECTION("E2", 8, "E3", 11)
	
	CONNECTION("E3", 6, "D2", 8)
	CONNECTION("E3", 8, "D2", 9)
	
	CONNECTION(V32, "G1", 2)
	CONNECTION(V32, "G1", 3)
	
	CONNECTION("G1", 1, "F2", 1)
	CONNECTION(V64, "F2", 2)
	CONNECTION(V128, "F2", 4)
	CONNECTION("D2", 10, "F2", 5)
	
	CONNECTION("C6", 7, "C5", 7)
	CONNECTION("C6", 9, "C5", 1)
	CONNECTION("D6", 7, "C5", 2)
	CONNECTION("D6", 9, "C5", 6)
	CONNECTION("F2", 6, "C5", 4)
	CONNECTION(VCC, "C5", 3)
	CONNECTION(VCC, "C5", 5)


	// Score display circuit 
	CONNECTION(H16, "E4", 3)
	
	CONNECTION(H4, "C3", 12)
	CONNECTION(H8, "C3", 13)
	
	CONNECTION("E4", 4, "E5", 9)
	CONNECTION(H4, "E5", 10)
	CONNECTION(H8, "E5", 11)
	
	CONNECTION("C3", 11, "D2", 5)
	CONNECTION("E4", 4, "D2", 6)
	
	CONNECTION(V8, "E5", 3)
	CONNECTION(V4, "E5", 4)
	CONNECTION("E4", 4, "E5", 5)
	
	CONNECTION(V4, "E2", 1)	
	CONNECTION(V8, "E2", 2)
	CONNECTION(H16, "E2", 13)
	
	CONNECTION("E4", 1, "E2", 12)
	
	CONNECTION(V16, "E4", 5)
	
	CONNECTION("E4", 6, "D4", 1)	
	CONNECTION("C5", 15, "D4", 2)
	CONNECTION("E5", 8, "D4", 13)	

	CONNECTION("C5", 9, "D5", 9)
	CONNECTION(V16, "D5", 10)
	CONNECTION("E5", 8, "D5", 11)	

	CONNECTION("D2", 4, "C4", 9)
	CONNECTION("E4", 6, "C4", 10)
	CONNECTION("C5", 12, "C4", 11)	
	
	CONNECTION("D2", 4, "D5", 1)	
	CONNECTION("C5", 11, "D5", 2)
	CONNECTION(V16, "D5", 13)		
	
	CONNECTION("C5", 13, "D4", 9)
	CONNECTION("E4", 6, "D4", 10)
	CONNECTION("E5", 6, "D4", 11)	
	
	CONNECTION("C5", 14, "D4", 3)
	CONNECTION("E4", 2, "D4", 4)
	CONNECTION("E4", 6, "D4", 5)		
	
	CONNECTION("E4", 2, "D5", 3)
	CONNECTION(V16, "D5", 4)
	CONNECTION("C5", 10, "D5", 5)
	
	CONNECTION("D4", 12, "D3", 12)
	CONNECTION("D5", 8, "D3", 5)
	CONNECTION("C4", 8, "D3", 4)
	CONNECTION(VCC, "D3", 1)
	CONNECTION("D5", 12, "D3", 2)
	CONNECTION("D4", 8, "D3", 11)
	CONNECTION("D4", 6, "D3", 6)
	CONNECTION("D5", 6, "D3", 3)
	

    // Coin Logic - This is somewhat different on the actual board
	CONNECTION("D8", 12, "B2", 1)
	CONNECTION("D8", 6, "B2", 2)
	
	CONNECTION("B2", 3, "E4", 9)
	
    #define SRST_n "COIN1", 1

    CONNECTION(SRST_n, "C9", 13)
	#define SRST "C9", 12    
	
    CONNECTION("LATCH", 2, SRST_n)
	CONNECTION("LATCH", 1, "E4", 8)

    #define RUN_n "LATCH", 3

	CONNECTION("B2", 3, "D2", 2)
	CONNECTION(RUN_n, "D2", 3)
	
	CONNECTION("D2", 1, "D1", 3)
	
    #define ATTRACT_n "D2", 1
    #define ATTRACT   "D1", 4

    // Game Control Logic			
	CONNECTION("H6", 6, "E6", 9)
	CONNECTION("H5", 8, "E6", 10)
	
	CONNECTION("E6", 8, "D1", 13)
	
	CONNECTION("D1", 12, "E1", 1)
	CONNECTION(ATTRACT_n, "E1", 2)
	
	CONNECTION(SRST_n, "E6", 4)
    CONNECTION("E6", 8, "E6", 5)
	
	CONNECTION("E6", 6, "E6", 1)
	CONNECTION("E6", 6, "E6", 2)
	
	CONNECTION("E6", 3, "F4", 2)
	CONNECTION(VCC, "F4", 4)
			
	CONNECTION(RUN_n, "E5", 1)
	CONNECTION("B2", 3, "E5", 2)
	CONNECTION("F4", 3, "E5", 13)
	
	CONNECTION("E5", 12, "B5", 12)
	CONNECTION("E5", 12, "B5", 13)
	CONNECTION(VCC, "B5", 10)		
	CONNECTION("G2", 8, "B5", 11)


    // Horizontal Ball Counter
	CONNECTION(ATTRACT_n, "E1", 4)
    CONNECTION("B5", 8, "E1", 5)
	
	CONNECTION("H4", 8, "G7", 3)
	CONNECTION("H4", 6, "G7", 4)
	CONNECTION(GND, "G7", 5)
	CONNECTION(VCC, "G7", 6)
	CONNECTION(CLK, "G7", 2)
	CONNECTION("E1", 6, "G7", 1)
	CONNECTION(VCC, "G7", 7)
	CONNECTION("H5", 6, "G7", 10)
	CONNECTION("G5", 8, "G7", 9)
	
	CONNECTION(GND, "H7", 3)
	CONNECTION(GND, "H7", 4)
	CONNECTION(GND, "H7", 5)
	CONNECTION(VCC, "H7", 6)
	CONNECTION(CLK, "H7", 2)
	CONNECTION("E1", 6, "H7", 1)
	CONNECTION("G7", 15, "H7", 7)
	CONNECTION(VCC, "H7", 10)
	CONNECTION("G5", 8, "H7", 9)
	
	CONNECTION(VCC, "G6", 8)
	CONNECTION(VCC, "G6", 11)
	CONNECTION("H7", 15, "G6", 9)
	CONNECTION("E1", 6, "G6", 10)
	
	CONNECTION("G7", 12, "H6", 9)
	CONNECTION("G7", 11, "H6", 10)
	CONNECTION("H7", 15, "H6", 12)
	CONNECTION("G6", 5, "H6", 13)

    #define HVID_n "H6", 8
	
	CONNECTION("H7", 15, "G5", 9)
	CONNECTION("G7", 15, "G5", 10)
	CONNECTION("G6", 5, "G5", 11)

	CONNECTION(HVID_n, "H6", 1)	
	CONNECTION(HVID_n, "H6", 2)
	CONNECTION(HVID_n, "H6", 4)
	CONNECTION(HVID_n, "H6", 5)
	
	#define HVID "H6", 6


	// Vertical Ball Counter
	CONNECTION("B4", 9, "B3", 3)
	CONNECTION("B4", 6, "B3", 4)
	CONNECTION("B4", 2, "B3", 5)
	CONNECTION("B4", 15, "B3", 6)
	CONNECTION("H5", 11, "B3", 2)
	CONNECTION(VCC, "B3", 1)
	CONNECTION(VCC, "B3", 7)
	CONNECTION("F5", 10, "B3", 10)
	CONNECTION("B2", 6, "B3", 9)	
	
	CONNECTION(GND, "A3", 3)
	CONNECTION(GND, "A3", 4)
	CONNECTION(GND, "A3", 5)
	CONNECTION(GND, "A3", 6)
	CONNECTION("H5", 11, "A3", 2)
	CONNECTION(VCC, "A3", 1)
	CONNECTION("B3", 15, "A3", 7)
	CONNECTION(VCC, "A3", 10)
	CONNECTION("B2", 6, "A3", 9)		
	
	CONNECTION("B3", 15, "B2", 5)
	CONNECTION("A3", 15, "B2", 4)
	
	CONNECTION("B3", 12, "E2", 3)
	CONNECTION("B3", 11, "E2", 4)
	CONNECTION("A3", 15, "E2", 5)
	
	CONNECTION("E2", 6, "D2", 11)
	CONNECTION("E2", 6, "D2", 12)

    #define VVID_n "E2", 6
    #define VVID   "D2", 13


	// Hit Logic
	CONNECTION(HVID_n, "G1", 5)
	CONNECTION(VVID_n, "G1", 6)
	
	CONNECTION("G2", 12, "G3", 1)
	CONNECTION("G1", 4, "G3", 2)

	CONNECTION("G2", 8, "G3", 13)
	CONNECTION("G1", 4, "G3", 12)	
	
	CONNECTION("G3", 3, "B2", 9)
	CONNECTION("G3", 11, "B2", 10)
	
	CONNECTION("B2", 8, "B2", 12)
	CONNECTION("B2", 8, "B2", 13)
	
    #define HIT   "B2", 8
    #define HIT_n "B2", 11


    // Horizontal Ball Control
	CONNECTION("E1", 8, "E1", 12)
	CONNECTION("C2", 6, "E1", 13)
	
	CONNECTION("E6", 6, "F1", 2)
	CONNECTION("E6", 6, "F1", 3)
	CONNECTION("E1", 11, "F1", 14)
	CONNECTION("F1", 12, "F1", 1)
	
	CONNECTION("F1", 8, "G1", 11)	
	CONNECTION("F1", 11, "G1", 12)
	
	CONNECTION("F1", 8, "E1", 9)	
	CONNECTION("F1", 11, "E1", 10)
	
	CONNECTION("G1", 13, "H1", 1)
	CONNECTION("G1", 13, "H1", 2)
	
	CONNECTION("H1", 3, "H1", 12)
	CONNECTION("E1", 8, "H1", 13)
	
	CONNECTION("H1", 11, "H1", 9)
	CONNECTION("E7", 6, "H1", 10)
	
	CONNECTION("H1", 3, "H1", 4)
	CONNECTION("E7", 6, "H1", 5)

	CONNECTION(H256_n, "G1", 8)
	CONNECTION("E7", 6, "G1", 9)
	
	CONNECTION(VCC, "H2", 8)
	CONNECTION("G1", 10, "H2", 9)
	CONNECTION("H4", 3, "H2", 11)
	CONNECTION("H1", 8, "H2", 10)

	CONNECTION("H2", 5, "H2", 1)
	CONNECTION("G1", 10, "H2", 12)
	CONNECTION(GND, "H2", 4)
	CONNECTION("H1", 6, "H2", 13)
	
	CONNECTION("H2", 5, "H4", 1)
	CONNECTION("H2", 3, "H4", 2)

	CONNECTION("G4", 3, "C1", 12)
	CONNECTION(ATTRACT, "C1", 13)
    
	CONNECTION("C1", 11, "D1", 1)

	CONNECTION("H3", 8, "H3", 12)
	CONNECTION("D1", 2, "H3", 11)
	CONNECTION("G3", 11, "H3", 13)
	CONNECTION("G3", 3, "H3", 10)

    CONNECTION("H4", 3, "H4", 12)
	CONNECTION("H3", 9, "H4", 13)
	
	CONNECTION("H4", 3, "H4", 4)
	CONNECTION("H3", 8, "H4", 5)
	
	CONNECTION("H4", 11, "H4", 10)
	CONNECTION("H4", 6, "H4", 9)



	// Vertical Ball Control
	CONNECTION("B8", 9, "A6", 1)	//Wrong pin numbers on schematics?
	CONNECTION(H256_n, "A6", 13)
	CONNECTION("A8", 9, "A6", 10)
	CONNECTION(H256, "A6", 9)
	
	CONNECTION("B8", 8, "A6", 3)
	CONNECTION(H256_n, "A6", 2)
	CONNECTION("A8", 8, "A6", 4)
	CONNECTION(H256, "A6", 5)

	CONNECTION("B8", 11, "B6", 3)
	CONNECTION(H256_n, "B6", 2)
	CONNECTION("A8", 11, "B6", 4)
	CONNECTION(H256, "B6", 5)
	
	CONNECTION(ATTRACT, "D1", 5)
    
	CONNECTION("A6", 8, "A5", 12)
	CONNECTION(HIT, "A5", 11)
	CONNECTION("D1", 6, "A5", 13)
	CONNECTION(VCC, "A5", 10)
	
	CONNECTION("A6", 6, "A5", 2)
	CONNECTION(HIT, "A5", 3)
	CONNECTION("D1", 6, "A5", 1)
	CONNECTION(VCC, "A5", 4)
	
	CONNECTION("B6", 6, "B5", 2)
	CONNECTION(HIT, "B5", 3)
	CONNECTION("D1", 6, "B5", 1)
	CONNECTION(VCC, "B5", 4)	
	
	CONNECTION(VVID, "A2", 1)
	CONNECTION("F5", 13, "A2", 12)
	CONNECTION(VVID, "A2", 4)
	CONNECTION("B2", 11, "A2", 13)
	
	CONNECTION("A5", 9, "A4", 9)
	CONNECTION("A2", 3, "A4", 10)

	CONNECTION("A2", 3, "A4", 4)	
	CONNECTION("A5", 5, "A4", 5)
	
	CONNECTION("A2", 3, "B6", 1)
	CONNECTION("B5", 5, "B6", 13)
	CONNECTION("B5", 6, "B6", 10)
	CONNECTION("A2", 2, "B6", 9)
	
	CONNECTION("B6", 8, "C4", 1)
	CONNECTION("B6", 8, "C4", 2)
	CONNECTION("B6", 8, "C4", 13)
	
	CONNECTION("A4", 8, "B4", 10)
	CONNECTION("A4", 6, "B4", 8)
	CONNECTION("B6", 8, "B4", 3)
	CONNECTION(GND, "B4", 1)
	CONNECTION("C4", 12, "B4", 11)
	CONNECTION(VCC, "B4", 7)
	CONNECTION(VCC, "B4", 4)
	CONNECTION(GND, "B4", 16)
	CONNECTION(GND, "B4", 13)



    // Paddle 1 Vertical
	CONNECTION(V256_n, "B9", 2)
	CONNECTION(VCC, "B9", 4)
	
	CONNECTION("B9", 3, "C9", 3)
	
	CONNECTION("H5", 11, "B7", 5)
	CONNECTION("A7", 8, "B7", 4)
	
	CONNECTION("B9", 3, "B8", 2)
	CONNECTION("B9", 3, "B8", 3)
	CONNECTION("B7", 6, "B8", 14)
	CONNECTION("B8", 12, "B8", 1)
	
	CONNECTION("B8", 12, "A7", 10)	
	CONNECTION("B8", 9, "A7", 9)
	CONNECTION("B8", 8, "A7", 12)
	CONNECTION("B8", 11, "A7", 13)
	
	CONNECTION("C9", 4, "B7", 1)
	CONNECTION("A7", 8, "B7", 2)
	
	
	
	// Paddle 2 Vertical
	CONNECTION(V256_n, "A9", 2)
	CONNECTION(VCC, "A9", 4)
	
	CONNECTION("A9", 3, "C9", 1)
	
	CONNECTION("A7", 6, "B7", 9)
	CONNECTION("H5", 11, "B7", 10)
	
	CONNECTION("A9", 3, "A8", 2)
	CONNECTION("A9", 3, "A8", 3)
	CONNECTION("B7", 8, "A8", 14)
	CONNECTION("A8", 12, "A8", 1)
	
	CONNECTION("A8", 12, "A7", 2)	
	CONNECTION("A8", 9, "A7", 1)
	CONNECTION("A8", 8, "A7", 4)
	CONNECTION("A8", 11, "A7", 5)
	
	CONNECTION("C9", 2, "B7", 13)
	CONNECTION("A7", 6, "B7", 12)

	
	
	// Paddle 1 & 2 Horizontal
	CONNECTION(VCC, "H3", 1)
	CONNECTION("D2", 1, "H3", 4)
	CONNECTION(H4, "H3", 3)
	CONNECTION(H128, "H3", 2)
	
	CONNECTION(H128, "G3", 10)
	CONNECTION("H3", 6, "G3", 9)
	
	CONNECTION("G3", 8, "G2", 9)
	CONNECTION(H256, "G2", 10)
	CONNECTION("B7", 3, "G2", 11)
	
	CONNECTION("B7", 11, "G2", 1)
	CONNECTION(H256_n, "G2", 2)
	CONNECTION("G3", 8, "G2", 13)	



	// Sound
	CONNECTION("E6", 8, "G4", 2)
	CONNECTION(VCC, "G4", 4)
	
	CONNECTION("G4", 3, "C3", 10)
	CONNECTION(V32, "C3", 9)
	
	CONNECTION(VCC, "C2", 2)
	CONNECTION(VCC, "C2", 4)
	CONNECTION(HIT_n, "C2", 1) 
	CONNECTION("A3", 15, "C2", 3)
	
	CONNECTION("C2", 6, "C3", 1)
	CONNECTION("A3", 14, "C3", 2)
	
	CONNECTION(VVID, "F3", 1)
	CONNECTION("F5", 13, "F3", 12)
	CONNECTION(VVID_n, "F3", 4)
	CONNECTION("B5", 9, "F3", 13)
	
	CONNECTION("F3", 3, "C3", 5)
	CONNECTION("A3", 13, "C3", 4)
	
	CONNECTION("C3", 6, "C4", 3)
	CONNECTION("C3", 3, "C4", 4)
	CONNECTION("C3", 8, "C4", 5)
	



	// Score Counters
	CONNECTION("H3", 9, "F5", 5)
	CONNECTION("E1", 3, "F5", 6)

	CONNECTION("H3", 8, "F5", 2)
	CONNECTION("E1", 3, "F5", 3)
	
	CONNECTION("F5", 4, "C7", 14)
	CONNECTION("C7", 12, "C7", 1)
	CONNECTION(SRST, "C7", 2)
	CONNECTION(SRST, "C7", 3)
	CONNECTION(GND, "C7", 6)	
	CONNECTION(GND, "C7", 7)	
		
	CONNECTION("F5", 1, "D7", 14)
	CONNECTION("D7", 12, "D7", 1)
	CONNECTION(SRST, "D7", 2)
	CONNECTION(SRST, "D7", 3)
	CONNECTION(GND, "D7", 6)
	CONNECTION(GND, "D7", 7)
	
	CONNECTION("C7", 11, "C8", 12)
	CONNECTION(VCC, "C8", 1)	
	CONNECTION(VCC, "C8", 4)
	CONNECTION(SRST_n, "C8", 13)
	
	CONNECTION("D7", 11, "C8", 9)
	CONNECTION(VCC, "C8", 8)	
	CONNECTION(VCC, "C8", 11)
	CONNECTION(SRST_n, "C8", 10)	
    
    CONNECTION(VCC, "DIPSW1", 1)
    CONNECTION("C7", 8, "DIPSW1", 2)

	CONNECTION("C7", 12, "D8", 1)
	CONNECTION("DIPSW1", 3, "D8", 2)
	CONNECTION("C8", 3, "D8", 13)

    CONNECTION(VCC, "DIPSW1", 4)
    CONNECTION("D7", 8, "DIPSW1", 5)

	CONNECTION("D7", 12, "D8", 3)
	CONNECTION("DIPSW1", 6, "D8", 4)
	CONNECTION("C8", 5, "D8", 5)
	

    // Video Summing
    CONNECTION("F2", 13, "G1", 4)  // Ball
    CONNECTION("F2", 12, "G2", 12)
    CONNECTION("F2", 10, NET)
    CONNECTION("F2", 9, "G2", 8)

    CONNECTION("F2", 8, "E4", 13)

    CONNECTION("VIDEO", 1, "E4", 12)
    CONNECTION("VIDEO", 2, "D3", 8)

    CONNECTION("VIDEO", Video::HBLANK_PIN, HBLANK)
    CONNECTION("VIDEO", Video::VBLANK_PIN, VBLANK)


    // Audio Summing
    CONNECTION(ATTRACT_n, "C1", 4)
    CONNECTION("C4", 6, "C1", 5)

    CONNECTION("AUDIO", 1, "C1", 6)
    CONNECTION("AUDIO", i1, "AUDIO", Audio::OUTPUT_MONO)

#ifdef DEBUG
    // Horizontal ball counter
    CONNECTION("LOG1", 1, CLK)
    CONNECTION("LOG1", 2, HBLANK_n)
    CONNECTION("LOG1", 3, "G7", 14)
    CONNECTION("LOG1", 4, "G7", 13)
    CONNECTION("LOG1", 5, "G7", 12)
    CONNECTION("LOG1", 6, "G7", 11)
    CONNECTION("LOG1", 7, "G7", 15)
    CONNECTION("LOG1", 8, "G5", 8)
    CONNECTION("LOG1", 9, "H7", 14)
    CONNECTION("LOG1", 10, "H7", 13)
    CONNECTION("LOG1", 11, "H7", 12)
    CONNECTION("LOG1", 12, "H7", 11)
    CONNECTION("LOG1", 13, "H7", 15)
    CONNECTION("LOG1", 14, "G6", 5)
#endif

CIRCUIT_LAYOUT_END

