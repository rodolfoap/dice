/* Crossfire by Atari 1975 */

#include "../circuit_desc.h"
#include "crossfire.h"

#define DEBUG
#undef DEBUG

#ifdef DEBUG
static VcdLogDesc vcd_log_desc
(
    "output_crossfire.vcd",
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
    15, "O",
    16, "P"
);
#endif

static Mono9602Desc a6_desc(K_OHM(12.0), U_FARAD(5.0), K_OHM(12.0), U_FARAD(5.0));
static Mono555Desc a7_desc(M_OHM(6.8), U_FARAD(0.1));
static Mono555Desc n9_desc(K_OHM(1180.0), U_FARAD(100.0));
static Astable555Desc m9_desc(K_OHM(47.0), K_OHM(3.3), U_FARAD(0.1));
static Astable555Desc l9_desc(K_OHM(47.0), K_OHM(3.3), U_FARAD(0.1));

static PotentimeterMono555Desc pot1_desc("playtime", "Play Time", K_OHM(1180.0), K_OHM(680.0), K_OHM(1680.0), n9_desc);

FireSoundDesc fire1_desc(&m9_desc);
FireSoundDesc fire2_desc(&l9_desc);

static CapacitorDesc cap12_desc(U_FARAD(0.001));
static CapacitorDesc cap27_desc(U_FARAD(0.001));
static CapacitorDesc cap29_desc(U_FARAD(0.1));
static CapacitorDesc cap36_desc(U_FARAD(0.001));
static CapacitorDesc capx_desc(U_FARAD(0.001));

static INPUT_DESC( crossfire )
    INPUT_INFO(JOYSTICK1_INPUT, {{ Joystick::UP, Joystick::DOWN }}, "Move Left Gun")
    INPUT_INFO(BUTTONS1_INPUT, {{ 1 }}, "Fire Gun")
    
    INPUT_INFO(JOYSTICK2_INPUT, {{ Joystick::UP, Joystick::DOWN }}, "Move Right Gun")
    INPUT_INFO(BUTTONS2_INPUT, {{ 1 }}, "Fire Gun")

    INPUT_INFO(COIN_INPUT, {{ 1 }}, "Insert Coin and Start Game")
INPUT_DESC_END


CIRCUIT_LAYOUT( crossfire )

    /**************************************************************************
    * Chips                                                                   *
    **************************************************************************/    
    CHIP("XTAL", CLOCK_12_096_MHZ)

    CHIP("A1", 7474)
    CHIP("A2", 7493)
    CHIP("A3", 7402)
    CHIP("A4", 7408)
    CHIP("A5", 7400)
    CHIP("A6", 9602, &a6_desc)
    CHIP("A7", 555_Mono, &a7_desc)

    CHIP("B1", 7402)
    CHIP("B2", 7420)
    CHIP("B3", 9316)
    CHIP("B4", 9321)
    CHIP("B5", 7400)
    CHIP("B6", 74107)
    CHIP("B7", 7404)

    CHIP("C1", 7402)
    CHIP("C2", 7400)
    CHIP("C3", 7486)
    CHIP("C4", 7483)
    CHIP("C5", 7400)
    CHIP("C6", 7474)
    CHIP("C7", 7474)
    CHIP("C8", 7404)

    CHIP("D1", 7486)
    CHIP("D2", 7410)
    CHIP("D3", 74193)
    CHIP("D4", 7486)
    CHIP("D5", 7402)
    CHIP("D6", 7402)
    CHIP("D7", 9322)
    CHIP("D8", 7400)

    CHIP("E1", 9316)
    CHIP("E2", 7486)
    CHIP("E3", 74193)
    CHIP("E4", 7400)
    CHIP("E5", 7402)
    CHIP("E6", 7474)
    CHIP("E7", 7474)

    CHIP("F1", 7408)
    CHIP("F2", 7483)
    CHIP("F3", 74107)
    CHIP("F4", 7410)
    CHIP("F5", 7432)
    CHIP("F6", 7400)
    CHIP("F7", 7474)

    CHIP("H1", 9316)
    CHIP("H2", 74107)
    CHIP("H3", 7404)
    CHIP("H4", 7400)
    CHIP("H5", 7400)
    CHIP("H6", 7400)
    CHIP("H7", 7402)
    CHIP("H8", 74107)
    CHIP("H9", 7400)

    CHIP("J1", 7427)
    CHIP("J2", 9312)
    CHIP("J3", 74107)
    CHIP("J4", 7493)
    CHIP("J5", 7430)
    CHIP("J6", 7400)
    CHIP("J7", 7420)
    CHIP("J8", 7410)
    CHIP("J9", 7420)

    CHIP("K1", 9312)
    CHIP("K2", 7448)
    CHIP("K3", 7490)
    CHIP("K4", 7410)
    CHIP("K5", 7493)
    CHIP("K6", 7493)
    CHIP("K7", 7402)
    CHIP("K8", 7400)
    CHIP("K9", 7404)

    CHIP("L1", 7400)
    CHIP("L2", 9322)
    CHIP("L3", 7490)
    CHIP("L4", 7420)
    CHIP("L5", 7493)
    CHIP("L6", 7492)
    CHIP("L7", 7400)
    CHIP("L8", 7474)
    CHIP("L9", 555_Astable, &l9_desc)	

    CHIP("M2", 7404)
    CHIP("M3", 7402)
    CHIP("M4", 7400)
    CHIP("M5", 7404)
    CHIP("M6", 7410)
    CHIP("M7", 7404)
    CHIP("M8", 74107)
    CHIP("M9", 555_Astable, &m9_desc)	

    CHIP("N3", 7400)
    CHIP("N4", 7400)
    CHIP("N5", 7402)
    CHIP("N6", 7486)
    CHIP("N7", 7430)
    CHIP("N8", 7402)
    CHIP("N9", 555_Mono, &n9_desc)

    CHIP("C12", CAPACITOR, &cap12_desc)
    CHIP("C27", CAPACITOR, &cap27_desc)
    CHIP("C29", CAPACITOR, &cap29_desc)
    CHIP("C36", CAPACITOR, &cap36_desc)
    CHIP("CAPX", CAPACITOR, &capx_desc)

    CHIP("??", 7402)

    // Swapped left + right controls so that game follows standard
    // player 1 on left, player 2 on right
    CHIP("STICK1", JOYSTICK2_INPUT)
    CHIP("BUTTON1", BUTTONS2_INPUT)
    CHIP("STICK1_V", JOYSTICK_VOLTAGE)
    CHIP("BULLET1_V", BULLET1_VOLTAGE)

    CHIP("STICK2", JOYSTICK1_INPUT)
    CHIP("BUTTON2", BUTTONS1_INPUT)
    CHIP("STICK2_V", JOYSTICK_VOLTAGE)
    CHIP("BULLET2_V", BULLET2_VOLTAGE)

    CHIP("VSCAN_COMP1", SCAN_COMPARATOR)
    CHIP("VSCAN_COMP2", SCAN_COMPARATOR)
    CHIP("HSCAN_COMP1", SCAN_COMPARATOR)
    CHIP("HSCAN_COMP2", SCAN_COMPARATOR)

    CHIP("FIRE_SOUND1", FIRE_SOUND_CV, &fire1_desc)
    CHIP("FIRE_SOUND2", FIRE_SOUND_CV, &fire2_desc)

    CHIP("COIN", COIN_INPUT)

    CHIP("GNE", GNE, &n9_desc)

    CHIP("POT1", POT_555_MONO, &pot1_desc)
    POTENTIOMETER_CONNECTION("POT1", "N9")
    POTENTIOMETER_CONNECTION("POT1", "GNE_TIMER")

    INPUT(crossfire)

    OPTIMIZATION_HINT("J5", 256, 64)
    OPTIMIZATION_HINT("N7", 256, 64)

    OPTIMIZATION_HINT("B2", 256, 128)
    OPTIMIZATION_HINT("J1", 256, 64)

    OPTIMIZATION_HINT("C6", 4, 64)
    OPTIMIZATION_HINT("C7", 2, 64)
    OPTIMIZATION_HINT("C8", 2, 64)

