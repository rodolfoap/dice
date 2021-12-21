/*	todo: 4 joysticks?
*/

/* ATTACK by Exidy */

#include "../circuit_desc.h"

#define DEBUG
#undef DEBUG

static Astable555Desc a5_555_desc(K_OHM(68.0), K_OHM(30.0), U_FARAD(1.2));
static Astable555Desc j3_555_desc(K_OHM(220.0), K_OHM(30.0), U_FARAD(1.2));
static Astable555Desc m4_555_desc(K_OHM(18.0), K_OHM(3.3), U_FARAD(33));
static Mono74123Desc l13_desc(K_OHM(82.0), U_FARAD(0.1), K_OHM(30.0), U_FARAD(0.22));

static Astable555Desc e13_555_desc(K_OHM(2500.0), K_OHM(0.33), U_FARAD(33.0));
static PotentimeterAstable555Desc pot1_desc("playtime", "Play Time", K_OHM(2500.0), K_OHM(1000.0), K_OHM(3000.0), e13_555_desc);

static SeriesRCDesc c27_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c28_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c36_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c37_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c38_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c57_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c31_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c39_desc(K_OHM(0.33), P_FARAD(330.0));
static SeriesRCDesc c42_desc(K_OHM(0.33), P_FARAD(330.0));

// Note - 2nd set of ROMs appear to be overdumped (size is 512 bytes and upper 256 bytes are all 0)
// 1st set is sized 256 bytes and is likely the correct dump
static RomDesc a1_desc("attack", "attack.a1", 0xD9B116B8, 0x5AFD5AFF);
static RomDesc b1_desc("attack", "attack.b1", 0x2317197F, 0x92D0FBF4);
static RomDesc c1_desc("attack", "attack.c1", 0x7391E44C, 0x25625D6E);
static RomDesc d1_desc("attack", "attack.d1", 0xD4A06439, 0x2FF8DD6B);
static RomDesc j6_desc("attack", "attack.j6", 0x1CE2921C, 0x21F87C1A);
static RomDesc k6_desc("attack", "attack.k6", 0xE120839F, 0xBA5115B3);

static MixerDesc mixer_desc({K_OHM(3.3), K_OHM(3.3)});

static AUDIO_DESC( attack )
    AUDIO_GAIN(15.0)
    AUDIO_SPEAKER_CONFIG(MONO)
AUDIO_DESC_END

static INPUT_DESC( attack )
    INPUT_INFO(JOYSTICK1_INPUT, {{ Joystick::LEFT, Joystick::RIGHT }}, "Move Ship")
    INPUT_INFO(JOYSTICK1_INPUT, {{ Joystick::UP, Joystick::DOWN }}, "Rotate Gun")
    INPUT_INFO(BUTTONS1_INPUT, {{ 1 }}, "Fire Gun")
    
    INPUT_INFO(JOYSTICK2_INPUT, {{ Joystick::LEFT, Joystick::RIGHT }}, "Rotate Plane")
    INPUT_INFO(BUTTONS2_INPUT, {{ 1 }}, "Fire Gun")
    INPUT_INFO(BUTTONS2_INPUT, {{ 2 }}, "Increase Speed")

    INPUT_INFO(COIN_INPUT, {{ 1 }}, "Insert Coin and Start Game")
INPUT_DESC_END

static VIDEO_DESC( attack )
    VIDEO_RESISTANCE(1, K_OHM(0.33))
    VIDEO_RESISTANCE(2, K_OHM(0.47))
    VIDEO_RESISTANCE(3, K_OHM(0.47))
    VIDEO_CONTRAST(1.5)
VIDEO_DESC_END