#ifdef DEBUG
    CHIP("LOG", VCD_LOG, &vcd_log_desc)
#endif

    /**************************************************************************
    * Signals                                                                 *
    **************************************************************************/
    #define CLOCK "B7", 6

    #define H1 "H8", 5
    #define H2 "H8", 3
    #define H4 "K6", 12
    #define H8 "K6", 9
    #define H16 "K6", 8
    #define H32 "K6", 11
    #define H64 "L6", 12
    #define H64_n "M5", 4
    #define H128 "L6", 11
    #define H256 "L6", 9
    #define H384 "L6", 8
    #define H384_n "M5", 2
    
    #define V1 "K5", 12
    #define V2 "K5", 9
    #define V4 "K5", 8
    #define V8 "K5", 11
    #define V16 "L5", 12
    #define V32 "L5", 9
    #define V32_n "M5", 10
    #define V64 "L5", 8
    #define V64_n "M5", 8
    #define V128 "L5", 11

    #define F1 "J4", 12
    #define F2 "J4", 9
    #define F4 "J4", 8
    #define F8 "J4", 11

    #define HSYNC "M5", 6
    #define HSYNC_n "M6", 8
    #define VSYNC "M5", 12
    #define VSYNC_n "J5", 8

    #define GOAL1_n "M6", 6
    #define GOAL2_n "C36", 2
    
    #define BOUNDS "M4", 8
    #define BOUNDS_n "M4", 11
    #define LBR_n "L4", 8
    #define FN1 "N5", 13
    #define FN2 "A4", 8

    #define ATTRACT "M8", 2
    #define ATTRACT_n "M8", 3
    #define VW "B1", 10
    #define HW "M3", 4

    #define HR_n "H4", 8
    #define VR_n "A5", 6

    #define HIT1 "J6", 3
    #define HIT1_n "J6", 6
    #define HIT2 "J6", 11
    #define HIT2_n "J6", 8
    
    #define FIRE1 "F7", 5
    #define FIRE1_n "F7", 6
    #define FIRE2 "F7", 9
    #define FIRE2_n "F7", 8

    #define START "L8", 6
    #define START_n "L8", 5

    #define Q_BALL "B1", 4
    #define SCORE "CAPX", 2

    #define GNE "GNE", 2



    /**************************************************************************
    * Layout - Page 1                                                         *
    **************************************************************************/
    // Clock Generation
    CONNECTION("XTAL", 1, "B7", 5)

    // Horizontal Counters
    CONNECTION(VCC, "H8", 8)
    CONNECTION(CLOCK, "H8", 9)
    CONNECTION(VCC, "H8", 11)
    CONNECTION(VCC, "H8", 10)

    CONNECTION(VCC, "H8", 1)
    CONNECTION(H1, "H8", 12)
    CONNECTION(VCC, "H8", 4)
    CONNECTION(VCC, "H8", 13)

    CONNECTION(H2, "K6", 14)
    CONNECTION(H4, "K6", 1)
    CONNECTION(GND, "K6", 2)
    CONNECTION(GND, "K6", 3)

    CONNECTION(H32, "L6", 14)
    CONNECTION(H64, "L6", 1)
    CONNECTION(GND, "L6", 6)
    CONNECTION(GND, "L6", 7)

    

    // Vertical Counters
    CONNECTION(H384, "K5", 14)
    CONNECTION(V1, "K5", 1)
    CONNECTION(GND, "K5", 2)
    CONNECTION(GND, "K5", 3)

    CONNECTION(V8, "L5", 14)
    CONNECTION(V16, "L5", 1)
    CONNECTION(GND, "L5", 2)
    CONNECTION(GND, "L5", 3)

    

    // Frame Counter
    CONNECTION(V128, "J4", 14)
    CONNECTION(F1, "J4", 1)
    CONNECTION(GND, "J4", 2)
    CONNECTION(GND, "J4", 3)


    
    // Inverted Counter Signals
    CONNECTION(H64, "M5", 3)
    CONNECTION(H384, "M5", 1)
    CONNECTION(V32, "M5", 11)
    CONNECTION(V64, "M5", 9)


    
    // H/V Sync
    CONNECTION(H384, "M6", 9)
    CONNECTION(H256, "M6", 10)
    CONNECTION(H64, "M6", 11)

    CONNECTION("M6", 8, "M5", 5)

    CONNECTION(V128, "J5", 2)
    CONNECTION(V64, "J5", 4)
    CONNECTION(V32, "J5", 12)
    CONNECTION(V16, "J5", 1)
    CONNECTION(V8, "J5", 3)
    CONNECTION(V4, "J5", 6)
    CONNECTION(V2, "J5", 5)
    CONNECTION(V2, "J5", 11)

    CONNECTION("J5", 8, "M5", 13)

    CONNECTION(H32, "A4", 10)
    CONNECTION(HSYNC, "A4", 9)



    // Hit 1 + 2
    CONNECTION(Q_BALL, "D2", 4)
    CONNECTION(Q_BALL, "D2", 5)
    CONNECTION("D6", 10, "D2", 3)

    CONNECTION("D2", 6, "J6", 1)
    CONNECTION("J6", 6, "J6", 2)

    CONNECTION("J6", 3, "J6", 4)
    CONNECTION(HSYNC_n, "J6", 5)

    CONNECTION(Q_BALL, "B5", 9)
    CONNECTION("D6", 13, "B5", 10)

    CONNECTION("B5", 8, "J6", 13)
    CONNECTION("J6", 8, "J6", 12)

    CONNECTION("J6", 11, "J6", 10)
    CONNECTION(HSYNC_n, "J6", 9)



    // Goals
    CONNECTION(H384, "N5", 3)
    CONNECTION(H256, "N5", 2)

    CONNECTION(H128, "??", 2)
    CONNECTION(H32, "??", 3)

    CONNECTION("N5", 1, "L7", 2)
    CONNECTION("??", 1, "L7", 1)

    CONNECTION("L7", 3, "C36", 1)

    CONNECTION("C36", 2, "M7", 3)

    CONNECTION(H32, "K7", 5)
    CONNECTION(H64, "K7", 6)

    CONNECTION("K7", 4, "M6", 3)
    CONNECTION(H384, "M6", 5)
    CONNECTION(H256, "M6", 4)

    CONNECTION("M6", 6, "M7", 5)


    
    // Bounds
    CONNECTION(V128, "L4", 9)
    CONNECTION(V64, "L4", 13)
    CONNECTION(V32, "L4", 10)
    CONNECTION(V16, "L4", 12)

    CONNECTION(V32, "N5", 8)
    CONNECTION(V128, "N5", 9)

    CONNECTION(V64, "M4", 2)
    CONNECTION("N5", 10, "M4", 1)

    CONNECTION("L4", 8, "M4", 9)
    CONNECTION("M4", 11, "M4", 10)

    CONNECTION("M4", 8, "M4", 13)
    CONNECTION("M4", 3, "M4", 12)



    // Obstacles
    CONNECTION(H384_n, "N6", 9)
    CONNECTION(H256, "N6", 10)

    CONNECTION("N6", 8, "N5", 12)
    CONNECTION(H128, "N5", 11)

    CONNECTION("N5", 13, "N4", 1)
    CONNECTION(V32, "N4", 2)

    CONNECTION(H128, "M6", 13)
    CONNECTION(V64, "M6", 1)
    CONNECTION(V32_n, "M6", 2)

    CONNECTION("N4", 3, "N4", 4)
    CONNECTION("M6", 12, "N4", 5)

    CONNECTION(H16, "N6", 13)
    CONNECTION(H384_n, "N6", 12)

    CONNECTION(V128, "N6", 1)
    CONNECTION(V8, "N6", 2)

    CONNECTION("M4", 11, "N7", 12)
    CONNECTION("N4", 6, "N7", 4)
    CONNECTION("N6", 11, "N7", 11)
    CONNECTION("N6", 3, "N7", 6)
    CONNECTION("N6", 3, "N7", 5)
    CONNECTION(H32, "N7", 1)
    CONNECTION(H64_n, "N7", 2)
    CONNECTION(V16, "N7", 3)

    CONNECTION("N7", 8, "H3", 3)



    // HREV + VREV (Ball Direction Control)
    CONNECTION("M4", 8, "A4", 12)
    CONNECTION(Q_BALL, "A4", 13)

    CONNECTION(Q_BALL, "A4", 2)
    CONNECTION("H3", 4, "A4", 1)

    CONNECTION("H3", 2, "A5", 12)
    CONNECTION("A4", 3, "A5", 13)

    CONNECTION("A4", 3, "A5", 2)
    CONNECTION("F4", 8, "A5", 1)

    CONNECTION("A4", 11, "A6", 12)
    CONNECTION("A5", 11, "A6", 11)
    CONNECTION(VCC, "A6", 13)

    CONNECTION("A5", 3, "A6", 5)
    CONNECTION(GND, "A6", 4)
    CONNECTION(VCC, "A6", 3)

    CONNECTION(VCC, "B6", 8)
    CONNECTION("A6", 9, "B6", 9)
    CONNECTION(VCC, "B6", 11)
    CONNECTION("B7", 8, "B6", 10)

    CONNECTION(VCC, "B6", 1)
    CONNECTION("A6", 7, "B6", 12)
    CONNECTION(VCC, "B6", 4)
    CONNECTION("B7", 8, "B6", 13)


    
    // HR
    CONNECTION("B7", 8, "H4", 4)
    CONNECTION("H4", 8, "H4", 5)

    CONNECTION("H4", 6, "H4", 9)
    CONNECTION(GOAL1_n, "H4", 10)
    


    // Fire 1
    CONNECTION("BUTTON1", 1_NO, "B7", 1)

    CONNECTION(HW, "H4", 2)
    CONNECTION(H384_n, "H4", 1)

    CONNECTION("H4", 3, "H5", 5)
    CONNECTION("H5", 3, "H5", 4)

    CONNECTION("H5", 6, "H5", 1)
    CONNECTION(VSYNC_n, "H5", 2)

    CONNECTION("H5", 3, "H6", 5)
    CONNECTION(H384_n, "H6", 4)

    CONNECTION(GOAL2_n, "F6", 4)
    CONNECTION("H6", 6, "F6", 5)
    
    CONNECTION("D6", 10, "F6", 10)
    CONNECTION("F6", 6, "F6", 9)

    CONNECTION(START_n, "H6", 10)
    CONNECTION("F6", 8, "H6", 9)

    CONNECTION("H6", 8, "H7", 11)
    CONNECTION(HIT1, "H7", 12)

    CONNECTION(VCC, "F7", 4)
    CONNECTION(VCC, "F7", 2)
    CONNECTION("D7", 4, "F7", 3)
    CONNECTION("H7", 13, "F7", 1)

    CONNECTION(FIRE1, "BULLET1_V", 1)

    CONNECTION(FN2, "HSCAN_COMP1", 1)
    CONNECTION("BULLET1_V", 2, "HSCAN_COMP1", 2)

    CONNECTION("HSCAN_COMP1", 3, "C8", 13)    
    
    
    
    // Fire 2
    CONNECTION("BUTTON2", 1_NO, "C8", 5)

    CONNECTION(HW, "H4", 12)
    CONNECTION(H384, "H4", 13)

    CONNECTION("H4", 11, "H5", 9)
    CONNECTION("H5", 11, "H5", 10)

    CONNECTION("H5", 8, "H5", 12)
    CONNECTION(VSYNC_n, "H5", 13)

    CONNECTION("H5", 11, "H6", 2)
    CONNECTION(H384, "H6", 1)

    CONNECTION(GOAL1_n, "F6", 1)
    CONNECTION("H6", 3, "F6", 2)
    
    CONNECTION("D6", 13, "F6", 13)
    CONNECTION("F6", 3, "F6", 12)

    CONNECTION(START_n, "H6", 13)
    CONNECTION("F6", 11, "H6", 12)

    CONNECTION("H6", 11, "H7", 9)
    CONNECTION(HIT2, "H7", 8)

    CONNECTION(VCC, "F7", 10)
    CONNECTION(VCC, "F7", 12)
    CONNECTION("D7", 7, "F7", 11)
    CONNECTION("H7", 10, "F7", 13)

    CONNECTION(FIRE2_n, "BULLET2_V", 1)

    CONNECTION(FN2, "HSCAN_COMP2", 1)
    CONNECTION("BULLET2_V", 2, "HSCAN_COMP2", 2)

    CONNECTION("HSCAN_COMP2", 3, "C8", 11)




    // Gun 1 / Bullet 1
    CONNECTION("STICK1", Joystick::UP, "STICK1_V", 1)
    CONNECTION("STICK1", Joystick::DOWN, "STICK1_V", 2)

    CONNECTION(VSYNC, "VSCAN_COMP1", 1)
    CONNECTION("STICK1_V", 3, "VSCAN_COMP1", 2)

    CONNECTION("VSCAN_COMP1", 3, "C8", 3)

    CONNECTION(BOUNDS_n, "D8", 9)
    CONNECTION("C8", 4, "D8", 10)

    CONNECTION("D8", 8, "D8", 5)
    CONNECTION(LBR_n, "D8", 4)

    CONNECTION("D7", 12, "C12", 1)
        
    CONNECTION("E6", 9, "E7", 10)
    CONNECTION(GND, "E7", 12)
    CONNECTION("C12", 2, "E7", 11)
    CONNECTION(VCC, "E7", 13)

    CONNECTION(VCC, "E6", 10)
    CONNECTION("E7", 9, "E6", 12)
    CONNECTION(V1, "E6", 11)
    CONNECTION(VCC, "E6", 13)

    CONNECTION(GOAL1_n, "D6", 5)
    CONNECTION("E6", 9, "D6", 6)

    CONNECTION("C6", 9, "C7", 10)
    CONNECTION(GND, "C7", 12)
    CONNECTION("C8", 12, "C7", 11)
    CONNECTION(VCC, "C7", 13)

    CONNECTION(FIRE1, "C6", 10)
    CONNECTION("C7", 9, "C6", 12)
    CONNECTION(H2, "C6", 11)
    CONNECTION(VCC, "C6", 13)

    CONNECTION("E6", 9, "D6", 9)
    CONNECTION("C6", 9, "D6", 8)



    // S + G
    CONNECTION(START, "J1", 3)
    CONNECTION("N3", 8, "J1", 4)
    CONNECTION("N3", 6, "J1", 5)

    CONNECTION(ATTRACT_n, "A7", 4)
    CONNECTION("J1", 6, "A7", 2)

    CONNECTION("A7", 3, "B7", 9)



    // Gun / Bullet 2
    CONNECTION("STICK2", Joystick::UP, "STICK2_V", 1)
    CONNECTION("STICK2", Joystick::DOWN, "STICK2_V", 2)

    CONNECTION(VSYNC, "VSCAN_COMP2", 1)
    CONNECTION("STICK2_V", 3, "VSCAN_COMP2", 2)

    CONNECTION("VSCAN_COMP2", 3, "C8", 1)

    CONNECTION(BOUNDS_n, "D8", 12)
    CONNECTION("C8", 2, "D8", 13)

    CONNECTION("D8", 11, "D8", 2)
    CONNECTION(LBR_n, "D8", 1)

    CONNECTION(ATTRACT_n, "M3", 2)
    CONNECTION(VW, "M3", 3)

    CONNECTION(ATTRACT, "D7", 1)
    CONNECTION("B7", 2, "D7", 2)
    CONNECTION(F8, "D7", 3)
    CONNECTION("C8", 6, "D7", 5)
    CONNECTION(F8, "D7", 6)
    CONNECTION("D8", 6, "D7", 14)
    CONNECTION(VW, "D7", 13)
    CONNECTION("D8", 3, "D7", 11)
    CONNECTION("C3", 8, "D7", 10)
    CONNECTION("M3", 1, "D7", 15)

    CONNECTION("E6", 5, "E7", 4)
    CONNECTION(GND, "E7", 2)
    CONNECTION("D7", 9, "E7", 3)
    CONNECTION(VCC, "E7", 1)

    CONNECTION(VCC, "E6", 4)
    CONNECTION("E7", 5, "E6", 2)
    CONNECTION(V1, "E6", 3)
    CONNECTION(VCC, "E6", 1)

    CONNECTION(GOAL2_n, "D6", 2)
    CONNECTION("E6", 5, "D6", 3)

    CONNECTION("C6", 5, "C7", 4)
    CONNECTION(GND, "C7", 2)
    CONNECTION("C8", 10, "C7", 3)
    CONNECTION(VCC, "C7", 1)

    CONNECTION(FIRE2, "C6", 4)
    CONNECTION("C7", 5, "C6", 2)
    CONNECTION(H2, "C6", 3)
    CONNECTION(VCC, "C6", 1)

    CONNECTION("E6", 5, "D6", 12)
    CONNECTION("C6", 5, "D6", 11)




    /**************************************************************************
    * Layout - Page 2                                                         *
    **************************************************************************/
    // Horizontal Ball Control
    CONNECTION("C5", 6, "E4", 12)
    CONNECTION("C5", 3, "E4", 13)

    CONNECTION("C5", 3, "D5", 2)
    CONNECTION("B6", 2, "D5", 3)
    
    CONNECTION("C5", 3, "D5", 8)
    CONNECTION("B6", 3, "D5", 9)

    CONNECTION("C5", 6, "D5", 5)
    CONNECTION("B6", 3, "D5", 6)

    CONNECTION("C5", 6, "D5", 11)
    CONNECTION("B6", 2, "D5", 12)

    CONNECTION(VCC, "F3", 1)
    CONNECTION("E4", 11, "F3", 12)
    CONNECTION(GND, "F3", 4)
    CONNECTION("B7", 8, "F3", 13)

    CONNECTION("D5", 10, "F5", 1)
    CONNECTION("D5", 13, "F5", 2)

    CONNECTION("D5", 4, "F5", 10)
    CONNECTION("D5", 1, "F5", 9)

    CONNECTION("E3", 12, "F4", 13)
    CONNECTION("F3", 3, "F4", 2)
    CONNECTION("F5", 3, "F4", 1)

    CONNECTION("F5", 8, "E4", 4)
    CONNECTION("E3", 13, "E4", 5)

    CONNECTION("F4", 12, "E3", 5)
    CONNECTION("E4", 6, "E3", 4)
    CONNECTION(GND, "E3", 15)
    CONNECTION(GND, "E3", 1)
    CONNECTION(GND, "E3", 10)
    CONNECTION(VCC, "E3", 9)
    CONNECTION(GND, "E3", 14)
    CONNECTION("B7", 8, "E3", 11)

    CONNECTION("B6", 3, "E2", 10)
    CONNECTION("E3", 3, "E2", 9)

    CONNECTION("B6", 3, "E2", 13)
    CONNECTION("E3", 2, "E2", 12)

    CONNECTION("B6", 3, "E2", 5)
    CONNECTION("E3", 6, "E2", 4)

    CONNECTION("B6", 3, "E2", 2)
    CONNECTION("E3", 7, "E2", 1)

    CONNECTION("F3", 2, "A3", 12)
    CONNECTION(F1, "A3", 11)

    CONNECTION(F2, "F2", 10)
    CONNECTION("E2", 8, "F2", 11)
    CONNECTION("A3", 13, "F2", 8)
    CONNECTION("E2", 11, "F2", 7)
    CONNECTION(GND, "F2", 3)
    CONNECTION("E2", 6, "F2", 4)
    CONNECTION(VCC, "F2", 1)
    CONNECTION("E2", 3, "F2", 16)
    CONNECTION("E2", 3, "F2", 13)
    
    CONNECTION("F2", 14, "H3", 11)

    CONNECTION(VSYNC, "F1", 4)
    CONNECTION(V1, "F1", 5)

    CONNECTION("F1", 6, "F1", 2)
    CONNECTION("F2", 2, "F1", 1)

    CONNECTION("F1", 6, "F1", 13)
    CONNECTION("F2", 15, "F1", 12)
    
    CONNECTION("F1", 6, "F1", 9)
    CONNECTION("H3", 10, "F1", 10)




    // Left, Right, Up, Down
    CONNECTION("B4", 4, "F4", 10)
    CONNECTION("H3", 2, "F4", 9)
    CONNECTION("B4", 6, "F4", 11)
    
    CONNECTION(HIT1, "C5", 2)
    CONNECTION("F4", 8, "C5", 1)

    CONNECTION("F4", 8, "C5", 4)
    CONNECTION(HIT2, "C5", 5)


    CONNECTION("B4", 5, "B5", 4)
    CONNECTION("B4", 4, "B5", 5)

    CONNECTION(HIT1_n, "B5", 12)
    CONNECTION(HIT2_n, "B5", 13)

    CONNECTION("B4", 6, "B5", 2)
    CONNECTION("B4", 7, "B5", 1)

    CONNECTION("B5", 6, "C5", 9)
    CONNECTION("B5", 11, "C5", 10)

    CONNECTION("B5", 11, "C5", 13)
    CONNECTION("B5", 3, "C5", 12)



    // Ball Horizontal Counter
    CONNECTION(VCC, "E1", 10)
    CONNECTION(VCC, "E1", 7)
    CONNECTION(H1, "E1", 2)
    CONNECTION("F1", 3, "E1", 3)
    CONNECTION("F1", 11, "E1", 4)
    CONNECTION("F1", 8, "E1", 5)
    CONNECTION("F1", 8, "E1", 6)
    CONNECTION("N4", 11, "E1", 9)
    CONNECTION(HR_n, "E1", 1)

    CONNECTION("F1", 8, "H3", 5)

    CONNECTION(VCC, "H1", 10)
    CONNECTION("E1", 15, "H1", 7)
    CONNECTION(H1, "H1", 2)
    CONNECTION("F1", 8, "H1", 3)
    CONNECTION("F1", 8, "H1", 4)
    CONNECTION("F1", 8, "H1", 5)
    CONNECTION("H3", 6, "H1", 6)
    CONNECTION("N4", 11, "H1", 9)
    CONNECTION(HR_n, "H1", 1)

    CONNECTION(VCC, "H2", 8)
    CONNECTION("H1", 15, "H2", 9)
    CONNECTION(VCC, "H2", 11)
    CONNECTION(HR_n, "H2", 10)

    CONNECTION("H1", 15, "H3", 9)

    CONNECTION("H2", 5, "M3", 5)
    CONNECTION("H3", 8, "M3", 6)

    CONNECTION("M3", 4, "N4", 12)
    CONNECTION("E1", 15, "N4", 13)



    // Vertical Ball Control
    CONNECTION("C5", 8, "E4", 1)
    CONNECTION("C5", 11, "E4", 2)

    CONNECTION("C5", 11, "E5", 2)
    CONNECTION("B6", 6, "E5", 3)

    CONNECTION("C5", 11, "E5", 8)
    CONNECTION("B6", 5, "E5", 9)

    CONNECTION("C5", 8, "E5", 5)
    CONNECTION("B6", 5, "E5", 6)

    CONNECTION("C5", 8, "E5", 11)
    CONNECTION("B6", 6, "E5", 12)

    CONNECTION(VCC, "F3", 8)
    CONNECTION("E4", 3, "F3", 9)
    CONNECTION(GND, "F3", 11)
    CONNECTION("B7", 8, "F3", 10)

    CONNECTION("E5", 10, "F5", 4)
    CONNECTION("E5", 13, "F5", 5)

    CONNECTION("E5", 4, "F5", 13)
    CONNECTION("E5", 1, "F5", 12)

    CONNECTION("D3", 12, "F4", 4)
    CONNECTION("F3", 5, "F4", 5)
    CONNECTION("F5", 6, "F4", 3)

    CONNECTION("F5", 11, "E4", 10)
    CONNECTION("D3", 13, "E4", 9)

    CONNECTION("F4", 6, "D3", 5)
    CONNECTION("E4", 8, "D3", 4)
    CONNECTION(GND, "D3", 15)
    CONNECTION(GND, "D3", 1)
    CONNECTION(GND, "D3", 10)
    CONNECTION(VCC, "D3", 9)
    CONNECTION(GND, "D3", 14)
    CONNECTION("B7", 8, "D3", 11)

    CONNECTION("B6", 5, "D4", 5)
    CONNECTION("D3", 3, "D4", 4)

    CONNECTION("B6", 5, "D4", 10)
    CONNECTION("D3", 2, "D4", 9)

    CONNECTION("B6", 5, "D4", 13)
    CONNECTION("D3", 6, "D4", 12)

    CONNECTION("B6", 5, "D4", 2)
    CONNECTION("D3", 7, "D4", 1)

    CONNECTION("F3", 6, "A3", 8)
    CONNECTION(F1, "A3", 9)

    CONNECTION(F2, "C4", 10)
    CONNECTION("D4", 6, "C4", 11)
    CONNECTION("A3", 10, "C4", 8)
    CONNECTION("D4", 8, "C4", 7)
    CONNECTION(GND, "C4", 3)
    CONNECTION("D4", 11, "C4", 4)
    CONNECTION(VCC, "C4", 1)
    CONNECTION("D4", 3, "C4", 16)
    CONNECTION("D4", 3, "C4", 13)

    CONNECTION("C4", 14, "H3", 13)




    // Ball Vertical Counter
    CONNECTION(VCC, "B3", 10)
    CONNECTION(VCC, "B3", 7)
    CONNECTION(HSYNC, "B3", 2)
    CONNECTION("C4", 2, "B3", 3)
    CONNECTION("C4", 15, "B3", 4)
    CONNECTION("H3", 12, "B3", 5)
    CONNECTION("H3", 12, "B3", 6)
    CONNECTION("L1", 3, "B3", 9)
    CONNECTION(VR_n, "B3", 1)

    CONNECTION("C2", 3, "A2", 2)
    CONNECTION("C2", 3, "A2", 3)
    CONNECTION("B3", 15, "A2", 14)
    CONNECTION("A2", 12, "A2", 1)

    CONNECTION("A2", 11, "B2", 12)
    CONNECTION("A2", 8, "B2", 9)
    CONNECTION("A2", 9, "B2", 10)
    CONNECTION("A2", 12, "B2", 13)

    CONNECTION(VCC, "A1", 4)
    CONNECTION("L1", 3, "A1", 2)
    CONNECTION(HSYNC, "A1", 3)
    CONNECTION(VCC, "A1", 1)

    CONNECTION(VCC, "A1", 10)
    CONNECTION("A1", 5, "A1", 12)
    CONNECTION(HSYNC, "A1", 11)
    CONNECTION(VCC, "A1", 13)

    CONNECTION("A1", 6, "B1", 8)
    CONNECTION("B2", 8, "B1", 9)

    CONNECTION("B1", 10, "L1", 1)
    CONNECTION("B3", 15, "L1", 2)

    CONNECTION(VR_n, "C2", 1)
    CONNECTION("A1", 9, "C2", 2)



    // Ball Graphics
    CONNECTION("E1", 14, "D1", 2)
    CONNECTION("E1", 11, "D1", 1)

    CONNECTION("E1", 13, "D1", 12)
    CONNECTION("E1", 11, "D1", 13)

    CONNECTION("E1", 12, "D1", 10)
    CONNECTION("E1", 11, "D1", 9)

    CONNECTION("B3", 14, "C3", 2)
    CONNECTION("B3", 11, "C3", 1)

    CONNECTION("B3", 13, "C3", 4)
    CONNECTION("B3", 11, "C3", 5)

    CONNECTION("B3", 12, "C3", 10)
    CONNECTION("B3", 11, "C3", 9)

    CONNECTION("C3", 3, "A3", 5)
    CONNECTION("C3", 6, "A3", 6)

    CONNECTION("C3", 8, "B4", 1)
    CONNECTION("B3", 11, "B4", 3)
    CONNECTION("A3", 4, "B4", 2)

    CONNECTION("C3", 8, "H3", 1)

    CONNECTION("D1", 11, "C2", 4)
    CONNECTION("C3", 6, "C2", 5)

    CONNECTION("D1", 3, "D2", 11)
    CONNECTION("C3", 3, "D2", 10)
    CONNECTION("C3", 6, "D2", 9)

    CONNECTION("D1", 3, "D2", 1)
    CONNECTION("D1", 11, "D2", 13)
    CONNECTION("C3", 3, "D2", 2)

    CONNECTION("D1", 8, "C2", 9)
    CONNECTION("C3", 8, "C2", 10)

    CONNECTION("D1", 3, "C1", 3)
    CONNECTION("C3", 6, "C1", 2)

    CONNECTION("D1", 8, "C1", 8)
    CONNECTION("C3", 8, "C1", 9)
    
    CONNECTION("D1", 11, "C1", 6)
    CONNECTION("C3", 3, "C1", 5)

    CONNECTION("C2", 6, "B2", 4)
    CONNECTION("D2", 8, "B2", 2)
    CONNECTION("D2", 12, "B2", 1)
    CONNECTION("C2", 8, "B2", 5)

    CONNECTION("C1", 1, "B1", 2)
    CONNECTION("C1", 10, "B1", 3)

    CONNECTION("C1", 10, "C1", 11)
    CONNECTION("C1", 4, "C1", 12)

    CONNECTION("B2", 6, "J1", 10)
    CONNECTION("B1", 1, "J1", 9)
    CONNECTION("C1", 13, "J1", 11)

    CONNECTION("J1", 8, "C27", 1)

    CONNECTION(VW, "C2", 12)
    CONNECTION(HW, "C2", 13)

    CONNECTION("C2", 11, "B1", 6)
   // CONNECTION("C27", 2, "B1", 5) // Capacitance too large, messes up gfx. TODO: examine capacitor delay?
    CONNECTION("J1", 8, "B1", 5)



    // VR
    CONNECTION(V64_n, "K4", 13)
    CONNECTION(V128, "K4", 1)
    CONNECTION(V32, "K4", 2)

    CONNECTION("B7", 8, "A5", 10)
    CONNECTION("A5", 6, "A5", 9)

    CONNECTION("A5", 8, "A5", 5)
    CONNECTION("K4", 12, "A5", 4)



    // Score Counters
    CONNECTION(H64_n, "K4", 3)
    CONNECTION(HW, "K4", 4)
    CONNECTION("M7", 4, "K4", 5)

    CONNECTION("K4", 6, "N3", 9)
    CONNECTION("N3", 11, "N3", 10)

    CONNECTION("N3", 8, "N3", 12)
    CONNECTION(VSYNC_n, "N3", 13)

    CONNECTION("N3", 11, "M3", 8)
    CONNECTION(ATTRACT, "M3", 9)
     
    CONNECTION(GND, "K3", 6)
    CONNECTION(GND, "K3", 7)
    CONNECTION(START, "K3", 2)
    CONNECTION(START, "K3", 3)
    CONNECTION("M3", 10, "K3", 14)
    CONNECTION("K3", 12, "K3", 1)

    CONNECTION(VCC, "J3", 1)
    CONNECTION("K3", 11, "J3", 12)
    CONNECTION(VCC, "J3", 4)
    CONNECTION(START_n, "J3", 13)


    CONNECTION(HW, "N4", 10)
    CONNECTION(FN2, "N4", 9)

    CONNECTION("N4", 8, "N3", 5)
    CONNECTION("N3", 3, "N3", 4)

    CONNECTION("N3", 6, "N3", 2)
    CONNECTION(VSYNC_n, "N3", 1)

    CONNECTION("N3", 3, "M3", 12)
    CONNECTION(ATTRACT, "M3", 11)

    CONNECTION(GND, "L3", 6)
    CONNECTION(GND, "L3", 7)
    CONNECTION(START, "L3", 2)
    CONNECTION(START, "L3", 3)
    CONNECTION("M3", 13, "L3", 14)
    CONNECTION("L3", 12, "L3", 1)

    CONNECTION(VCC, "J3", 8)
    CONNECTION("L3", 11, "J3", 9)
    CONNECTION(VCC, "J3", 11)
    CONNECTION(START_n, "J3", 10)

    CONNECTION("J3", 3, "L7", 12)
    CONNECTION(H384, "L7", 13)

    CONNECTION("J3", 5, "L7", 4)
    CONNECTION(H384_n, "L7", 5)

    CONNECTION("L7", 11, "L7", 10)
    CONNECTION("L7", 6, "L7", 9)

    CONNECTION(H64, "K7", 11)
    CONNECTION(V16, "K7", 12)

    CONNECTION(BOUNDS, "J7", 1)
    CONNECTION(H128, "J7", 4)
    CONNECTION("K7", 13, "J7", 2)
    CONNECTION(H32, "J7", 5)

    CONNECTION(H16, "K7", 9)
    CONNECTION(H8, "K7", 8)

    CONNECTION("J7", 6, "M2", 11)

    CONNECTION("L7", 8, "J7", 10)
    CONNECTION("K7", 10, "J7", 12)
    CONNECTION(H4, "J7", 13)
    CONNECTION("M2", 10, "J7", 9)

    CONNECTION(H384_n, "L2", 1)
    CONNECTION("K3", 12, "L2", 2)
    CONNECTION("L3", 12, "L2", 3)
    CONNECTION("K3", 9, "L2", 5)
    CONNECTION("L3", 9, "L2", 6)
    CONNECTION("K3", 8, "L2", 14)
    CONNECTION("L3", 8, "L2", 13)
    CONNECTION("K3", 11, "L2", 11)
    CONNECTION("L3", 11, "L2", 10)
    CONNECTION(GND, "L2", 15)

    CONNECTION(H16, "K2", 4)
    CONNECTION("L2", 4, "K2", 7)
    CONNECTION("L2", 7, "K2", 1)
    CONNECTION("L2", 12, "K2", 2)
    CONNECTION("L2", 9, "K2", 6)
    CONNECTION(VCC, "K2", 5)
    CONNECTION(VCC, "K2", 3)

    CONNECTION("K2", 15, "J2", 1)
    CONNECTION(GND, "J2", 2)
    CONNECTION("K2", 9, "J2", 3)
    CONNECTION(GND, "J2", 4)
    CONNECTION(GND, "J2", 5)
    CONNECTION("K2", 12, "J2", 6)
    CONNECTION(GND, "J2", 7)
    CONNECTION("K2", 11, "J2", 9)
    CONNECTION(H4, "J2", 11)
    CONNECTION(V8, "J2", 12)
    CONNECTION(H8, "J2", 13)
    CONNECTION("J7", 6, "J2", 10)

    CONNECTION("K2", 13, "K1", 1)
    CONNECTION(GND, "K1", 2)
    CONNECTION(GND, "K1", 3)
    CONNECTION("K2", 14, "K1", 4)
    CONNECTION(GND, "K1", 5)
    CONNECTION(GND, "K1", 6)
    CONNECTION(GND, "K1", 7)
    CONNECTION("K2", 10, "K1", 9)
    CONNECTION(V2, "K1", 11)
    CONNECTION(V4, "K1", 12)
    CONNECTION(V8, "K1", 13)
    CONNECTION("J7", 6, "K1", 10)

    CONNECTION("J2", 14, "K4", 10)
    CONNECTION("K1", 14, "K4", 9)
    CONNECTION("J7", 8, "K4", 11)

    CONNECTION(V32, "A4", 4)
    CONNECTION("K4", 8, "A4", 5)

    CONNECTION("A4", 6, "CAPX", 1)



    // Coin / Attract
    CONNECTION("COIN", 1_NC, "H9", 13)
    CONNECTION(ATTRACT, "H9", 12)

    CONNECTION(VCC, "C29", 1) // Power on reset

    CONNECTION("C29", 2, "K9", 13)

    CONNECTION("K9", 12, "K9", 11)

    CONNECTION("K9", 12, "N8", 12)
    CONNECTION("H9", 11, "N8", 11)

    CONNECTION("N8", 13, "L8", 10)
    CONNECTION("H9", 11, "L8", 12)
    CONNECTION(V64, "L8", 11)
    CONNECTION(VCC, "L8", 13)

    CONNECTION("N8", 13, "L8", 4)
    CONNECTION("L8", 9, "L8", 2)
    CONNECTION(V64, "L8", 3)
    CONNECTION(VCC, "L8", 1)

    CONNECTION(ATTRACT_n, "N9", 4)
    CONNECTION(START_n, "N9", 2)

    CONNECTION("N9", 3, "N8", 2)
    CONNECTION("M8", 2, "N8", 3)

    CONNECTION("K9", 10, "M8", 13)
    CONNECTION("L8", 6, "M8", 1)
    CONNECTION(V64, "M8", 12)
    CONNECTION("N8", 1, "M8", 4)

    CONNECTION("N9", 3, "GNE", 1)



    // Video Summing
    CONNECTION(Q_BALL, "J1", 13)
    CONNECTION("D6", 10, "J1", 1)
    CONNECTION("D6", 13, "J1", 2)

    CONNECTION(GNE, "K8", 13)
    CONNECTION(F8, "K8", 12)

    CONNECTION(SCORE, "K8", 9)
    CONNECTION("K8", 11, "K8", 10)

    CONNECTION(H384, "N6", 4)
    CONNECTION(H64, "N6", 5)

    CONNECTION(FN1, "L4", 1)
    CONNECTION("N6", 6, "L4", 2)
    CONNECTION(ATTRACT_n, "L4", 4)
    CONNECTION(V32_n, "L4", 5)

    CONNECTION("K8", 8, "J8", 2)
    CONNECTION("M4", 8, "J8", 1)
    CONNECTION("L4", 6, "J8", 13)

    CONNECTION("J1", 12, "J8", 9)
    CONNECTION("N7", 8, "J8", 10)
    CONNECTION("J8", 12, "J8", 11)

    // HSYNC Hack
    CHIP("HSYNC_MOD", HSYNC_MOD)
    CONNECTION(HSYNC, "HSYNC_MOD", 1)
    CONNECTION(H32, "HSYNC_MOD", 2)

    CONNECTION("VIDEO", 1, "J8", 8)
    CONNECTION("VIDEO", 2, "D6", 1)
    CONNECTION("VIDEO", 3, "D6", 4)
    //CONNECTION("VIDEO", Video::HBLANK_PIN, HSYNC)
    CONNECTION("VIDEO", Video::HBLANK_PIN, "HSYNC_MOD", 3)
    CONNECTION("VIDEO", Video::VBLANK_PIN, VSYNC)



    // Sound
    CONNECTION(VW, "M7", 9)

    CONNECTION("B5", 11, "M7", 1)

    CONNECTION(VCC, "M8", 8)
    CONNECTION("M7", 8, "M8", 9)
    CONNECTION(GND, "M8", 11)
    CONNECTION("M7", 2, "M8", 10)

    CONNECTION(V8, "N8", 8)
    CONNECTION("M8", 5, "N8", 9)

    CONNECTION("A6", 10, "H7", 5)
    CONNECTION("A6", 6, "H7", 6)

    CONNECTION(V32, "H7", 2)
    CONNECTION("H7", 4, "H7", 3)

    CONNECTION("N8", 10, "N8", 5)
    CONNECTION("H7", 1, "N8", 6)

    CONNECTION("A7", 3, "J8", 4)
    CONNECTION(F8, "J8", 3)
    CONNECTION(V16, "J8", 5)


    CONNECTION(FIRE1_n, "M7", 13)

    CONNECTION("M7", 12, "FIRE_SOUND1", 1)
    CONNECTION(F2, "FIRE_SOUND1", 2)

    CONNECTION(VCC, "M9", 4)
    CONNECTION("FIRE_SOUND1", i3, "M9", i3)

    CONNECTION("M9", 3, "K9", 5)

    CONNECTION("K9", 6, "K8", 1)
    CONNECTION(FIRE1, "K8", 2)


    CONNECTION(FIRE2_n, "M7", 11)

    CONNECTION("M7", 10, "FIRE_SOUND2", 1)
    CONNECTION(F2, "FIRE_SOUND2", 2)

    CONNECTION(VCC, "L9", 4)
    CONNECTION("FIRE_SOUND2", i3, "L9", i3)

    CONNECTION("L9", 3, "K9", 3)

    CONNECTION(FIRE2, "K8", 5)
    CONNECTION("K9", 4, "K8", 4)

    CONNECTION("K8", 3, "J9", 1)
    CONNECTION("K8", 6, "J9", 2)
    CONNECTION("J8", 6, "J9", 4)
    CONNECTION("N8", 4, "J9", 5)

    CONNECTION(ATTRACT_n, "H9", 9)
    CONNECTION("J9", 6, "H9", 10)

    CONNECTION("H9", 8, "K9", 9)

    CONNECTION("K9", 8, "AUDIO", 1)

    CONNECTION("AUDIO", i1, "AUDIO", Audio::OUTPUT_MONO)