#ifdef DEBUG
static VcdLogDesc vcd_log_desc
(
    "output_attack.vcd",
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

CIRCUIT_LAYOUT( attack )

/* chips */

/* PAGE 1 */
CHIP("A1", 74S287, &a1_desc)  //1001
CHIP("A2", 74191)
CHIP("B1", 74S287, &b1_desc)  //1002
CHIP("B2", 74165)
CHIP("C1", 74S287, &c1_desc)  //1003
CHIP("C2", 7474)
CHIP("D1", 74S287, &d1_desc)  //1004
CHIP("D2", 74165)
/* PAGE 2 */
CHIP("A3", 7474)
CHIP("A4", 7474)
CHIP("A5", 555_Astable, &a5_555_desc)  
CHIP("B3", 7400)
CHIP("B4", 7474)
CHIP("C3", 7402)
CHIP("C4", 7474)
CHIP("D3", 7404)
CHIP("D4", 74191)
CHIP("C27", SERIES_RC, &c27_desc) 
CHIP("C28", SERIES_RC, &c28_desc) 
/* PAGE 3 */
CHIP("A6", 7402)
CHIP("A7", 7408)
CHIP("B5", 7451)
CHIP("B6", 7404)
CHIP("C5", 7451)
CHIP("C6", 7402)
CHIP("D5", 7474)
CHIP("D6", 7408)
CHIP("C36", SERIES_RC, &c36_desc) 
CHIP("C37", SERIES_RC, &c37_desc) 
CHIP("C38", SERIES_RC, &c38_desc) 
/* PAGE 4 */
CHIP("A8", 7402)
CHIP("A9", 7400)
CHIP("B7", 7408)
CHIP("B8", 7400)
CHIP("B9", 7474)
CHIP("C7", 7402)
CHIP("C8", 7402)
CHIP("C9", 7474)
CHIP("D7", 7495) 
CHIP("D8", 7400)
CHIP("D9", 7408)
CHIP("C57", SERIES_RC, &c57_desc) 
/* PAGE 5 */
CHIP("E5", 74191)
CHIP("E6", 7485)
CHIP("F5", 74191)
CHIP("F6", 7485)
CHIP("G5", 74191)
CHIP("G6", 7485)
CHIP("H5", 74191)
CHIP("H6", 7485)
/* PAGE 6 */
CHIP("E7", 74191)
CHIP("E8", 7485)
CHIP("F7", 74191)
CHIP("F8", 7485)
CHIP("G7", 74191)
CHIP("G8", 7485)
CHIP("H7", 74191)
CHIP("H8", 7485)
/* PAGE 7 */
CHIP("J1", 7474)
CHIP("J2", 7495)
CHIP("K1", 7474)
CHIP("K2", 7402)
CHIP("K3", 7400)
CHIP("L1", 7400)
CHIP("L2", 7408)
CHIP("L3", 7402)
CHIP("M1", 7408)
CHIP("M2", 7404)
CHIP("M3", 7402)
/* PAGE 8 */
CHIP("J3", 555_Astable, &j3_555_desc)  
CHIP("J4", 7404)
CHIP("J5", 74191)
CHIP("K4", 7400)
CHIP("K5", 7402)
CHIP("L4", 7474)
CHIP("L5", 7400)
CHIP("L6", 7402)
CHIP("M4", 555_Astable, &m4_555_desc)
CHIP("M5", 7474)
CHIP("M6", 7474)
CHIP("C31", SERIES_RC, &c31_desc) 
/* PAGE 9 */
CHIP("J6", 74S287, &j6_desc)  //1005
CHIP("J7", 74165)
CHIP("J8", 7404)
CHIP("K6", 74S287, &k6_desc)  //1006
CHIP("K7", 74165)
CHIP("K8", 7402)
CHIP("L7", 74191)
CHIP("L8", 7400)
CHIP("M7", 7474)
CHIP("M8", 7474)
CHIP("C39", SERIES_RC, &c39_desc) 
/* PAGE 10 */
CHIP("E1", 74191)
CHIP("E2", 7485)
CHIP("F1", 74191)
CHIP("F2", 7485)
CHIP("G1", 74191)
CHIP("G2", 7485)
CHIP("H1", 74191)
CHIP("H2", 7485)
/* PAGE 11 */
CHIP("E3", 74191)
CHIP("E4", 7485)
CHIP("F3", 74191)
CHIP("F4", 7485)
CHIP("G3", 7400)
CHIP("H3", 7402)
CHIP("H4", 7474)
/* PAGE 12 */
CHIP("CLOCK", CLOCK_8_MHZ)
CHIP("E9", 74191)
CHIP("F9", 74191)
CHIP("G9", 74191)
CHIP("G10", 7404)
CHIP("H9", 74191)
CHIP("H10", 7474)
/* PAGE 13 */
CHIP("J9", 7402)
CHIP("J10", 7400)
CHIP("J11", 7474)
CHIP("K9", 7410)
CHIP("K10", 7402)
CHIP("L9", 7432)
CHIP("L10", 7448)
CHIP("M9", 74151)
CHIP("M10", 74157)
/* PAGE 14 */
CHIP("K11", 74153)
CHIP("K12", 74153)
CHIP("L11", 74192)
CHIP("L12", 74192)
CHIP("M11", 74192)
CHIP("M12", 74192)
/* PAGE 15 */
CHIP("A10", 7410)
CHIP("A11", 7408)
CHIP("A12", 7408)
CHIP("B10", 7402)
CHIP("B11", 7400)
CHIP("C10", 7495)
CHIP("C11", 7402)
CHIP("D10", 7404)
CHIP("D11", 7495)
/* PAGE 16 */
CHIP("A13", 7496)
CHIP("B12", 7400)
CHIP("B13", 7496)
CHIP("C12", 7402)
CHIP("C13", 7496)
CHIP("D12", 7400)
CHIP("D13", 7496)
/* PAGE 17 */
CHIP("E11", 74191)
CHIP("E12", 7400)
CHIP("E13", 555_Astable, &e13_555_desc)
CHIP("F11", 74191)
CHIP("F12", 7404)
CHIP("G11", 7495)
CHIP("G12", 7400)
CHIP("H11", 7402)
CHIP("POT1", POT_555_ASTABLE, &pot1_desc)
POTENTIOMETER_CONNECTION("POT1", "E13") 
CHIP("C42", SERIES_RC, &c42_desc) 
//H12
/* PAGE 18 */
CHIP("J12", 7400)
CHIP("J13", 7400)
CHIP("K13", 7408)
CHIP("L13", 74123, &l13_desc)
CHIP("M13", 7402)

/* other */

//CHIP("LATCH", LATCH)
CHIP("COIN1", COIN_INPUT)
//CHIP("START1", START_INPUT)

CHIP("STICK1", JOYSTICK1_INPUT)
CHIP("STICK2", JOYSTICK2_INPUT)

CHIP("SHPFIRE", BUTTONS1_INPUT)
CHIP("PLNFIRE", BUTTONS2_INPUT)

CHIP("MIXER", MIXER, &mixer_desc)

VIDEO(attack)
AUDIO(attack)
INPUT(attack)

#ifdef DEBUG
	CHIP("LOG1", VCD_LOG, &vcd_log_desc)
#endif


/* Signals */

/* page 1 */
#define PLN_VID "C2", 5
#define PLN_VID_n "C2", 6
/* page 2 */
#define PVA "D4", 3
#define PVB "D4", 2
#define PVC "D4", 6
#define PVD "D4", 7
#define PLN_FAL "C4", 5
#define PLN_FAL_n "C4", 6
#define SET_PV_n "C3", 1
#define SET_PX_n "D3", 6
#define PY_POS "D5", 9
#define PY_POS_n "D5", 8
#define SET_PY_n "C4", 8
/* page 3 */
#define PLN_SPD "D5", 5
#define PLN_SPD_n "D5", 6
#define PX_CLK_n "C5", 8
#define PY_CLK_n "C5", 6
#define PY_UD_n "D3", 10
#define PY_ENB "A7", 3
#define PX_ENB "A7", 6
#define PX_UD_n "A6", 10
/* page 4 */
#define PBY_CLK_n "A8", 13
#define PBX_CLK_n "A8", 4
#define PBX_ENB_n "D9", 3
#define PBX_UD_n "D9", 6
#define PBY_ENB_n "D9", 8
#define PBY_UD_n "D9", 11
#define PLN_BF_n "A9", 8
#define PB_SET_n "A9", 11
#define PBV_ENB "C9", 9
/* page 5 */
#define PX_COM "F6", 6
#define PPX_1 "E5", 3
#define PPX_2 "E5", 2
#define PPX_3 "E5", 6
#define PPX_4 "E5", 7
#define PPX_5 "F5", 3
#define PPX_6 "F5", 2
#define PPX_7 "F5", 6
#define PPX_8 "F5", 7
#define PY_COM "H6", 6
#define PPY_1 "G5", 3
#define PPY_2 "G5", 2
#define PPY_3 "G5", 6
#define PPY_4 "G5", 7
#define PPY_5 "H5", 3
#define PPY_6 "H5", 2
#define PPY_7 "H5", 6 
#define PPY_8 "H5", 7
/* page 6 */
#define PBX_COM "F8", 6
#define PBY_COM "H8", 6
/* page 7 */
#define SBX_CLK_n "M3", 1
#define SBY_CLK_n "L3", 1
#define SBVA_n "M2", 2
#define SBVB_n "L2", 3
#define SBY_ENB_n "M2", 8
#define SBY_ENB "L1", 11
#define SBX_ENB_n "L2", 8
#define SBX_UD_n "L2", 11
#define SHP_BF_n "M2", 12
#define SB_SET_n "K3", 8
#define SBV_ENB "K1", 5
/* page 8 */
#define SV_A  "J5", 3
#define SV_B  "J5", 2
#define SV_C  "J5", 6
#define SV_D  "J5", 7
#define SS1_n  "M5", 9
#define SNK_SHP  "J4", 4
#define SNK_SHP_n  "K4", 11
#define DLY_SHP_n  "K4", 8
#define INT_SHP_n  "L6", 13
#define SHN_n  "M5", 6
/* page 9 */
#define SHP_VID "M7", 5
#define SHP_VID_n "M7", 6
#define SHP_Y "K8", 4
/* page 10 */
#define SBX_COM "F2", 6
#define SBY_COM "H2", 6
/* page 11 */
#define SX_COM "F4", 6
#define SPX1 "E3", 3  //diagram is wrong
#define SPX2 "E3", 2  //diagram is wrong
#define SPX3 "E3", 6  //diagram is wrong
#define SPX4 "E3", 7  //diagram is wrong
#define SPX5 "F3", 3  //diagram is wrong
#define SPX6 "F3", 2  //diagram is wrong
#define SPX7 "F3", 6  //diagram is wrong
#define SPX8 "F3", 7  //diagram is wrong
#define SME_n "G3",3 
/* page 12 */
#define X_16_CLK_n "E9", 13
#define XC_1 "E9", 3
#define XC_2 "E9", 2
#define XC_3 "E9", 6 
#define XC_4 "E9", 7 
#define XC_5 "F9", 3
#define XC_6 "F9", 2
#define XC_7 "F9", 6
#define XC_8 "F9", 7 
#define SLC_n "F9", 13
#define Y_16_CLK_n "G9", 13
#define YC_1 "G9", 3
#define YC_2 "G9", 2
#define YC_3 "G9", 6 
#define YC_4 "G9", 7
#define YC_5 "H9", 3
#define YC_6 "H9", 2
#define YC_7 "H9", 6 
#define YC_8 "H9", 7
#define VFC_n "H9", 13
#define SLC "G10", 8
#define VFC_A_n "G10", 6
#define VFC "G10", 4
#define BIT_CLK_n "H10", 8
#define BIT_CLK "H10", 9
/* page 13 */
#define SCR_VID "M9", 5
#define SCR_A "J11", 5
#define SCR_B "J11", 9 
#define XLN "J10", 8
/* page 14 */
#define SCR_DA "K11", 7
#define SCR_DB "K11", 9
#define SCR_DC "K12", 7
#define SCR_DD "K12", 9
/* page 15 */
#define PMV_n "C11", 1
#define SBV_n "A10", 8
#define BLK "D12", 8
#define RS "C10", 13
#define LS "B10", 13
#define BLK_n "A11", 6
#define V_SYN "D11", 12
#define V_SYN_n "D10", 12
/* page 16 */
#define SHP_HIT_n "D12", 3
#define PVB_n "B12", 3
#define PV6_n "B12", 6  //named *p6v on page 16
#define END_SB_n "C12", 13
#define END_PB_n "A12", 3
#define PBV_X "A13", 15
#define PBV_Y "B13", 15
#define SBV_X "C13", 15
#define SBV_Y "D13", 15
#define SLB_n "B12", 11
/* page 17 */
#define VCK1 "E11", 3
#define VCK2 "E11", 2
#define VCK3 "E11", 6
#define VCK4 "E11", 7
#define VCK5 "F11", 3
#define VCK6 "F11", 2
#define CLK1 "H11", 1
#define CLK2 "H11", 13
#define CLK3 "H11", 10
#define CLK4 "H11", 4
#define POR_n "F12", 4
#define CLK_SCR "E12", 11
#define CON_LTH_n "F12", 12
#define CON_LTH "F12", 8
/* user input */
#define SML_n "STICK1", Joystick::LEFT  //ship move left
#define SMR_n "STICK1", Joystick::RIGHT //ship move right
#define SHP_GR_n "STICK1", Joystick::UP    //ship gun right
#define SHP_GL_n "STICK1", Joystick::DOWN  //ship gun left
#define SBF_NO_n "SHPFIRE", 1 //ship button fire
#define SBF_NC   "SHPFIRE", i1 //ship button fire
#define PBF_NO_n "PLNFIRE", 1 //plane button fire
#define PBF_NC_n "PLNFIRE", i1 //plane button fire ?
#define PLN_SPD_FST_n "PLNFIRE", 2 //plane speed fast
#define PLN_RCCW_n "STICK2", Joystick::LEFT  //plane rotate counter-clockwise
#define PLN_RCW_n  "STICK2", Joystick::RIGHT //plane rotate clockwise


/* Layout */
//from (output pins) - to (input pins)

/* page 1 - plane display*/

// 1001
CONNECTION(PVD, "A1", 15)
CONNECTION(PVC, "A1", 1)
CONNECTION(PVB, "A1", 2)
CONNECTION(PVA, "A1", 3)
CONNECTION("A2", 7, "A1", 4)
CONNECTION("A2", 6, "A1", 7)
CONNECTION("A2", 2, "A1", 6)
CONNECTION("A2", 3, "A1", 5)

// 1002
CONNECTION(PVD, "B1", 15)
CONNECTION(PVC, "B1", 1)
CONNECTION(PVB, "B1", 2)
CONNECTION(PVA, "B1", 3)
CONNECTION("A2", 7, "B1", 4)
CONNECTION("A2", 6, "B1", 7)
CONNECTION("A2", 2, "B1", 6)
CONNECTION("A2", 3, "B1", 5)

// 1003
CONNECTION(PVD, "C1", 15)
CONNECTION(PVC, "C1", 1)
CONNECTION(PVB, "C1", 2)
CONNECTION(PVA, "C1", 3)
CONNECTION("A2", 7, "C1", 4)
CONNECTION("A2", 6, "C1", 7)
CONNECTION("A2", 2, "C1", 6)
CONNECTION("A2", 3, "C1", 5)

// 1004
CONNECTION(PVD, "D1", 15)
CONNECTION(PVC, "D1", 1)
CONNECTION(PVB, "D1", 2)
CONNECTION(PVA, "D1", 3)
CONNECTION("A2", 7, "D1", 4)
CONNECTION("A2", 6, "D1", 7)
CONNECTION("A2", 2, "D1", 6)
CONNECTION("A2", 3, "D1", 5)

CONNECTION(VCC, "A4", 4)
CONNECTION(VCC, "A4", 2)
CONNECTION(PY_COM, "A4", 3)
CONNECTION("A2", 13, "A4", 1)

CONNECTION(SLC, "B3", 1)
CONNECTION("A4", 5, "B3", 2)

CONNECTION("B3", 3, "A2", 14)
CONNECTION(GND, "A2", 4)
CONNECTION(VCC, "A2", 11)
CONNECTION(GND, "A2", 5)

CONNECTION(BIT_CLK_n, "C3", 9)
CONNECTION("C2", 8, "C3", 8)

CONNECTION(POR_n, "C2", 4)
CONNECTION("D2", 9 , "C2", 2)
CONNECTION("C3", 10, "C2", 3)
CONNECTION("C2", 9, "C2", 1)

CONNECTION(VCC, "C2", 10)
CONNECTION(VCC, "C2", 12)
CONNECTION(PX_COM, "C2", 11)
CONNECTION("B3", 3, "C2", 13)

CONNECTION("A1", 9, "B2", 11)
CONNECTION("A1", 10, "B2", 12)
CONNECTION("A1", 11, "B2", 13)
CONNECTION("A1", 12, "B2", 14)
CONNECTION("B1", 9, "B2", 3)
CONNECTION("B1", 10, "B2", 4)
CONNECTION("B1", 11, "B2", 5)
CONNECTION("B1", 12, "B2", 6)
CONNECTION("B3", 3, "B2", 1)
CONNECTION("C3", 10, "B2", 2)
CONNECTION(GND, "B2", 10)
CONNECTION(GND, "B2", 15)

CONNECTION("C1", 9, "D2", 11)
CONNECTION("C1", 10, "D2", 12)
CONNECTION("C1", 11, "D2", 13)
CONNECTION("C1", 12, "D2", 14)
CONNECTION("D1", 9, "D2", 3)
CONNECTION("D1", 10, "D2", 4)
CONNECTION("D1", 11, "D2", 5)
CONNECTION("D1", 12, "D2", 6)
CONNECTION("B3", 3, "D2", 1)
CONNECTION("C3", 10, "D2", 2)
CONNECTION("B2", 9, "D2", 10)
CONNECTION(GND, "D2", 15)


/* page 2 - plane vector count */

CONNECTION(VCC, "A3", 4)
CONNECTION(PLN_RCCW_n, "A3", 2)  //IO PIN 16
CONNECTION(V_SYN_n, "A3", 3)
CONNECTION(VCC, "A3", 1)

CONNECTION(VCC, "A3", 10)
CONNECTION(PLN_RCW_n, "A3", 12)  // IO PIN 17
CONNECTION(V_SYN_n, "A3", 11)
CONNECTION(VCC, "A3", 13)

CONNECTION("A3", 6, "C3", 5)
CONNECTION("A3", 8, "C3", 6)

CONNECTION("C3", 4, "D3", 13)

CONNECTION("D3", 12, "A5", 4)

CONNECTION(VCC, "B4", 10)
CONNECTION("A5", 3, "B4", 12)
CONNECTION(V_SYN, "B4", 11)
CONNECTION(VCC, "B4", 13)

CONNECTION(VCC, "B4", 4)
CONNECTION("B4", 6, "B4", 2)
CONNECTION("B4", 9, "B4", 3)
CONNECTION("D3", 12, "B4", 1)

CONNECTION("B4", 5, "B3", 4)
CONNECTION(PLN_SPD, "B3", 5)

CONNECTION("B4", 9, "B3", 12)
CONNECTION(PLN_SPD_n, "B3", 13)

CONNECTION("B3", 6, "B3", 9)
CONNECTION("B3", 11, "B3", 10)

CONNECTION("B3", 8, "C28", 1)
CONNECTION(GND, "C28", 2)
CONNECTION("C28", 3, "D3", 9)

CONNECTION(VCC, "C4", 4)
CONNECTION(VCC, "C4", 2)
CONNECTION(PMV_n, "C4", 3)
CONNECTION(PVB_n, "C4", 1)

CONNECTION(PVB_n, "C4", 10)
CONNECTION(CON_LTH_n, "C4", 12)
CONNECTION(PV6_n, "C4", 11)
CONNECTION(VCC, "C4", 13)

CONNECTION("C4", 5, "C3", 2)
CONNECTION("C4", 9, "C3", 3)  //wrong pin on schematic

CONNECTION("C4", 9, "C27", 1)
CONNECTION(GND, "C27", 2)
CONNECTION("C27", 3, "D3", 5)

CONNECTION("C4", 6, "A4", 10)
CONNECTION(VCK5, "A4", 12)
CONNECTION("D3", 6, "A4", 11)
CONNECTION(VCC, "A4", 13)

CONNECTION(VCC, "D5", 10)
CONNECTION(VCK6, "D5", 12)
CONNECTION("D3", 6, "D5", 11)
CONNECTION(VCC, "D5", 13)

CONNECTION("A3", 6, "D4", 5)
CONNECTION("C3", 4, "D4", 4)
CONNECTION("D3", 8, "D4", 14)
CONNECTION("C3", 1, "D4", 11)
CONNECTION(GND, "D4", 15)
CONNECTION(GND, "D4", 1)
CONNECTION("C4", 5, "D4", 10)
CONNECTION("A4", 9, "D4", 9)


/* page 3 - plane vector control*/

CONNECTION(CLK1, "D3", 1)

CONNECTION(VCC, "D5", 4)
CONNECTION(PLN_SPD_FST_n, "D5", 2)   //IO PIN 15
CONNECTION("D3", 2, "D5", 3)
CONNECTION(SET_PV_n, "D5", 1)

CONNECTION(VCK3, "C36", 1)
CONNECTION(VCK1, "C38", 1)
CONNECTION(VCK4, "C37", 1)
CONNECTION(GND, "C36", 2)
CONNECTION(GND, "C38", 2)
CONNECTION(GND, "C37", 2)

CONNECTION(CLK1, "B5", 1)
CONNECTION("D5", 6, "B5", 13)
CONNECTION("C36", 3, "B5", 10)
CONNECTION("D5", 5, "B5", 9)

CONNECTION("C38", 3, "B5", 3)
CONNECTION("D5", 6, "B5", 2)
CONNECTION("C37", 3, "B5", 4)
CONNECTION("D5", 5, "B5", 5)

CONNECTION("B5", 8, "B6", 5)

CONNECTION("B5", 6, "B6", 1)

CONNECTION(PVC, "B6", 9)

CONNECTION(PVD, "D3", 11)

CONNECTION(PVB, "C6", 2)
CONNECTION("B6", 8, "C6", 3)

CONNECTION(PVB, "D6", 1)
CONNECTION(PVA, "D6", 2)

CONNECTION("C6", 1, "C6", 5)
CONNECTION("D6", 6, "C6", 6)

CONNECTION("B6", 8, "D6", 4)
CONNECTION("D6", 3, "D6", 5)

CONNECTION("C6", 4, "B6", 3)  //wrong on schematic

CONNECTION(PVA, "A6", 2)
CONNECTION(PVB, "A6", 3)

CONNECTION("B6", 8, "B6", 13)

CONNECTION("B6", 8, "A7", 1)
CONNECTION("A6", 1, "A7", 2)

CONNECTION("A6", 1, "A7", 4)
CONNECTION(PVC, "A7", 5)

CONNECTION("D3", 10, "A7", 9)
CONNECTION(PVC, "A7", 10)

CONNECTION("D3", 10, "A6", 5)
CONNECTION(PVC, "A6", 6)

CONNECTION("A7", 8, "A6", 8)
CONNECTION("A6", 4, "A6", 9)

CONNECTION("B6", 12, "D6", 9)  //? GUESS D6 
CONNECTION(PVB, "D6", 10)      //? GUESS D6 8,9,10

CONNECTION(PVA, "D6", 12)
CONNECTION("D6", 8, "D6", 13)  //? guess d6,8

CONNECTION("B6", 12, "C6", 8)
CONNECTION(PVB, "C6", 9)

CONNECTION("C6", 10, "C6", 12)
CONNECTION("D6", 11, "C6", 11)

CONNECTION("C6", 13, "B6", 11)

CONNECTION("B6", 6, "C5", 13)
CONNECTION("C6", 4, "C5", 1)
CONNECTION("B6", 2, "C5", 10)
CONNECTION("B6", 4, "C5", 9)  //wrong on schematic

CONNECTION("B6", 6, "C5", 2)
CONNECTION("C6", 13, "C5", 3)
CONNECTION("B6", 2, "C5", 4)
CONNECTION("B6", 10, "C5", 5)


/* page 4 - plane bullet vector */

CONNECTION(VCC, "D7", 6)
CONNECTION(PVD, "D7", 5)
CONNECTION(PVC, "D7", 4)
CONNECTION(PVB, "D7", 3)
CONNECTION(PVA, "D7", 2)
CONNECTION("A9", 11, "D7", 8)

CONNECTION("D7", 11, "C7", 2)
CONNECTION("D7", 12, "C7", 3)

CONNECTION("D7", 12, "B7", 1)
CONNECTION("D7", 11, "B7", 2)

CONNECTION("B8", 6, "D8", 1)
CONNECTION("D7", 12, "D8", 2)

CONNECTION("D7", 13, "B8", 4)
CONNECTION("D7", 13, "B8", 5)

CONNECTION("D7", 13, "C7", 5)
CONNECTION("D7", 12, "C7", 6)

CONNECTION("D7", 11, "C7", 8)
CONNECTION("D7", 10, "C7", 9)

CONNECTION("D7", 11, "B7", 9)
CONNECTION("D7", 10, "B7", 10)

CONNECTION("B8", 6, "D8", 9)
CONNECTION("C7", 1, "D8", 10)

CONNECTION("D7", 10, "B8", 1)
CONNECTION("D7", 10, "B8", 2)

CONNECTION("C7", 1, "C8", 2)
CONNECTION("B7", 3, "C8", 3)

CONNECTION("D8", 3, "D8", 4)
CONNECTION(CLK4, "D8", 5)

CONNECTION("D7", 11, "B7", 4)
CONNECTION("C7", 4, "B7", 5)  //wrong pin on diagram

CONNECTION("C7", 10, "C7", 11)
CONNECTION("B7", 8, "C7", 12)

CONNECTION("D8", 8, "D10", 9)

CONNECTION("C7", 13, "D10", 5)

CONNECTION(CLK3, "D10", 11)

CONNECTION("C8", 1, "C8", 5)
CONNECTION("C8", 1, "C8", 6)

CONNECTION("D10", 10, "B8", 13)
CONNECTION("D8", 6, "B8", 12)

CONNECTION("C8", 1, "B8", 9)  //b8 not labeled on diagram
CONNECTION("D8", 3, "B8", 10) //b8 not labeled on diagram

CONNECTION("C8", 4, "D8", 12)
CONNECTION("D8", 3, "D8", 13)

CONNECTION("D8", 11, "A7", 12)
CONNECTION("B8", 11, "A7", 13)

CONNECTION("B8", 11, "B7", 12)
CONNECTION("B8", 8, "B7", 13)

CONNECTION(CLK2, "C8", 8)
CONNECTION("A7", 11, "C8", 9)

CONNECTION(CLK2, "C8", 12)  //wrong pin on scematic
CONNECTION("B7", 11, "C8", 11)

CONNECTION(YC_5, "C57", 1)
CONNECTION(GND, "C57", 2)

CONNECTION("C57", 3, "J8", 9)

CONNECTION("J8", 8, "A6", 11)
CONNECTION("A9", 8, "A6", 12)

CONNECTION("C8", 10, "A8", 8)
CONNECTION("C9", 8, "A8", 9)

CONNECTION("C8", 13, "A8", 2)
CONNECTION("C9", 8, "A8", 3)

CONNECTION("A6", 13, "A8", 11)
CONNECTION("A8", 10, "A8", 12)

CONNECTION("A6", 13, "A8", 5)
CONNECTION("A8", 1, "A8", 6)

CONNECTION("B7", 6, "D9", 1)
CONNECTION("A9", 8, "D9", 2)

CONNECTION("A9", 8, "D9", 4)
CONNECTION("D10", 6, "D9", 5)

CONNECTION("D10", 8, "D9", 9)
CONNECTION("A9", 8, "D9", 10)

CONNECTION("A9", 8, "D9", 12)
CONNECTION("B8", 3, "D9", 13)

CONNECTION(PBF_NC_n, "A9", 1)  //IO PIN 14
CONNECTION("A9", 6, "A9", 2)

CONNECTION("A9", 3, "A9", 4)
CONNECTION(PBF_NO_n, "A9", 5)  //IO PIN 13

CONNECTION(VCC, "B9", 4)
CONNECTION(CON_LTH, "B9", 2)
CONNECTION("A9", 6, "B9", 3)
CONNECTION(END_PB_n, "B9", 1)

CONNECTION(VCC, "B9", 10)
CONNECTION("B9", 5, "B9", 12)
CONNECTION(VFC_n, "B9", 11)
CONNECTION(END_PB_n, "B9", 13)

CONNECTION(VCC, "C9", 4)
CONNECTION("B9", 9, "C9", 2)
CONNECTION(VFC_n, "C9", 3)
CONNECTION(END_PB_n, "C9", 1)

CONNECTION(VCC, "C9", 10)
CONNECTION("C9", 5, "C9", 12)
CONNECTION(VFC_n, "C9", 11)
CONNECTION(END_PB_n, "C9", 13)

CONNECTION("B9", 9, "A9", 10)
CONNECTION("C9", 6, "A9", 9)

CONNECTION("B9", 8, "A9", 12)
CONNECTION(VFC, "A9", 13)


/* page 5 - plane count */

CONNECTION(BIT_CLK, "E6", 3)
CONNECTION(XC_1, "E6", 15)
CONNECTION(XC_2, "E6", 13)
CONNECTION(XC_3, "E6", 12)
CONNECTION(XC_4, "E6", 10)
CONNECTION("E5", 3, "E6", 1)
CONNECTION("E5", 2, "E6", 14)
CONNECTION("E5", 6, "E6", 11)
CONNECTION("E5", 7, "E6", 9)

CONNECTION(PX_UD_n, "E5", 5)
CONNECTION(PX_ENB, "E5", 4)
CONNECTION(PX_CLK_n, "E5", 14)
CONNECTION(SET_PX_n, "E5", 11)
CONNECTION(VCC, "E5", 15)
CONNECTION(VCC, "E5", 1)
CONNECTION(GND, "E5", 10)
CONNECTION(VCC, "E5", 9)

CONNECTION("E6", 6, "F6", 3)
CONNECTION("F5", 3, "F6", 1)
CONNECTION("F5", 2, "F6", 14)
CONNECTION("F5", 6, "F6", 11)
CONNECTION("F5", 7, "F6", 9)
CONNECTION(XC_5, "F6", 15)
CONNECTION(XC_6, "F6", 13)
CONNECTION(XC_7, "F6", 12)
CONNECTION(XC_8, "F6", 10)

CONNECTION(PX_UD_n, "F5", 5)
CONNECTION(PX_ENB, "F5", 4)

CONNECTION("E5", 13, "F5", 14)
CONNECTION(SET_PX_n, "F5", 11)
CONNECTION(VCC, "F5", 15)
CONNECTION(GND, "F5", 1)
CONNECTION(GND, "F5", 10)
CONNECTION(GND, "F5", 9)

CONNECTION(SLC, "G6", 3)
CONNECTION("G5", 3, "G6", 1)
CONNECTION("G5", 2, "G6", 14)
CONNECTION("G5", 6, "G6", 11)
CONNECTION("G5", 7, "G6", 9)
CONNECTION(YC_1, "G6", 15)
CONNECTION(YC_2, "G6", 13)
CONNECTION(YC_3, "G6", 12)
CONNECTION(YC_4, "G6", 10)

CONNECTION(PY_UD_n, "G5", 5)
CONNECTION(PY_ENB, "G5", 4)
CONNECTION(PY_CLK_n, "G5", 14)
CONNECTION(SET_PY_n, "G5", 11)
CONNECTION(VCC, "G5", 15)
CONNECTION(VCC, "G5", 1)
CONNECTION(VCC, "G5", 10)
CONNECTION(GND, "G5", 9)

CONNECTION("G6", 6, "H6", 3)
CONNECTION("H5", 3, "H6", 1)
CONNECTION("H5", 2, "H6", 14)
CONNECTION("H5", 6, "H6", 11)
CONNECTION("H5", 7, "H6", 9)
CONNECTION(YC_5, "H6", 15)
CONNECTION(YC_6, "H6", 13)
CONNECTION(YC_7, "H6", 12)
CONNECTION(YC_8, "H6", 10)

CONNECTION(PY_UD_n, "H5", 5)
CONNECTION(PY_ENB, "H5", 4)
CONNECTION("G5", 13, "H5", 14)
CONNECTION(SET_PY_n, "H5", 11)
CONNECTION(VCC, "H5", 15)
CONNECTION(PY_POS_n, "H5", 1)
CONNECTION(PY_POS, "H5", 10)
CONNECTION(PY_POS_n, "H5", 9)


/* page 6 - plane bullet count */

CONNECTION(BIT_CLK, "E8", 3)
CONNECTION("E7", 3, "E8", 1)
CONNECTION("E7", 2, "E8", 14)
CONNECTION("E7", 6, "E8", 11)
CONNECTION("E7", 7, "E8", 9)
CONNECTION(XC_1, "E8", 15)
CONNECTION(XC_2, "E8", 13)
CONNECTION(XC_3, "E8", 12)
CONNECTION(XC_4, "E8", 10)

CONNECTION(PBX_UD_n, "E7", 5)
CONNECTION(PBX_ENB_n, "E7", 4)
CONNECTION(PBX_CLK_n, "E7", 14)
CONNECTION(PB_SET_n, "E7", 11)
CONNECTION(PPX_1, "E7", 15)
CONNECTION(PPX_2, "E7", 1)
CONNECTION(PPX_3, "E7", 10)
CONNECTION(PPX_4, "E7", 9)

CONNECTION("E8", 6, "F8", 3)
CONNECTION("F7", 3, "F8", 1)
CONNECTION("F7", 2, "F8", 14)
CONNECTION("F7", 6, "F8", 11)
CONNECTION("F7", 7, "F8", 9)
CONNECTION(XC_5, "F8", 15)
CONNECTION(XC_6, "F8", 13)
CONNECTION(XC_7, "F8", 12)
CONNECTION(XC_8, "F8", 10)

CONNECTION(PBX_UD_n, "F7", 5)
CONNECTION(PBX_ENB_n, "F7", 4)
CONNECTION("E7", 13, "F7", 14)
CONNECTION(PB_SET_n, "F7", 11)
CONNECTION(PPX_5, "F7", 15)
CONNECTION(PPX_6, "F7", 1)
CONNECTION(PPX_7, "F7", 10)
CONNECTION(PPX_8, "F7", 9)

CONNECTION(SLC, "G8", 3)
CONNECTION("G7", 3, "G8", 1)
CONNECTION("G7", 2, "G8", 14)
CONNECTION("G7", 6, "G8", 11)
CONNECTION("G7", 7, "G8", 9)
CONNECTION(YC_1, "G8", 15)
CONNECTION(YC_2, "G8", 13)
CONNECTION(YC_3, "G8", 12)
CONNECTION(YC_4, "G8", 10)

CONNECTION(PBY_UD_n, "G7", 5)
CONNECTION(PBY_ENB_n, "G7", 4)
CONNECTION(PBY_CLK_n, "G7", 14)
CONNECTION(PB_SET_n, "G7", 11)
CONNECTION(PPY_1, "G7", 15)
CONNECTION(PPY_2, "G7", 1)
CONNECTION(PPY_3, "G7", 10)
CONNECTION(PPY_4, "G7", 9)

CONNECTION("G8", 6, "H8", 3)
CONNECTION("H7", 3, "H8", 1)
CONNECTION("H7", 2, "H8", 14)
CONNECTION("H7", 6, "H8", 11)
CONNECTION("H7", 7, "H8", 9)
CONNECTION(YC_5, "H8", 15)
CONNECTION(YC_6, "H8", 13)
CONNECTION(YC_7, "H8", 12)
CONNECTION(YC_8, "H8", 10)

CONNECTION(PBY_UD_n, "H7", 5)
CONNECTION(PBY_ENB_n, "H7", 4)
CONNECTION("G7", 13, "H7", 14)
CONNECTION(PB_SET_n, "H7", 11)
CONNECTION(PPY_5, "H7", 15)
CONNECTION(PPY_6, "H7", 1)
CONNECTION(PPY_7, "H7", 10)
CONNECTION(PPY_8, "H7", 9)


/* page 7 - ship bullet vector */

CONNECTION(SBF_NC, "K3", 1)  //IO PIN12
CONNECTION("K3", 6, "K3", 2)

CONNECTION("K3", 3, "K3", 4)
CONNECTION(SBF_NO_n, "K3", 5)  //IO PIN 11

CONNECTION(SS1_n, "K5", 11)
CONNECTION(CON_LTH_n, "K5", 12)

CONNECTION(VCC, "J1", 10)
CONNECTION("K5", 13, "J1", 12)
CONNECTION("K3", 6, "J1", 11)
CONNECTION(END_SB_n, "J1", 13)

CONNECTION(VCC, "J1", 4)
CONNECTION("J1", 9, "J1", 2)
CONNECTION(VFC_A_n, "J1", 3)
CONNECTION(END_SB_n, "J1", 1)

CONNECTION(VCC, "K1", 4)
CONNECTION("J1", 5, "K1", 2)
CONNECTION(VFC_A_n, "K1", 3)
CONNECTION(END_SB_n, "K1", 1)

CONNECTION("J1", 5, "M1", 13)
CONNECTION("K1", 6, "M1", 12)

CONNECTION("M1", 11, "M2", 13)

CONNECTION(VFC, "K3", 9)
CONNECTION("J1", 6, "K3", 10)

CONNECTION(VCC, "J2", 6)
CONNECTION(SV_D, "J2", 5)
CONNECTION(SV_C, "J2", 4)
CONNECTION(SV_B, "J2", 3)
CONNECTION(SV_A, "J2", 2)
CONNECTION("K3", 8, "J2", 8)

CONNECTION("J2", 13, "K2", 5)
CONNECTION("J2", 12, "K2", 6)

CONNECTION("J2", 11, "K2", 8)
CONNECTION("J2", 10, "K2", 9)

CONNECTION("K2", 4, "L1", 4)
CONNECTION("K2", 10, "L1", 5)

CONNECTION("K2", 4, "L1", 9)
CONNECTION("J2", 11, "L1", 10)

CONNECTION("J2", 11, "L3", 11)
CONNECTION("J2", 10, "L3", 12)

CONNECTION("L1", 8, "J4", 13)

CONNECTION("L3", 13, "M2", 11)

CONNECTION("J4", 12, "L2", 9)
CONNECTION("M2", 12, "L2", 10)

CONNECTION("M2", 12, "L2", 12)
CONNECTION("M2", 10, "L2", 13)  //WRONG PIN ON DIAGRAM?

CONNECTION("J2", 10, "M2", 5)

CONNECTION("M2", 6, "L1", 13)
CONNECTION("L1", 6, "L1", 12)


CONNECTION("L1", 11, "M2", 9)

CONNECTION("J2", 12, "M1", 1)
CONNECTION("J2", 11, "M1", 2)

CONNECTION("J2", 12, "K2", 2)
CONNECTION("J2", 11, "K2", 3)

CONNECTION("K2", 1, "K2", 12)
CONNECTION("M1", 3, "K2", 11)

CONNECTION("J2", 13, "M2", 1)

CONNECTION("J2", 12, "L1", 1)
CONNECTION("M2", 2, "L1", 2)

CONNECTION(CLK4, "M1", 4)
CONNECTION("L1", 3, "M1", 5)

CONNECTION("L1", 3, "L2", 1)
CONNECTION("K2", 13, "L2", 2)

CONNECTION(CLK3, "L3", 8)
CONNECTION("M1", 6, "L3", 9)

CONNECTION("L2", 3, "M2", 3)

CONNECTION("M2", 4, "L2", 4)
CONNECTION("L1", 3, "L2", 5)

CONNECTION("L3", 10, "L3", 5)
CONNECTION("L2", 6, "L3", 6)

CONNECTION(CLK2, "L3", 2)
CONNECTION("L3", 4, "L3", 3)

CONNECTION("L3", 10, "M3", 5)
CONNECTION("L2", 3, "M3", 6)

CONNECTION(CLK2, "M3", 8)
CONNECTION("M3", 4, "M3", 9)

CONNECTION("M3", 10, "M3", 11)
CONNECTION("K1", 6, "M3", 12)

CONNECTION(CLK4, "M1", 9)
CONNECTION("M1", 11, "M1", 10)

CONNECTION("M1", 8, "M3", 2)
CONNECTION("M3", 13, "M3", 3)


/* page 8 - ship vector control */

CONNECTION("H4", 8, "J3", 4)

CONNECTION("J3", 3, "C31", 1)
CONNECTION(GND, "C31", 2)
CONNECTION("C31", 3, "J4", 9)


CONNECTION(LS, "K4", 5)
CONNECTION(SHP_VID, "K4", 4)

CONNECTION(XLN, "K4", 2)
CONNECTION(SHP_VID, "K4", 1)

CONNECTION("K4", 6, "L4", 4)
CONNECTION(GND, "L4", 2)
CONNECTION("K4", 3, "L4", 3)
CONNECTION(VCC, "L4", 1)

CONNECTION("M5", 5, "M4", 4)

CONNECTION("M5", 5, "L5", 2)
CONNECTION("M4", 3, "L5", 1)

CONNECTION(SHP_HIT_n, "M5", 4)
CONNECTION("M6", 8, "M5", 2)
CONNECTION("L5", 3, "M5", 3)
CONNECTION(CON_LTH, "M5", 1)

CONNECTION("M5", 6, "L5", 4)
CONNECTION(CON_LTH, "L5", 5)

CONNECTION(CON_LTH, "M5", 10)
CONNECTION("M5", 5, "M5", 12)
CONNECTION("L5", 3, "M5", 11)
CONNECTION("L5", 6, "M5", 13)

CONNECTION(CON_LTH, "M6", 4)
CONNECTION("M5", 9, "M6", 2)
CONNECTION("L5", 3, "M6", 3)
CONNECTION("L5", 6, "M6", 1)

CONNECTION(CON_LTH, "M6", 10)
CONNECTION("M6", 5, "M6", 12)
CONNECTION("L5", 3, "M6", 11)
CONNECTION("L5", 6, "M6", 13)

CONNECTION("L4", 5, "L6", 11)
CONNECTION("M6", 9, "L6", 12)

CONNECTION("M6", 8, "K4", 9)
CONNECTION("M6", 5, "K4", 10)

CONNECTION("M4", 3, "L6", 8)
CONNECTION(CON_LTH_n, "L6", 9)

CONNECTION("M5", 5, "K4", 12)
CONNECTION("M6", 6, "K4", 13)

CONNECTION("K4", 11, "J4", 3)

CONNECTION("J4", 2, "L6", 3)
CONNECTION(SHP_GL_n, "L6", 2)  //IO PIN 10

CONNECTION(SHP_GR_n, "L6", 5)  //IO PIN 9
CONNECTION("J5", 7, "L6", 6)

CONNECTION("L6", 1, "K5", 5)
CONNECTION("L6", 4, "K5", 6)

CONNECTION(VCC, "H4", 4)
CONNECTION("L6", 1, "H4", 2)
CONNECTION(V_SYN_n, "H4", 3)
CONNECTION(VCC, "H4", 1)

CONNECTION(VCC, "H4", 10)
CONNECTION("K5", 4, "H4", 12)
CONNECTION(V_SYN_n, "H4", 11)
CONNECTION(VCC, "H4", 13)

CONNECTION("H4", 5, "J5", 5)
CONNECTION("H4", 9, "J5", 4)
CONNECTION("J4", 8, "J5", 14)
CONNECTION("M5", 8, "J5", 15)
CONNECTION("M5", 9, "J5", 1)
CONNECTION("M6", 9, "J5", 10)
CONNECTION("M6", 8, "J5", 9)
CONNECTION("L6", 10, "J5", 11)

CONNECTION("J5", 3, "K5", 3)
CONNECTION("J5", 2, "K5", 2)

CONNECTION("J5", 6, "K5", 8)
CONNECTION("J5", 7, "K5", 9)

CONNECTION("K5", 1, "L5", 9)
CONNECTION("K5", 10, "L5", 10)

CONNECTION("L5", 8, "J4", 1)


/* PAGE 9 - ship display */

CONNECTION(YC_4, "L8", 1)
CONNECTION(YC_6, "L8", 2)

CONNECTION(YC_7, "L8", 12)
CONNECTION(YC_8, "L8", 13)

CONNECTION("L8", 3, "K8", 2)
CONNECTION("L8", 11, "K8", 3)

CONNECTION("K8", 1, "J8", 1)

CONNECTION("J8", 2, "K8", 5)
CONNECTION(YC_5, "K8", 6)

CONNECTION(SLC, "L8", 9)
CONNECTION("K8", 4, "L8", 10)

CONNECTION(VCC, "M8", 10)
CONNECTION(VCC, "M8", 12)
CONNECTION("L8", 8, "M8", 11)
CONNECTION("M7", 8, "M8", 13)

CONNECTION("J8", 4, "L7", 14)
CONNECTION(GND, "L7", 4)
CONNECTION(V_SYN_n, "L7", 11)
CONNECTION(GND, "L7", 15)
CONNECTION(GND, "L7", 1)
CONNECTION(GND, "L7", 10)
CONNECTION(GND, "L7", 9)
CONNECTION(GND, "L7", 5)

CONNECTION("L8", 8, "J8", 5)

CONNECTION("M8", 8, "L8", 5)
CONNECTION("J8", 6, "L8", 4)

CONNECTION("M8", 9, "C39", 1)
CONNECTION(GND, "C39", 2)

CONNECTION("C39", 3, "J8", 3)

CONNECTION("M7", 8, "K8", 11)  //pin 11 and 13 reversed on diagram
CONNECTION(BIT_CLK_n, "K8", 12)

CONNECTION(VCC, "M7", 10)
CONNECTION(VCC, "M7", 12)
CONNECTION(SX_COM, "M7", 11)
CONNECTION("L8", 8, "M7", 13)

CONNECTION(SV_D, "K6", 15)
CONNECTION(SV_C, "K6", 1)
CONNECTION(SV_B, "K6", 2)
CONNECTION(SV_A, "K6", 3)
CONNECTION("L7", 6, "K6", 4)
CONNECTION("L7", 2, "K6", 7)
CONNECTION("L7", 3, "K6", 6)
CONNECTION("M8", 9, "K6", 5)
CONNECTION(GND, "K6", 13)
CONNECTION(GND, "K6", 14)

CONNECTION(SV_D, "J6", 15)
CONNECTION(SV_C, "J6", 1)
CONNECTION(SV_B, "J6", 2)
CONNECTION(SV_A, "J6", 3)
CONNECTION("L7", 6, "J6", 4)
CONNECTION("L7", 2, "J6", 7)
CONNECTION("L7", 3, "J6", 6)
CONNECTION("M8", 9, "J6", 5)
CONNECTION(GND, "J6", 13)
CONNECTION(GND, "J6", 14)

CONNECTION("J6", 9, "J7", 11)
CONNECTION("J6", 10, "J7", 12)
CONNECTION("J6", 11, "J7", 13)
CONNECTION("J6", 12, "J7", 14)
CONNECTION("K6", 9, "J7", 3)
CONNECTION("K6", 10, "J7", 4)
CONNECTION("K6", 11, "J7", 5)
CONNECTION("K6", 12, "J7", 6)
CONNECTION("J8", 4, "J7", 1)
CONNECTION("K8", 13, "J7", 2)  //pin 11 and 13 reversed on diagram
CONNECTION(GND, "J7", 15)
CONNECTION(GND, "J7", 10)

CONNECTION("J6", 9, "K7", 11)
CONNECTION("J6", 10, "K7", 12)
CONNECTION("J6", 11, "K7", 13)
CONNECTION("J6", 12, "K7", 14)
CONNECTION("K6", 9, "K7", 3)
CONNECTION("K6", 10, "K7", 4)
CONNECTION("K6", 11, "K7", 5)
CONNECTION("K6", 12, "K7", 6)
CONNECTION("L8", 6, "K7", 1)
CONNECTION("K8", 13, "K7", 2)  //pin 11 and 13 reversed on diagram
CONNECTION(GND, "K7", 15)
CONNECTION("J7", 9, "K7", 10)

CONNECTION(VCC, "M7", 4)
CONNECTION("K7", 9, "M7", 2)
CONNECTION("K8", 13, "M7", 3)  //pin 11 and 13 reversed on diagram
CONNECTION("M7", 9, "M7", 1)


/* page 10 - ship bullet count */

CONNECTION(SBX_UD_n, "E1", 5)
CONNECTION(SBX_ENB_n, "E1", 4)
CONNECTION(SBX_CLK_n, "E1", 14)
CONNECTION(SB_SET_n, "E1", 11)
CONNECTION(SPX1, "E1", 15)
CONNECTION(SPX2, "E1", 1)
CONNECTION(SPX3, "E1", 10)
CONNECTION(SPX4, "E1", 9)

CONNECTION(BIT_CLK,"E2", 3)
CONNECTION("E1", 3,"E2", 1)
CONNECTION("E1", 2,"E2", 14)
CONNECTION("E1", 6,"E2", 11)
CONNECTION("E1", 7,"E2", 9)
CONNECTION(XC_1,"E2", 15)
CONNECTION(XC_2,"E2", 13)
CONNECTION(XC_3,"E2", 12)
CONNECTION(XC_4,"E2", 10)

CONNECTION(SBX_UD_n, "F1", 5)
CONNECTION(SBX_ENB_n, "F1", 4)
CONNECTION("E1", 13, "F1", 14)
CONNECTION(SB_SET_n, "F1", 11)
CONNECTION(SPX5, "F1", 15)
CONNECTION(SPX6, "F1", 1)
CONNECTION(SPX7, "F1", 10)
CONNECTION(SPX8, "F1", 9)

CONNECTION("E2", 6, "F2", 3)
CONNECTION("F1", 3, "F2", 1)
CONNECTION("F1", 2, "F2", 14)
CONNECTION("F1", 6, "F2", 11)
CONNECTION("F1", 7, "F2", 9)
CONNECTION(XC_5, "F2", 15)
CONNECTION(XC_6, "F2", 13)
CONNECTION(XC_7, "F2", 12)
CONNECTION(XC_8, "F2", 10)

CONNECTION(VCC, "G1", 5) 
CONNECTION(SBY_ENB, "G1", 4)
CONNECTION(SBY_CLK_n, "G1", 14)
CONNECTION(SB_SET_n, "G1", 11)
CONNECTION(SBVA_n, "G1", 15)
CONNECTION(SBVB_n, "G1", 1)
CONNECTION(SBY_ENB_n, "G1", 10)
CONNECTION(VCC, "G1", 9)

CONNECTION(SLC, "G2", 3)
CONNECTION("G1", 3, "G2", 1)
CONNECTION("G1", 2, "G2", 14)
CONNECTION("G1", 6, "G2", 11)
CONNECTION("G1", 7, "G2", 9)
CONNECTION(YC_1, "G2", 15)
CONNECTION(YC_2, "G2", 13)
CONNECTION(YC_3, "G2", 12)
CONNECTION(YC_4, "G2", 10)

CONNECTION(VCC, "H1", 5) 
CONNECTION(SBY_ENB, "H1", 4)
CONNECTION("G1", 13, "H1", 14)
CONNECTION(SB_SET_n, "H1", 11)
CONNECTION(GND, "H1", 15)
CONNECTION(VCC, "H1", 1)
CONNECTION(VCC, "H1", 10)
CONNECTION(VCC, "H1", 9)

CONNECTION("G2", 6, "H2", 3)
CONNECTION("H1", 3, "H2", 1)
CONNECTION("H1", 2, "H2", 14)
CONNECTION("H1", 6, "H2", 11)
CONNECTION("H1", 7, "H2", 9)
CONNECTION(YC_5, "H2", 15)
CONNECTION(YC_6, "H2", 13)
CONNECTION(YC_7, "H2", 12)
CONNECTION(YC_8, "H2", 10)


/* page 11 - ship x count */

CONNECTION(SML_n, "H3", 6)   //IO PIN 7
CONNECTION(SNK_SHP, "H3", 5)

CONNECTION(SMR_n, "H3", 3)   //IO PIN 8
CONNECTION("L4", 9, "H3", 2)

CONNECTION(SNK_SHP, "H3", 11)
CONNECTION(SNK_SHP, "H3", 12)

CONNECTION("H3", 13, "L4", 10)
CONNECTION(RS, "L4", 12)
CONNECTION(SHP_VID_n, "L4", 11)
CONNECTION(VCC, "L4", 13)

CONNECTION("H3", 4, "H3", 9)
CONNECTION("H3", 1, "H3", 8)

CONNECTION("H3", 10, "G3", 5)
CONNECTION(INT_SHP_n, "G3", 4)

CONNECTION("H3", 1, "G3", 13)
CONNECTION(INT_SHP_n, "G3", 12)

CONNECTION("G3", 6, "G3", 1)
CONNECTION("G3", 6, "G3", 2)

CONNECTION(CLK1, "G3", 9)
CONNECTION(CLK1, "G3", 10)

CONNECTION("G3", 8, "E3", 14)
CONNECTION("G3", 3, "E3", 4)
CONNECTION("G3", 11, "E3", 5)
CONNECTION(DLY_SHP_n, "E3", 11)
CONNECTION(VCC, "E3", 15)
CONNECTION(VCC, "E3", 1)
CONNECTION(VCC, "E3", 10)
CONNECTION(VCC, "E3", 9)

CONNECTION("E3", 13, "F3", 14) //diagram is wrong
CONNECTION("G3", 3, "F3", 4)
CONNECTION("G3", 11, "F3", 5)
CONNECTION(DLY_SHP_n, "F3", 11)
CONNECTION(VCC, "F3", 15)
CONNECTION(VCC, "F3", 1)
CONNECTION(VCC, "F3", 10)
CONNECTION(VCC, "F3", 9)

CONNECTION(BIT_CLK, "E4", 3)      
CONNECTION("E3", 3, "E4", 1)   //diagram is wrong
CONNECTION("E3", 2, "E4", 14)  //diagram is wrong
CONNECTION("E3", 6, "E4", 11)  //diagram is wrong
CONNECTION("E3", 7, "E4", 9)   //diagram is wrong
CONNECTION(XC_1, "E4", 15)  // i think the diagram has these reversed - see plane on page 5
CONNECTION(XC_2, "E4", 13)
CONNECTION(XC_3, "E4", 12)
CONNECTION(XC_4, "E4", 10)

CONNECTION("E4", 6, "F4", 3)
CONNECTION("F3", 3, "F4", 1)   //diagram is wrong
CONNECTION("F3", 2, "F4", 14)  //diagram is wrong
CONNECTION("F3", 6, "F4", 11)  //diagram is wrong
CONNECTION("F3", 7, "F4", 9)   //diagram is wrong
CONNECTION(XC_5, "F4", 15)  // i think the diagram has these reversed - see plane on page 5
CONNECTION(XC_6, "F4", 13)
CONNECTION(XC_7, "F4", 12)
CONNECTION(XC_8, "F4", 10)


/* page 12 - timing */

CONNECTION(VCC, "H10", 10)
CONNECTION("H10", 8, "H10", 12)
CONNECTION("CLOCK", 1, "H10", 11)
CONNECTION(VCC, "H10", 13)

CONNECTION("H10", 8, "E9", 14)
CONNECTION(GND, "E9", 5)
CONNECTION(GND, "E9", 4)
CONNECTION(VCC, "E9", 11)

CONNECTION("E9", 13, "F9", 14)
CONNECTION(GND, "F9", 5)
CONNECTION(GND, "F9", 4)
CONNECTION(VCC, "F9", 11)

CONNECTION("F9", 13, "G9", 14)
CONNECTION(GND, "G9", 5)
CONNECTION(GND, "G9", 4)
CONNECTION(VCC, "G9", 11)

CONNECTION("G9", 13, "H9", 14)
CONNECTION(GND, "H9", 5)
CONNECTION(GND, "H9", 4)
CONNECTION(VCC, "H9", 11)

CONNECTION("F9", 13, "G10", 9)

CONNECTION("H9", 13, "G10", 3)

CONNECTION("G10", 4, "G10", 5)


/* PAGE 13 - SCORE VIDEO */

CONNECTION(XC_3, "K10", 2)
CONNECTION(XC_3, "K10", 3)

CONNECTION(XC_2, "K10", 5)
CONNECTION(XC_2, "K10", 6)

CONNECTION("K10", 1, "J10", 1)
CONNECTION("K10", 4, "J10", 2)

CONNECTION(YC_8, "K10", 8)
CONNECTION(YC_6, "K10", 9)

CONNECTION(YC_5, "K10", 11)
CONNECTION(YC_5, "K10", 12)

CONNECTION("K10", 10, "K9", 1)
CONNECTION(YC_7, "K9", 2)
CONNECTION("K10", 13, "K9", 13)

CONNECTION("K9", 12, "J9", 8)
CONNECTION("K9", 12, "J9", 9)

CONNECTION(XC_8, "J9", 2)
CONNECTION(XC_5, "J9", 3)

CONNECTION(XC_6, "J9", 11)
CONNECTION(XC_5, "J9", 12)

CONNECTION("J9", 1, "K9", 3)
CONNECTION(XC_7, "K9", 4)
CONNECTION(XC_6, "K9", 5)

CONNECTION(XC_7, "K9", 11)
CONNECTION(XC_8, "K9", 10)
CONNECTION("J9", 13, "K9", 9)

CONNECTION("K9", 6, "J10", 9)
CONNECTION("K9", 8, "J10", 10)

CONNECTION("J9", 10, "J10", 13)
CONNECTION("J10", 8, "J10", 12)

CONNECTION("J10", 11, "J9", 5)
CONNECTION("J10", 11, "J9", 6)

CONNECTION(XC_3, "J10", 4)
CONNECTION("J9", 4, "J10", 5)

CONNECTION(VCC, "J11", 4)
CONNECTION("J11", 6, "J11", 2)
CONNECTION("J10", 6, "J11", 3)
CONNECTION(SLC_n, "J11", 1)

CONNECTION(VCC, "J11", 10)
CONNECTION("J11", 8, "J11", 12)
CONNECTION("J11", 6, "J11", 11)
CONNECTION(SLC_n, "J11", 13)

CONNECTION(VCC, "L10", 5)
CONNECTION(VCC, "L10", 3)
CONNECTION(SCR_DA, "L10", 7)
CONNECTION(SCR_DB, "L10", 1)
CONNECTION(SCR_DC, "L10", 2)
CONNECTION(SCR_DD, "L10", 6)
CONNECTION("J10", 3, "L10", 4)

CONNECTION("L10", 15, "M10", 3)
CONNECTION("L10", 12, "M10", 2)
CONNECTION("L10", 9, "M10", 6)
CONNECTION("L10", 11, "M10", 5)
CONNECTION("K10", 1, "M10", 1)
CONNECTION("K10", 4, "M10", 15)

CONNECTION("M10", 4, "L9", 9)
CONNECTION("L10", 14, "L9", 10)

CONNECTION("L10", 13, "L9", 4)
CONNECTION("M10", 4, "L9", 5)

CONNECTION("L9", 8, "L9", 12)
CONNECTION("M10", 7, "L9", 13)

CONNECTION("L10", 10, "L9", 1)
CONNECTION("M10", 7, "L9", 2)

CONNECTION("L9", 6, "M9", 4)
CONNECTION("M10", 4, "M9", 3)
CONNECTION("L9", 11, "M9", 2)
CONNECTION("M10", 7, "M9", 1)
CONNECTION("L9", 3, "M9", 15)
CONNECTION(GND, "M9", 12)
CONNECTION(GND, "M9", 13)
CONNECTION(GND, "M9", 14)
CONNECTION("J10", 11, "M9", 7)
CONNECTION(YC_2, "M9", 11)
CONNECTION(YC_3, "M9", 10)
CONNECTION(YC_4, "M9", 9)


/* page 14 - score count */

CONNECTION(PLN_FAL_n, "L11", 5)
CONNECTION(VCC, "L11", 4)
CONNECTION("M11", 12, "L11", 11)
CONNECTION(VCC, "L11", 15)
CONNECTION(GND, "L11", 1)
CONNECTION(GND, "L11", 10)
CONNECTION(VCC, "L11", 9)
CONNECTION(CLK_SCR, "L11", 14)

CONNECTION("L11", 12, "M11", 5)
CONNECTION(VCC, "M11", 4)
CONNECTION("M11", 12, "M11", 11)
CONNECTION(VCC, "M11", 15)
CONNECTION(GND, "M11", 1)
CONNECTION(GND, "M11", 10)
CONNECTION(VCC, "M11", 9)
CONNECTION(CLK_SCR, "M11", 14)

CONNECTION(SNK_SHP_n, "L12", 5)
CONNECTION(VCC, "L12", 4)
CONNECTION("M12", 12, "L12", 11)
CONNECTION(VCC, "L12", 15)
CONNECTION(GND, "L12", 1)
CONNECTION(GND, "L12", 10)
CONNECTION(VCC, "L12", 9)
CONNECTION(CLK_SCR, "L12", 14)

CONNECTION("L12", 12, "M12", 5)
CONNECTION(VCC, "M12", 4)
CONNECTION("M12", 12, "M12", 11)
CONNECTION(VCC, "M12", 15)
CONNECTION(GND, "M12", 1)
CONNECTION(GND, "M12", 10)
CONNECTION(VCC, "M12", 9)
CONNECTION(CLK_SCR, "M12", 14)

CONNECTION(GND, "K11", 1)
CONNECTION(GND, "K11", 15) 
CONNECTION("M11", 3, "K11", 6)
CONNECTION("L11", 3, "K11", 5) 
CONNECTION("M12", 3, "K11", 4)
CONNECTION("L12", 3, "K11", 3) 
CONNECTION("M11", 2, "K11", 10)
CONNECTION("L11", 2, "K11", 11)
CONNECTION("M12", 2, "K11", 12)
CONNECTION("L12", 2, "K11", 13)
CONNECTION(SCR_A, "K11", 14)
CONNECTION(SCR_B, "K11", 2)

CONNECTION(GND, "K12", 1)
CONNECTION(GND, "K12", 15) 
CONNECTION("M11", 6, "K12", 6)
CONNECTION("L11", 6, "K12", 5) 
CONNECTION("M12", 6, "K12", 4) 
CONNECTION("L12", 6, "K12", 3) 
CONNECTION("M11", 7, "K12", 10) 
CONNECTION("L11", 7, "K12", 11)
CONNECTION("M12", 7, "K12", 12)
CONNECTION("L12", 7, "K12", 13)
CONNECTION(SCR_A, "K12", 14)
CONNECTION(SCR_B, "K12", 2)

/* page 15 - video control */

CONNECTION(VFC_n, "D11", 8)
CONNECTION(Y_16_CLK_n, "D11", 9)
CONNECTION(YC_8, "D11", 6)
CONNECTION(GND, "D11", 1)
CONNECTION(VCC, "D11", 2)
CONNECTION(GND, "D11", 3)
CONNECTION(VCC, "D11", 4)
CONNECTION(GND, "D11", 5)

CONNECTION("D11", 12, "B10", 2)
CONNECTION("D11", 11, "B10", 3)

CONNECTION(SLC_n, "C10", 8)
CONNECTION(X_16_CLK_n, "C10", 9)
CONNECTION(XC_8, "C10", 6)
CONNECTION(GND, "C10", 1)
CONNECTION(VCC, "C10", 2)
CONNECTION(GND, "C10", 3)
CONNECTION(VCC, "C10", 4)
CONNECTION(GND, "C10", 5)

CONNECTION("C10", 12, "B10", 5)
CONNECTION("C10", 11, "B10", 6)

CONNECTION("B10", 1, "A11", 4)
CONNECTION("B10", 4, "A11", 5)

CONNECTION("B10", 4, "B10", 11)
CONNECTION("C10", 13, "B10", 12)

CONNECTION("D11", 12, "D10", 13)

CONNECTION("D11", 12, "B10", 8)
CONNECTION("C10", 12, "B10", 9)

CONNECTION("A11", 6, "A11", 1)
CONNECTION("B11", 8, "A11", 2)

CONNECTION("A11", 6, "A11", 9)
CONNECTION("B11", 11, "A11", 10)

CONNECTION("A11", 6, "D12", 9)
CONNECTION("A11", 6, "D12", 10)

CONNECTION(SHP_VID, "C11", 9)
CONNECTION(SCR_VID, "C11", 8)

CONNECTION(PBV_X, "A10", 1)
CONNECTION(PBV_Y, "A10", 2)
CONNECTION(PBV_ENB, "A10", 13)

CONNECTION(SBV_X, "A10", 9)
CONNECTION(SBV_Y, "A10", 10)
CONNECTION(SBV_ENB, "A10", 11)

CONNECTION(PLN_FAL, "A10", 5)
CONNECTION(PLN_FAL, "A10", 4)
CONNECTION(VCK2, "A10", 3)

CONNECTION(PLN_VID, "B11", 4)
CONNECTION("A10", 6, "B11", 5)  //missing on diagram

CONNECTION("A10", 8, "A12", 9)
CONNECTION("B11", 6, "A12", 10)

CONNECTION("A10", 12, "A11", 12)
CONNECTION("A12", 8, "A11", 13)

CONNECTION("C11", 10, "B11", 9)
CONNECTION("A11", 11, "B11", 10)

CONNECTION(SHP_Y, "G10", 1)

CONNECTION(VCC, "H10", 4)
CONNECTION(VCC, "H10", 2)
CONNECTION("G10", 2, "H10", 3)
CONNECTION("B10", 1, "H10", 1)

CONNECTION("H10", 5, "B11", 12)
CONNECTION(YC_1, "B11", 13)

CONNECTION(SLB_n, "A12", 5)
CONNECTION(SHP_VID_n, "A12", 4)
CONNECTION(PLN_VID, "A12", 12)
CONNECTION("H10", 5, "A12", 13)

CONNECTION(PLN_VID_n, "C11", 5)
CONNECTION("A12", 6, "C11", 6)

CONNECTION("C11", 4, "C11", 3)
CONNECTION("A12", 11, "C11", 2)

CONNECTION("VIDEO", 1, "A11", 3)
CONNECTION("VIDEO", 2, "A11", 8)
CONNECTION("VIDEO", 3, GND)
CONNECTION("VIDEO", Video::VBLANK_PIN, V_SYN)
CONNECTION("VIDEO", Video::HBLANK_PIN, "C10", 12)

/* page 16 - bullet generator */

CONNECTION(GND, "A13", 9)
CONNECTION(BIT_CLK, "A13", 1)
CONNECTION(PBX_COM, "A13", 8)
CONNECTION(VCC, "A13", 2)
CONNECTION(VCC, "A13", 3)
CONNECTION(VCC, "A13", 4)
CONNECTION(VCC, "A13", 16) // not shown

CONNECTION(GND, "B13", 9)
CONNECTION(SLC, "B13", 1)
CONNECTION(PBY_COM, "B13", 8)
CONNECTION(VCC, "B13", 2)
CONNECTION(VCC, "B13", 3)
CONNECTION(VCC, "B13", 4)
CONNECTION(VCC, "B13", 16) // not shown

CONNECTION(GND, "C13", 9)
CONNECTION(BIT_CLK, "C13", 1)
CONNECTION(SBX_COM, "C13", 8)
CONNECTION(VCC, "C13", 2)
CONNECTION(VCC, "C13", 3)
CONNECTION(VCC, "C13", 4)
CONNECTION(VCC, "C13", 16) // not shown

CONNECTION(GND, "D13", 9)
CONNECTION(SLC, "D13", 1)
CONNECTION(SBY_COM, "D13", 8)
CONNECTION(VCC, "D13", 2)
CONNECTION(VCC, "D13", 3)
CONNECTION(VCC, "D13", 4)
CONNECTION(VCC, "D13", 16) // not shown

CONNECTION(PLN_VID_n, "D12", 12)
CONNECTION("B12", 8, "D12", 13)

CONNECTION("A13", 13, "B12", 9)
CONNECTION("B13", 13, "B12", 10)

CONNECTION("C13", 13, "B12", 12)
CONNECTION("D13", 13, "B12", 13)

CONNECTION(SHP_VID, "D12", 1)
CONNECTION("D12", 11, "D12", 2)

CONNECTION(PLN_VID, "B12", 1)
CONNECTION(BLK, "B12", 2)

CONNECTION(BLK, "C12", 5)
CONNECTION(XC_6, "C12", 6)

CONNECTION(PLN_VID, "B12", 4)
CONNECTION("C12", 4, "B12", 5)

CONNECTION(BLK_n, "C12", 8)
CONNECTION(SBV_n, "C12", 9)

CONNECTION("C12", 10, "C12", 11)
CONNECTION(PLN_FAL, "C12", 12)

CONNECTION(BLK_n, "C11", 11)
CONNECTION("B12", 8, "C11", 12)

CONNECTION("C11", 13, "D12", 4)
CONNECTION("C11", 13, "D12", 5)

CONNECTION("D12", 6, "A12", 2)
CONNECTION(SHN_n, "A12", 1)


/* page 17 - coin */


// clock stuff

CONNECTION(SLC, "G12", 5)
CONNECTION(V_SYN, "G12", 4)

CONNECTION(VFC_n, "E11", 14)
CONNECTION(GND, "E11", 5)
CONNECTION(GND, "E11", 4)
CONNECTION(VCC, "E11", 11)

CONNECTION("E11", 13, "F11", 14)
CONNECTION(GND, "F11", 5)
CONNECTION(GND, "F11", 4)
CONNECTION(VCC, "F11", 11)

CONNECTION(SLC, "G11", 8)
CONNECTION(SLC, "G11", 9)
CONNECTION(V_SYN_n, "G11", 6)
CONNECTION(GND, "G11", 5)
CONNECTION(GND, "G11", 4)
CONNECTION(GND, "G11", 3)
CONNECTION(GND, "G11", 2)
CONNECTION(VCC, "G11", 1)

CONNECTION("G11", 13, "H11", 2)
CONNECTION("G12", 6, "H11", 3)

CONNECTION("G11", 12, "H11", 11)
CONNECTION("G12", 6, "H11", 12)

CONNECTION("G11", 11, "H11", 8)
CONNECTION("G12", 6, "H11", 9)

CONNECTION("G11", 10, "H11", 5)
CONNECTION("G12", 6, "H11", 6)

// coin and start stuff

// i think inserting a coin starts the game ??? no start button...

CONNECTION("COIN1", 1, "E12", 10)
CONNECTION("E12", 6, "E12", 9)

CONNECTION("E12", 8, "E12", 5)
CONNECTION("F12", 10, "E12", 4)

CONNECTION("E12", 6, "F12", 9)

CONNECTION("F12", 8, "F12", 13)

CONNECTION("E12", 8, "E13", 4)  //555 WITH POTENTIOMETER

CONNECTION("E12", 8, "C42", 1)
CONNECTION(GND, "C42", 2)
CONNECTION("C42", 3, "F12", 1)

CONNECTION(VCC, "F12", 5)

CONNECTION("F12", 6, "F12", 3)

CONNECTION("F12", 4, "E12", 12)
CONNECTION("F12", 2, "E12", 13)

CONNECTION("E13", 3, "E12", 1)
CONNECTION("F12", 4, "E12", 2)

CONNECTION("E12", 3, "F12", 11)


/* page 18 - sound */

CONNECTION(PLN_SPD_n, "D10", 1)

CONNECTION("D10", 2, "K13", 1)
CONNECTION(VCK5, "K13", 2)

CONNECTION(PLN_SPD_n, "K13", 13)  //K13 pin not numbered
CONNECTION(VCK2, "K13", 12)  //K13 pin not numbered

CONNECTION("K13", 3, "M13", 11)
CONNECTION("K13", 11, "M13", 12)  //K13 pin not numbered

CONNECTION(CON_LTH, "L13", 10)
CONNECTION("M13", 13, "L13", 9)
CONNECTION(VCC, "L13", 11)

CONNECTION(VCK3, "L13", 2)
CONNECTION(SME_n, "L13", 1)
CONNECTION(CON_LTH, "L13", 3)

CONNECTION("L13", 13, "K13", 4)
CONNECTION(YC_3, "K13", 5)

CONNECTION("L13", 5, "M13", 8)
CONNECTION("K13", 6, "M13", 9)  

CONNECTION("M13", 10, "M13", 5)
CONNECTION("M13", 10, "M13", 6)

CONNECTION(PLN_BF_n, "J13", 1)
CONNECTION(SHP_BF_n, "J13", 2)

CONNECTION("J13", 3, "J13", 12)
CONNECTION(YC_7, "J13", 13)

CONNECTION(PLN_FAL_n, "J12", 1)
CONNECTION(SNK_SHP_n, "J12", 2)

CONNECTION(SNK_SHP_n, "J12", 12)
CONNECTION(SNK_SHP_n, "J12", 13)

CONNECTION("J12", 11, "J12", 10)
CONNECTION(VCK4, "J12", 9)

CONNECTION("J12", 3, "J12", 4)
CONNECTION("J12", 8, "J12", 5)

CONNECTION("J12", 6, "G12", 12) //LM324 ANALOG CIRCUIT BETWEEN THIS CONNECTION
CONNECTION("J12", 6, "G12", 13)

CONNECTION("J12", 3, "J13", 5)
//CONNECTION("G12", 11, "J13", 4)
CONNECTION("J12", 6, "J13", 4)

CONNECTION("J13", 11, "J13", 10)
CONNECTION("J13", 6, "J13", 9)

CONNECTION(CON_LTH, "K13", 10)
CONNECTION("J13", 8, "K13", 9)
            
CONNECTION("AUDIO", 1, "M13", 4)
CONNECTION("AUDIO", 2, "K13", 8)  
              
CONNECTION("AUDIO", i1, "MIXER", 1)
CONNECTION("AUDIO", i2, "MIXER", 2)

CONNECTION("MIXER", i1, "AUDIO", Audio::OUTPUT_MONO)

CIRCUIT_LAYOUT_END
            
          


/* inputs - IO PINS*/             
/*
 7 SML_n ship move left
 8 SMR_n ship move right
 9 SHP_GR_n ship gun right
10 SHP_GL_n ship gun left
11 SBF_NO_n ship button fire
12 SBF_NC   ship button fire
13 PBF_NO_n plane button fire
14 PBF_NC_n plane button fire
15 PLN_SPD_FST_n plane speed fast
16 PLN_RCCW_n plane rotate counter-clockwise
17 PLN_RCW_n plane rotate clockwise
*/