#ifdef DEBUG
    CONNECTION("LOG", 1, "C29", 2)
    CONNECTION("LOG", 2, "K9", 12)
    CONNECTION("LOG", 3, "K9", 10)
    CONNECTION("LOG", 4, "N9", 3)
    CONNECTION("LOG", 5, GNE)
    CONNECTION("LOG", 6, VSYNC)
    /*CONNECTION("LOG", 1, H32)
    CONNECTION("LOG", 2, H64)
    CONNECTION("LOG", 3, H128)
    CONNECTION("LOG", 4, H256)
    CONNECTION("LOG", 5, H384)
    CONNECTION("LOG", 6, "N5", 1)
    CONNECTION("LOG", 7, "??", 1)
    CONNECTION("LOG", 8, "L7", 3)
    CONNECTION("LOG", 9, "C36", 2)
    CONNECTION("LOG", 10, "M7", 4)
    CONNECTION("LOG", 11, HSYNC)
    CONNECTION("LOG", 12, FN2)*/


    /*CONNECTION("LOG", 2, "HSCAN_COMP2", 3)
    CONNECTION("LOG", 3, FIRE2)
    CONNECTION("LOG", 4, FN2)
    CONNECTION("LOG", 5, "H7", 10)
    CONNECTION("LOG", 6, "H6", 11)
    CONNECTION("LOG", 7, "F6", 11)
    CONNECTION("LOG", 8, "F6", 3)
    CONNECTION("LOG", 9, "H6", 3)
    CONNECTION("LOG", 10, GOAL1_n)
    CONNECTION("LOG", 11, "D6", 13)*/
#endif

CIRCUIT_LAYOUT_END


