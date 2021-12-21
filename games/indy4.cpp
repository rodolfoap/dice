/* Indy 4, by Atari, 1975 */

// wip 4/13/2014
// TODO: 50 Hz Mode ?

#include "../circuit_desc.h"
#include "../circuit.h"


#define DEBUG
#undef DEBUG

#ifdef DEBUG
static VcdLogDesc vcd_log_desc
(
    "output_indy4.vcd",
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

static RomDesc syncf4_desc("indy4", "indy.s1", 0x1AAFBE72);    
static RomDesc synce5_desc("indy4", "indy.s2", 0xE30EA877);    
static RomDesc cark2_desc("indy4", "indy.c1", 0xB3443354);    
static RomDesc cark4_desc("indy4", "indy.c2", 0x01DCA5B9);    

static Mono555Desc carl1_555_desc(K_OHM(82), U_FARAD(5));   // R5, C1
static Astable555Desc coinl1_555_desc(K_OHM(720.0), K_OHM(220.0), U_FARAD(1.0));

static PotentimeterAstable555Desc pot1_desc("playtime", "Play Time", K_OHM(720.0), K_OHM(220.0), K_OHM(1220.0), coinl1_555_desc);

static Wheel1Desc wheel1_desc;
static Wheel2Desc wheel2_desc;
static Wheel3Desc wheel3_desc;
static Wheel4Desc wheel4_desc;

static CapacitorDesc carcap27_desc(P_FARAD(100));
static CapacitorDesc carcap31_desc(P_FARAD(100));
static CapacitorDesc carcap32_desc(P_FARAD(100));
static CapacitorDesc coincap35_desc(U_FARAD(0.1));
static CapacitorDesc scorecap08_desc(P_FARAD(33)); // or 330? Indy 8 has 330 pF.
static CapacitorDesc scorecap10_desc(U_FARAD(0.001));
static CapacitorDesc scorecap11_desc(U_FARAD(0.001));
static CapacitorDesc scorecap12_desc(U_FARAD(0.001));
static CapacitorDesc scorecap13_desc(U_FARAD(0.001));
static CapacitorDesc scorecap14_desc(U_FARAD(0.001));
static CapacitorDesc synccap01_desc(P_FARAD(330));

static VIDEO_RESISTOR_MATRIX indy4_video_res =
{   //    R           G           B
     { K_OHM(2.2), K_OHM(2.4), K_OHM(2.4) }, // White
     { K_OHM(2.2), K_OHM(2.4),   OHM(0.0) }, // Yellow
     {   OHM(0.0), K_OHM(2.4), K_OHM(2.2) }, // Cyan
     {   OHM(0.0), K_OHM(2.2),   OHM(0.0) }, // Green
     { K_OHM(2.2),   OHM(0.0), K_OHM(2.2) }, // Magenta
     { K_OHM(2.2),   OHM(0.0),   OHM(0.0) }, // Red
     {   OHM(0.0),   OHM(0.0), K_OHM(2.2) }, // Blue
     { K_OHM(1.8), K_OHM(2.7), K_OHM(3.3) }, // Peach
     { K_OHM(1.8), K_OHM(1.8), K_OHM(1.8) }  // GND
};

static VIDEO_DESC( indy4 )
    VIDEO_MONITOR_TYPE(COLOR)
    VIDEO_RESISTANCES_RGB(indy4_video_res)
    VIDEO_CONTRAST(4.5)
VIDEO_DESC_END

static INPUT_DESC( indy4 )
    INPUT_INFO(WHEEL1_INPUT, "Rotate Green Car")
    INPUT_INFO(BUTTONS1_INPUT, {{ 1, 2 }}, "Gas Pedal, Hand Brake")
    INPUT_INFO(WHEEL2_INPUT, "Rotate Cyan Car")
    INPUT_INFO(BUTTONS2_INPUT, {{ 1, 2 }}, "Gas Pedal, Hand Brake")
    INPUT_INFO(WHEEL3_INPUT, "Rotate Violet Car")
    INPUT_INFO(BUTTONS3_INPUT, {{ 1, 2 }}, "Gas Pedal, Hand Brake")
    INPUT_INFO(WHEEL4_INPUT, "Rotate Red Car")
    INPUT_INFO(BUTTONS4_INPUT, {{ 1, 2 }}, "Gas Pedal, Hand Brake")
    INPUT_INFO(COIN_INPUT, {{ 1 }}, "Insert Coin (Green)")
    INPUT_INFO(COIN_INPUT, {{ 2 }}, "Insert Coin (Cyan)")
    INPUT_INFO(COIN_INPUT, {{ 3 }}, "Insert Coin (Violet)")
    INPUT_INFO(COIN_INPUT, {{ 4 }}, "Insert Coin (Red)")
    INPUT_INFO(COIN_INPUT, {{ 5 }}, "Insert Dollar Bill")
INPUT_DESC_END


/**************************************************************************
    Board Prefixes
**************************************************************************/
#define SYNC "SYNC."
#define SCORE "SCORE."
#define COIN "COIN."
#define CAR "CAR."

// Cars
#define GREEN "GREEN."
#define CYAN "CYAN."
#define VIOLET "VIOLET."
#define RED "RED."

/**************************************************************************
    Signals
**************************************************************************/
// **** sync board ****
#define _CLOCK                SYNC"CRYSTAL", 1
#define _CLOCKn               SYNC"C2", 8 
#define _1H                   SYNC"A4", 14
#define _2H                   SYNC"A4", 13
#define _4H                   SYNC"A4", 12
#define _8H                   SYNC"A4", 11 
#define _16H                  SYNC"B4", 14 
#define _32H                  SYNC"B4", 13
#define _64H                  SYNC"B4", 12 
#define _128H                 SYNC"B4", 11 
#define _256H                 SYNC"C4", 14 
#define _512H                 SYNC"C4", 13
#define _VRESETn              SYNC"C3", 5
#define _VRESET               SYNC"C3", 6
#define _1V                   SYNC"D4", 12
#define _2V                   SYNC"D4", 9 
#define _4V                   SYNC"D4", 8 
#define _8V                   SYNC"D4", 11 
#define _16V                  SYNC"E4", 12
#define _32V                  SYNC"E4", 9
#define _64V                  SYNC"E4", 8 
#define _128V                 SYNC"E4", 11 
#define _256V                 SYNC"E3", 3
#define _256Vn                SYNC"E3", 2
#define _COMPRESETCLOCK       SYNC"C1", 6 
#define _BOUNDS               SYNC"B3", 6 //Horizontal boundary
#define _OTHERCARSVIOLET      SYNC"F5", 8 
#define _OTHERCARSRED         SYNC"F5", 3 
#define _OTHERCARSGREEN       SYNC"H5", 3 
#define _OTHERCARSCYAN        SYNC"H5", 6 
#define _FLASH                SYNC"J3", 11 //flash leader's score 
#define _BUFFEREDVRESET       SYNC"C2", 3 
#define _VSYNCn               SYNC"D2", 13
#define _VSYNC                SYNC"D2", 1
#define _HSYNCn               SYNC"C3", 9 
#define _HSYNC                SYNC"C3", 8
#define _50VSYNC              SYNC"E2", 8
#define _COMPSYNC             SYNC"D2", 10 
#define _VCLOCKn              SYNC"C2", 6 
#define _CHPT1                SCORE"CAP10", 2 //SYNC"E5", 9 
#define _CHPT2                SCORE"CAP13", 2 //SYNC"E5", 7 
#define _CHPT3                SCORE"CAP12", 2 //SYNC"E5", 6 
#define _CHPT4                SCORE"CAP14", 2 //SYNC"E5", 5 
#define _CHPT5                SCORE"CAP11", 2 //SYNC"E5", 4 
#define _SPS                  SYNC"E5", 3
#define _SL1                  SYNC"E5", 2
#define _SL2                  SYNC"E5", 1
#define _SCOREWINDOWn         SYNC"C1", 8 
#define _RT                   SYNC"C2", 11 //RaceTrack
#define _16VXOR32H            SYNC"B1", 11 
#define _FA                   SYNC"H2", 4
#define _FB                   SYNC"H2", 7
#define _FC                   SYNC"H2", 12
#define _FD                   SYNC"H2", 9


#define _SCORERESET           SCORE"B6", 8 //ALSO B6.6
#define _NOLEADER             SCORE"K2", 3
#define _LEADERID1            SCORE"K2", 7
#define _LEADERID2            SCORE"K2", 10
#define _LEADERID3            SCORE"K2", 15
#define _SCORE                SCORE"C2", 15
#define _SCOREn               SCORE"CAP08", 2
#define _VIOLETSCOREn         SCORE"D6", 11
#define _CYANSCOREn           SCORE"D6", 10
#define _REDSCOREn            SCORE"D6", 9
#define _GREENSCOREn          SCORE"D6", 3
#define _3D                   SCORE"F4", 12
#define _3A                   SCORE"F4", 9
#define _3B                   SCORE"F4", 8
#define _3C                   SCORE"F4", 11
#define _3E                   SCORE"E4", 12
#define _4A                   SCORE"E4", 9
#define _4B                   SCORE"E4", 8
#define _4C                   SCORE"E4", 11
#define _3F                   SCORE"E3", 12
#define _4D                   SCORE"E3", 9
#define _4E                   SCORE"E3", 8
#define _4F                   SCORE"E3", 11
#define _7D                   SCORE"B4", 12
#define _7A                   SCORE"B4", 9
#define _7B                   SCORE"B4", 8
#define _7C                   SCORE"B4", 11
#define _7E                   SCORE"A4", 12
#define _8A                   SCORE"A4", 9
#define _8B                   SCORE"A4", 8
#define _8C                   SCORE"A4", 11
#define _7F                   SCORE"A3", 12
#define _8D                   SCORE"A3", 9
#define _8E                   SCORE"A3", 8
#define _8F                   SCORE"A3", 11
#define _A                    SCORE"L3", 15
#define _B                    SCORE"K3", 15
#define _C                    SCORE"J3", 15
#define _D                    SCORE"F3", 15
#define _E                    SCORE"D3", 15
#define _F                    SCORE"B3", 15
#define _a                    SCORE"F1", 13
#define _b                    SCORE"F1", 12
#define _c                    SCORE"F1", 11
#define _d                    SCORE"F1", 10
#define _e                    SCORE"F1", 9
#define _f                    SCORE"F1", 15
#define _g                    SCORE"F1", 14
#define _VIDEOWHITE           SCORE"C6", 8
#define _VIDEOVIOLET          SCORE"J6", 13
#define _VIDEORED             SCORE"J6", 10
#define _VIDEOGREEN           SCORE"J6", 1
#define _VIDEOCYAN            SCORE"H6", 4

#define _CARCLOCK           "J4", 4
#define _CARVRESETn         "J4", 8
#define _CARATTRACTn        "J4", 10
#define _CARR0              "E2", 6
#define _CARR1              "E2", 7
#define _CARR2              "F2", 3
#define _CARR3              "F2", 2
#define _CARR4              "F2", 6
#define _CARD1              "D1", 16
#define _CARD2              "D1", 15
#define _CARD3              "D1", 10
#define _CARD4              "D1", 9
#define _CARSKIDn           "C1", 3
#define _CARC0              "H3", 14
#define _CARC1              "H3", 13
#define _CARC2              "H3", 12
#define _CARC3              "H3", 11
#define _CARC4              "J2", 14
#define _CARL0              "K3", 13
#define _CARL1              "K3", 12
#define _CARL2              "K3", 11
#define _CARVTCn            "K1", 8
#define _CARVTC             "J4", 12 //mislabeled as pin 6 on schematic
#define _CARCARWINDOWn      "K1", 12
#define _CAR128CV           "J1", 5
#define _CARCAR             "J1", 3
#define _CARCARn            "J1", 2
#define _CAR1F              "B1", 12
#define _CAR2F              "B1", 9
#define _CAR4F              "B1", 8
#define _CAR8F              "B1", 11
#define _CAR16F             "A1", 3
#define _CAR16Fn            "A1", 2
#define _CARSPEED1          "C2", 3
#define _CARSPEED2          "C2", 2
#define _CARSPEED3          "C2", 6
#define _CARSPEED4          "C2", 7
#define _CARSPEED4n         "A3", 10
#define _CARSPEED5          "A1", 5
#define _CARSPEED5n         "A1", 6
#define _CARSTOP            "B2", 10
#define _CARGO              "D2", 5
#define _CARCOLLISION       "L1", 3
#define _CARCOLLISIONn      "A3", 8
#define _CARRESETn          "J4", 6
#define _CARBRAKE(car)      car"BUTTONS", i2 //PRESSED=HI
#define _CARGAS(car)        car"BUTTONS", i1 //PRESSED=HI
#define _CARCRASHSEQn       "F1", 9
#define _CARCRASHn          "H1", 8

#define _DOLLARCREDITn        COIN"F1", 4
#define _CREDIT               COIN"C5", 8
#define _CREDITn              COIN"CAP35", 2 //COIN"E1", 4
#define _PLAY                 COIN"H1", 10
#define _CRCGREEN             COIN"C4", 10
#define _CRCCYAN              COIN"C4", 11
#define _CRCVIOLET            COIN"C4", 12
#define _CRCRED               COIN"C4", 13
#define _ENDGAMEn             COIN"H1", 1
#define _ENDGAME              COIN"H1", 4
#define _GREENCOIN            COIN"COIN_IN", Coin::COIN1_HI
#define _CYANCOIN             COIN"COIN_IN", Coin::COIN2_HI
#define _VIOLETCOIN           COIN"COIN_IN", Coin::COIN3_HI
#define _REDCOIN              COIN"COIN_IN", Coin::COIN4_HI
#define _QGREENn              COIN"F5", 10
#define _QCYANn               COIN"F5", 8
#define _QVIOLETn             COIN"D5", 10
#define _QREDn                COIN"F5", 12
#define _LOGREENn             COIN"F5", 4
#define _LOCYANn              COIN"F5", 6
#define _LOVIOLETn            COIN"D5", 4
#define _LOREDn               COIN"F5", 2
#define _ATTRACTGREEN         COIN"F4", 6
#define _ATTRACTCYAN          COIN"F4", 3
#define _ATTRACTVIOLET        COIN"F4", 8
#define _ATTRACTRED           COIN"F4", 11
#define _CARRESETGREEN        COIN"A4", 5
#define _CARRESETCYAN         COIN"A4", 9
#define _CARRESETVIOLET       COIN"A3", 9
#define _CARRESETRED          COIN"B3", 5
#define _SCORERESETn          COIN"D5", 2
#define _1SEC                 COIN"E1", 6

#define _GREENCARn            GREEN"F4", 6
#define _VIOLETCARn           VIOLET"F4", 6
#define _REDCARn              RED"F4", 6
#define _CYANCARn             CYAN"F4", 6



/**************************************************************************
**** sync board ****
**************************************************************************/
static CIRCUIT_LAYOUT( sync )
    /* * * CHIPS * * */
    CHIP("CRYSTAL", CLOCK_14_318_MHZ)

    CHIP("A1", 7400)
    CHIP("A2", 74H04) //74H04
    CHIP("A3", 7420)
    CHIP("A4", 9316)
    CHIP("B1", 7486)
    CHIP("B2", 7400)
    CHIP("B3", 7400)
    CHIP("B4", 9316)
    //CHIP("B5", 7417) //7407
    CHIP("C1", 7420)
    CHIP("C2", 7437)
    CHIP("C3", 7474)
    CHIP("C4", 9316)
    CHIP("D1", 74107)
    CHIP("D2", 7402)
    CHIP("D3", 7410)
    CHIP("D4", 7493)
    //CHIP("D5", 7417) //7407
    CHIP("E2", 7404)
    CHIP("E3", 74107)
    CHIP("E4", 7493)
    CHIP("E5", 82S123, &synce5_desc)  //ROM 005502
    CHIP("F3", 74153)
    CHIP("F4", 82S129, &syncf4_desc)  //ROM 005503
    CHIP("F5", 7400)
    CHIP("H2", 9322)
    CHIP("H3", 7427)
    CHIP("H4", 7427)
    CHIP("H5", 7400)
    CHIP("J3", 7490)
    CHIP("J4", 7400)
    CHIP("CAP01", CAPACITOR, &synccap01_desc)
    
    OPTIMIZATION_HINT("F3", 256, 64)
    OPTIMIZATION_HINT("C2", 128, 256)
    
    /* * * CONNECTIONS * * */

      //CLOCKn
    CONNECTION(_CLOCK,           "C2", 9)
    CONNECTION(_CLOCK,           "C2", 10)

      //H COUNT
    CONNECTION(GND,              "A4", 3)
    CONNECTION(GND,              "A4", 4)
    CONNECTION(VCC,              "A4", 5)
    CONNECTION(GND,              "A4", 6)
    CONNECTION("B3", 8,          "A4", 9)
    CONNECTION(VCC,              "A4", 7)
    CONNECTION(VCC,              "A4", 10)
    CONNECTION(_CLOCK,           "A4", 2)
    CONNECTION(VCC,              "A4", 1)

    CONNECTION(VCC,              "B4", 3)
    CONNECTION(VCC,              "B4", 4)
    CONNECTION(VCC,              "B4", 5)
    CONNECTION(GND,              "B4", 6)
    CONNECTION("B3", 8,          "B4", 9)
    CONNECTION("A4", 15,         "B4", 7)
    CONNECTION(VCC,              "B4", 10)
    CONNECTION(_CLOCK,           "B4", 2)
    CONNECTION(VCC,              "B4", 1)

    CONNECTION(GND,              "C4", 3)
    CONNECTION(GND,              "C4", 4)
    CONNECTION(VCC,              "C4", 5)
    CONNECTION(VCC,              "C4", 6)
    CONNECTION("B3", 8,          "C4", 9)
    CONNECTION("A4", 15,         "C4", 7)
    CONNECTION("B4", 15,         "C4", 10)
    CONNECTION(_CLOCK,           "C4", 2)
    CONNECTION(VCC,              "C4", 1)

    CONNECTION("A4", 15,         "B3", 10)
    CONNECTION("C4", 15,         "B3", 9)

      //V COUNT
    CONNECTION(_256V,            "A3", 1)
    CONNECTION(_FA,              "A3", 2)
    CONNECTION(_FB,              "A3", 4)
    CONNECTION(_FC,              "A3", 5)

    CONNECTION(VCC,              "C3", 4)
    CONNECTION("A3", 6,          "C3", 2)
    CONNECTION(_512H,            "C3", 3)
    CONNECTION(VCC,              "C3", 1)

    CONNECTION(_VRESETn,         "C2", 1)
    CONNECTION(_VRESETn,         "C2", 2)

    CONNECTION(_VRESET,          "D4", 2)
    CONNECTION(_VRESET,          "D4", 3)
    CONNECTION(_512H,            "D4", 14)
    CONNECTION(_1V,              "D4", 1)

    CONNECTION(_VRESET,          "E4", 2)
    CONNECTION(_VRESET,          "E4", 3)
    CONNECTION(_8V,              "E4", 14)
    CONNECTION(_16V,             "E4", 1)

    CONNECTION(VCC,              "E3", 1)
    CONNECTION(_128V,            "E3", 12)
    CONNECTION(VCC,              "E3", 4)
    CONNECTION(_VRESETn ,        "E3", 13)

    CONNECTION(VCC,              "E3", 8)
    CONNECTION(_256V,            "E3", 9)
    CONNECTION(VCC,              "E3", 11)
    CONNECTION(VCC,              "E3", 10)

      //SYNC
    CONNECTION(_VRESET,          "D2", 12)
    CONNECTION(_VSYNC,           "D2", 11)
    
    CONNECTION(_VSYNCn,          "D2", 3)
    CONNECTION(_4V,              "D2", 2)

    CONNECTION(_256H,            "B3", 12)
    CONNECTION(_64H,             "B3", 13)

    CONNECTION("B3", 11,         "C3", 10)
    CONNECTION(_512H,            "C3", 12)
    CONNECTION(_256H,            "C3", 11)
    CONNECTION(VCC,              "C3", 13)

    CONNECTION(_256V,            "D3", 2)
    CONNECTION(_16V,             "D3", 1)
    CONNECTION(_8V,              "D3", 13)

    CONNECTION("D3", 12,         "E2", 9)

    CONNECTION(_HSYNC,           "D2", 9)
    CONNECTION(_FD,              "D2", 8)

    CONNECTION(_32V,             "H2", 2)
    CONNECTION(VCC,              "H2", 3)
    CONNECTION(_16V,             "H2", 5)
    CONNECTION(_4V,              "H2", 6)
    CONNECTION(_8V,              "H2", 14)
    CONNECTION(_2V,              "H2", 13)
    CONNECTION(_50VSYNC,         "H2", 11)
    CONNECTION(_VSYNC,           "H2", 10)
    CONNECTION(GND,              "H2", 15)
    CONNECTION(VCC,              "H2", 1) // TODO: DIPSWITCH ?

      //VCLOCK
    CONNECTION(_VSYNCn,          "D3", 9)
    CONNECTION(_16H,             "D3", 11)
    CONNECTION(_HSYNC,           "D3", 10)

    CONNECTION(_256V,            "D2", 5)
    CONNECTION("D3", 8,          "D2", 6)

    CONNECTION("D2", 4,          "C2", 4)
    CONNECTION("D2", 4,          "C2", 5)

      //
    CONNECTION(_16V,             "B1", 12)
    CONNECTION(_32H,             "B1", 13)

      //COMP RESET CLOCK
    CONNECTION("D1", 6,          "D1", 1)
    CONNECTION(_2V,              "D1", 12)
    CONNECTION("D1", 3,          "D1", 4)
    CONNECTION(_VSYNCn,          "D1", 13)

    CONNECTION("D1", 3,          "D1", 8)
    CONNECTION(_2V,              "D1", 9)
    CONNECTION("D1", 5,          "D1", 11)
    CONNECTION(_VSYNCn,          "D1", 10)

    CONNECTION(_32H,             "B2", 13)
    CONNECTION(_512H,            "B2", 12)

    CONNECTION("B2", 11,         "B2", 10)
    CONNECTION("B2", 6,          "B2", 9)

    CONNECTION("B2", 8,          "B2", 5)
    CONNECTION(_512H,            "B2", 4)

    CONNECTION("D1", 5,          "C1", 5)
    CONNECTION(_1V,              "C1", 1)
    CONNECTION(_2V,              "C1", 4)
    CONNECTION("B2", 8,          "C1", 2)

      //BOUNDS
    CONNECTION(_512H,            "H3", 11)
    CONNECTION(_128H,            "H3", 10)
    CONNECTION(GND,              "H3", 9)

    CONNECTION("H3", 8,          "B3", 5)
    CONNECTION(_256H,            "B3", 4)

      //RACE TRACK
    CONNECTION(_4V,              "A2", 13)

    CONNECTION("A2", 12,         "A3", 9)
    CONNECTION(_8H,              "A3", 13)
    CONNECTION(_512H,            "A3", 12)
    CONNECTION(_2V,              "A3", 10)

    CONNECTION(_128V,            "F4", 15)
    CONNECTION(_64V,             "F4", 1)
    CONNECTION(_32V,             "F4", 2)
    CONNECTION(_16V,             "F4", 3)
    CONNECTION(_8V,              "F4", 4)
    CONNECTION(_256H,            "F4", 7)
    CONNECTION(_128H,            "F4", 6)
    CONNECTION(_64H,             "F4", 5)
    CONNECTION(GND,              "F4", 13)
    CONNECTION(GND,              "F4", 14)

    CONNECTION("A3", 8,          "F3", 15)
    CONNECTION("F4", 9,          "F3", 10)
    CONNECTION("F4", 10,         "F3", 11)
    CONNECTION("F4", 11,         "F3", 12)
    CONNECTION("F4", 12,         "F3", 13)
    CONNECTION(_32H,             "F3", 2)
    CONNECTION(_16H,             "F3", 14)

    CONNECTION("F3", 9,          "CAP01", 1)

    CONNECTION(_4H,              "B3", 2)
    CONNECTION("CAP01", 2,       "B3", 1)

    CONNECTION(_SCORERESETn,     "H3", 13)
    CONNECTION(_FLASH,           "H3", 1)
    CONNECTION(_512H,            "H3", 2)

    CONNECTION(_SCOREWINDOWn,    "D3", 5)
    CONNECTION(_128H,            "D3", 4)
    CONNECTION("H3", 12,         "D3", 3)

    CONNECTION("B3", 3,          "C2", 12)
    CONNECTION("D3", 6,          "C2", 13)

      //FLASH
    CONNECTION(GND,              "J3", 6)
    CONNECTION(GND,              "J3", 7)
    CONNECTION(GND,              "J3", 2)
    CONNECTION(GND,              "J3", 3)
    CONNECTION(_VRESET,          "J3", 14)
    CONNECTION("J3", 12,         "J3", 1)

      //SCORE WINDOW
    CONNECTION(_512H,            "B1", 1)
    CONNECTION(_SPS,             "B1", 2)

    CONNECTION(_32H,             "A1", 1)
    CONNECTION(_SPS,             "A1", 2)

    CONNECTION(_SPS,             "A1", 12)
    CONNECTION(_SPS,             "A1", 13)

    CONNECTION("A1", 11,         "A1", 10)
    CONNECTION(_16V,             "A1", 9)

    CONNECTION("A1", 3,          "A1", 4)
    CONNECTION("A1", 8,          "A1", 5)

    CONNECTION("A1", 6,          "B1", 5)
    CONNECTION(_SL2,             "B1", 4)

    CONNECTION("B1", 3,          "C1", 10)
    CONNECTION(_SL1,             "C1", 13)
    CONNECTION(_32V,             "C1", 12)
    CONNECTION("B1", 6,          "C1", 9)

      //CHECKPOINT
    CONNECTION(_128V,            "E5", 14)
    CONNECTION(_64V,             "E5", 13)
    CONNECTION(_256H,            "E5", 12)
    CONNECTION(_128H,            "E5", 11)
    CONNECTION(_64H,             "E5", 10)
    CONNECTION(GND,              "E5", 15)

    CONNECTION("E5", 9,          SCORE"CAP10", 1)  
    CONNECTION("E5", 7,          SCORE"CAP13", 1)  
    CONNECTION("E5", 6,          SCORE"CAP12", 1)  
    CONNECTION("E5", 5,          SCORE"CAP14", 1)  
    CONNECTION("E5", 4,          SCORE"CAP11", 1)  


      //OTHER CARS (for crash detection)
      //unused connections left over from indy 800 design
    CONNECTION(VCC,              "J4", 2)
    CONNECTION(VCC,              "J4", 1)

    CONNECTION(VCC,              "J4", 4)
    CONNECTION(VCC,              "J4", 5)

      //used connections
    CONNECTION(_GREENCARn,       "J4", 13)
    CONNECTION(_CYANCARn,        "J4", 12)

    CONNECTION(_VIOLETCARn,      "J4", 10)
    CONNECTION(_REDCARn,         "J4", 9)

    CONNECTION("J4", 3,          "H4", 1)
    CONNECTION("J4", 6,          "H4", 13)
    CONNECTION("J4", 11,         "H4", 2)

    CONNECTION("J4", 3,          "H3", 5)
    CONNECTION("J4", 6,          "H3", 4)
    CONNECTION("J4", 8,          "H3", 3)

       //unused connections left over from indy 800 design
    CONNECTION("J4", 3,          "H4", 11)
    CONNECTION("J4", 11,         "H4", 10)
    CONNECTION("J4", 8,          "H4", 9)

    CONNECTION("J4", 6,          "H4", 4)
    CONNECTION("J4", 11,         "H4", 3)
    CONNECTION("J4", 8,          "H4", 5)

       //used
       //violet
    CONNECTION(_REDCARn,         "F5", 10)
    CONNECTION("H4", 12,         "F5", 9)
       
       //red
    CONNECTION("H4", 12,         "F5", 2)
    CONNECTION(_VIOLETCARn,      "F5", 1)

       //green
    CONNECTION(_CYANCARn,        "H5", 1)
    CONNECTION("H3", 6,          "H5", 2)

       //cyan
    CONNECTION("H3", 6,          "H5", 5)
    CONNECTION(_GREENCARn,       "H5", 4)
    
       //unused connections left over from indy 800 design
    CONNECTION(VCC,              "H5", 10)
    CONNECTION("H4", 8,          "H5", 9)

    CONNECTION("H4", 8,          "F5", 5)
    CONNECTION(VCC,              "F5", 4)

    CONNECTION(VCC,              "H5", 13)
    CONNECTION("H4", 6,          "H5", 12)

    CONNECTION("H4", 6,          "F5", 12)
    CONNECTION(VCC,              "F5", 13)

CIRCUIT_LAYOUT_END




/**************************************************************************
**** score board ****
**************************************************************************/
static CIRCUIT_LAYOUT( score )
    /* * * CHIPS * * */
    CHIP("A1", 74153)
    CHIP("A2", 9312)
    CHIP("A3", 7493)
    CHIP("A4", 7490)
    CHIP("A5", 9312)
    CHIP("A6", 7400)
    CHIP("B3", 9312)
    CHIP("B4", 7490)
    CHIP("B5", 9312)
    CHIP("B6", 7404)
    CHIP("C1", 7432)
    CHIP("C2", 9312)
    CHIP("C6", 7410)
    CHIP("D1", 74153)
    CHIP("D2", 7402)
    CHIP("D3", 9312)
    CHIP("D6", 9301)
    CHIP("E2", 7404)
    CHIP("E3", 7493)
    CHIP("E4", 7490)
    CHIP("E5", 9312)
    CHIP("E6", 7408)
    CHIP("F1", 7448)
    CHIP("F2", 7485)
    CHIP("F3", 9312)
    CHIP("F4", 7490)
    CHIP("F5", 9312)
    CHIP("F6", 7408)
    CHIP("H1", 9322)
    CHIP("H2", 74174)
    CHIP("H6", 7402)
    CHIP("J2", 7485)
    CHIP("J3", 9312)
    CHIP("J6", 7402)
    CHIP("K2", 74175)
    CHIP("K3", 9312)
    CHIP("L2", 7485)
    CHIP("L3", 9312)
    CHIP("CAP08",  CAPACITOR, &scorecap08_desc)
    CHIP("CAP10",  CAPACITOR, &scorecap10_desc)
    CHIP("CAP11",  CAPACITOR, &scorecap11_desc)
    CHIP("CAP12",  CAPACITOR, &scorecap12_desc)
    CHIP("CAP13",  CAPACITOR, &scorecap13_desc)
    CHIP("CAP14",  CAPACITOR, &scorecap14_desc)

    OPTIMIZATION_HINT("A6", 256, 256)
    OPTIMIZATION_HINT("B6", 256, 256)
    OPTIMIZATION_HINT("C6", 256, 256)

    /* * * CONNECTIONS * * */
    CONNECTION(GND,                     "F5", 9)
    CONNECTION(_CHPT4,                  "F5", 7)
    CONNECTION(GND,                     "F5", 6)
    CONNECTION(_CHPT2,                  "F5", 5)
    CONNECTION(_CHPT4,                  "F5", 4)
    CONNECTION(_CHPT3,                  "F5", 3)
    CONNECTION(_CHPT5,                  "F5", 2)
    CONNECTION(_CHPT1,                  "F5", 1)
    CONNECTION(_3C,                     "F5", 11)
    CONNECTION(_3B,                     "F5", 12)
    CONNECTION(_3A,                     "F5", 13)
    CONNECTION(_VIOLETCARn,             "F5", 10)
     
    CONNECTION(GND,                     "E5", 9)
    CONNECTION(_CHPT4,                  "E5", 7)
    CONNECTION(GND,                     "E5", 6)
    CONNECTION(_CHPT2,                  "E5", 5)
    CONNECTION(_CHPT4,                  "E5", 4)
    CONNECTION(_CHPT3,                  "E5", 3)
    CONNECTION(_CHPT5,                  "E5", 2)
    CONNECTION(_CHPT1,                  "E5", 1)
    CONNECTION(_4C,                     "E5", 11)
    CONNECTION(_4B,                     "E5", 12)
    CONNECTION(_4A,                     "E5", 13)
    CONNECTION(_REDCARn,                "E5", 10)
     
    CONNECTION(GND,                     "B5", 9)
    CONNECTION(_CHPT4,                  "B5", 7)
    CONNECTION(GND,                     "B5", 6)
    CONNECTION(_CHPT2,                  "B5", 5)
    CONNECTION(_CHPT4,                  "B5", 4)
    CONNECTION(_CHPT3,                  "B5", 3)
    CONNECTION(_CHPT5,                  "B5", 2)
    CONNECTION(_CHPT1,                  "B5", 1)
    CONNECTION(_7C,                     "B5", 11)
    CONNECTION(_7B,                     "B5", 12)
    CONNECTION(_7A,                     "B5", 13)
    CONNECTION(_GREENCARn,              "B5", 10)
     
    CONNECTION(GND,                     "A5", 9)
    CONNECTION(_CHPT4,                  "A5", 7)
    CONNECTION(GND,                     "A5", 6)
    CONNECTION(_CHPT2,                  "A5", 5)
    CONNECTION(_CHPT4,                  "A5", 4)
    CONNECTION(_CHPT3,                  "A5", 3)
    CONNECTION(_CHPT5,                  "A5", 2)
    CONNECTION(_CHPT1,                  "A5", 1)
    CONNECTION(_8C,                     "A5", 11)
    CONNECTION(_8B,                     "A5", 12)
    CONNECTION(_8A,                     "A5", 13)
    CONNECTION(_CYANCARn,               "A5", 10)
     
    CONNECTION(GND,                     "F4", 6)
    CONNECTION(GND,                     "F4", 7)
    CONNECTION(_SCORERESET,             "F4", 2)
    CONNECTION(_SCORERESET,             "F4", 3)
    CONNECTION(_3C,                     "F4", 14)
    CONNECTION("F5", 15,                "F4", 1)
     
    CONNECTION(GND,                     "E4", 6)
    CONNECTION(GND,                     "E4", 7)
    CONNECTION(_SCORERESET,             "E4", 2)
    CONNECTION(_SCORERESET,             "E4", 3)
    CONNECTION(_3D,                     "E4", 14)
    CONNECTION("E5", 15,                "E4", 1)
     
    CONNECTION(_SCORERESET,             "E3", 2)
    CONNECTION(_SCORERESET,             "E3", 3)
    CONNECTION(_3E,                     "E3", 14)
    CONNECTION(_4C,                     "E3", 1)
     
    CONNECTION(GND,                     "B4", 6)
    CONNECTION(GND,                     "B4", 7)
    CONNECTION(_SCORERESET,             "B4", 2)
    CONNECTION(_SCORERESET,             "B4", 3)
    CONNECTION(_7C,                     "B4", 14)
    CONNECTION("B5", 15,                "B4", 1)
     
    CONNECTION(GND,                     "A4", 6)
    CONNECTION(GND,                     "A4", 7)
    CONNECTION(_SCORERESET,             "A4", 2)
    CONNECTION(_SCORERESET,             "A4", 3)
    CONNECTION(_7D,                     "A4", 14)
    CONNECTION("A5", 15,                "A4", 1)
     
    CONNECTION(_SCORERESET,             "A3", 2)
    CONNECTION(_SCORERESET,             "A3", 3)
    CONNECTION(_7E,                     "A3", 14)
    CONNECTION(_8C,                     "A3", 1)
     
    CONNECTION(GND,                     "L3", 9)
    CONNECTION(GND,                     "L3", 7)
    CONNECTION(_7A,                     "L3", 6)
    CONNECTION(_4A,                     "L3", 5)
    CONNECTION(_8A,                     "L3", 4)
    CONNECTION(_3A,                     "L3", 3)
    CONNECTION(GND,                     "L3", 2)
    CONNECTION(GND,                     "L3", 1)
    CONNECTION(_256H,                   "L3", 11)
    CONNECTION(_64V,                    "L3", 12)
    CONNECTION(_128V,                   "L3", 13)
    CONNECTION(GND,                     "L3", 10)
     
    CONNECTION(GND,                     "K3", 9)
    CONNECTION(GND,                     "K3", 7)
    CONNECTION(_7B,                     "K3", 6)
    CONNECTION(_4B,                     "K3", 5)
    CONNECTION(_8B,                     "K3", 4)
    CONNECTION(_3B,                     "K3", 3)
    CONNECTION(GND,                     "K3", 2)
    CONNECTION(GND,                     "K3", 1)
    CONNECTION(_256H,                   "K3", 11)
    CONNECTION(_64V,                    "K3", 12)
    CONNECTION(_128V,                   "K3", 13)
    CONNECTION(GND,                     "K3", 10)
     
    CONNECTION(GND,                     "J3", 9)
    CONNECTION(GND,                     "J3", 7)
    CONNECTION(_7C,                     "J3", 6)
    CONNECTION(_4C,                     "J3", 5)
    CONNECTION(_8C,                     "J3", 4)
    CONNECTION(_3C,                     "J3", 3)
    CONNECTION(GND,                     "J3", 2)
    CONNECTION(GND,                     "J3", 1)
    CONNECTION(_256H,                   "J3", 11)
    CONNECTION(_64V,                    "J3", 12)
    CONNECTION(_128V,                   "J3", 13)
    CONNECTION(GND,                     "J3", 10)
     
    CONNECTION(GND,                     "F3", 9)
    CONNECTION(GND,                     "F3", 7)
    CONNECTION(_7D,                     "F3", 6)
    CONNECTION(_4D,                     "F3", 5)
    CONNECTION(_8D,                     "F3", 4)
    CONNECTION(_3D,                     "F3", 3)
    CONNECTION(GND,                     "F3", 2)
    CONNECTION(GND,                     "F3", 1)
    CONNECTION(_256H,                   "F3", 11)
    CONNECTION(_64V,                    "F3", 12)
    CONNECTION(_128V,                   "F3", 13)
    CONNECTION(GND,                     "F3", 10)
     
    CONNECTION(GND,                     "D3", 9)
    CONNECTION(GND,                     "D3", 7)
    CONNECTION(_7E,                     "D3", 6)
    CONNECTION(_4E,                     "D3", 5)
    CONNECTION(_8E,                     "D3", 4)
    CONNECTION(_3E,                     "D3", 3)
    CONNECTION(GND,                     "D3", 2)
    CONNECTION(GND,                     "D3", 1)
    CONNECTION(_256H,                   "D3", 11)
    CONNECTION(_64V,                    "D3", 12)
    CONNECTION(_128V,                   "D3", 13)
    CONNECTION(GND,                     "D3", 10)
     
    CONNECTION(GND,                     "B3", 9)
    CONNECTION(GND,                     "B3", 7)
    CONNECTION(_7F,                     "B3", 6)
    CONNECTION(_4F,                     "B3", 5)
    CONNECTION(_8F,                     "B3", 4)
    CONNECTION(_3F,                     "B3", 3)
    CONNECTION(GND,                     "B3", 2)
    CONNECTION(GND,                     "B3", 1)
    CONNECTION(_256H,                   "B3", 11)
    CONNECTION(_64V,                    "B3", 12)
    CONNECTION(_128V,                   "B3", 13)
    CONNECTION(GND,                     "B3", 10)
     
      //SCORE RESET
    CONNECTION(_SCORERESETn,            "B6", 9)

      //LEADER ID
    CONNECTION(_64H,                    "B6", 1)
     
    CONNECTION(_128H,                   "C6", 1)
    CONNECTION("B6", 2,                 "C6", 13)
    CONNECTION("F2", 5,                 "C6", 2)
     
    CONNECTION(GND,                     "J2", 10)
    CONNECTION(GND,                     "J2", 12)
    CONNECTION(_A,                      "J2", 13)
    CONNECTION(_B,                      "J2", 15)
    CONNECTION(GND,                     "J2", 4)
    CONNECTION(VCC,                     "J2", 3)
    CONNECTION(GND,                     "J2", 2)
    CONNECTION(GND,                     "J2", 9)
    CONNECTION(GND,                     "J2", 11)
    CONNECTION("H2", 2,                 "J2", 14)
    CONNECTION("H2", 5,                 "J2", 1)
     
    CONNECTION(_C,                      "F2", 10)
    CONNECTION(_D,                      "F2", 12)
    CONNECTION(_E,                      "F2", 13)
    CONNECTION(_F,                      "F2", 15)
    CONNECTION("J2", 5,                 "F2", 4)
    CONNECTION("J2", 6,                 "F2", 3)
    CONNECTION("J2", 7,                 "F2", 2)
    CONNECTION("H2", 7,                 "F2", 9)
    CONNECTION("H2", 10,                "F2", 11)
    CONNECTION("H2", 12,                "F2", 14)
    CONNECTION("H2", 15,                "F2", 1)
     
    CONNECTION(VCC,                     "K2", 4)
    CONNECTION(_256H,                   "K2", 5)
    CONNECTION(_64V,                    "K2", 12)
    CONNECTION(_128V,                   "K2", 13)
    CONNECTION("C6", 12,                "K2", 9)
    CONNECTION(_SCORERESETn,            "K2", 1)
     
    CONNECTION(_A,                      "H2", 3)
    CONNECTION(_B,                      "H2", 4)
    CONNECTION(_C,                      "H2", 6)
    CONNECTION(_D,                      "H2", 11)
    CONNECTION(_E,                      "H2", 13)
    CONNECTION(_F,                      "H2", 14)
    CONNECTION("C6", 12,                "H2", 9)
    CONNECTION(_SCORERESETn,            "H2", 1)

    CONNECTION(GND,                     "H1", 2)
    CONNECTION(_D,                      "H1", 3)
    CONNECTION(_A,                      "H1", 5)
    CONNECTION(_E,                      "H1", 6)
    CONNECTION(_B,                      "H1", 14)
    CONNECTION(_F,                      "H1", 13)
    CONNECTION(_C,                      "H1", 11)
    CONNECTION(GND,                     "H1", 10)
    CONNECTION(_16VXOR32H,              "H1", 1)
    CONNECTION(GND,                     "H1", 15)

    CONNECTION("H1", 4,                 "F1", 7)
    CONNECTION("H1", 7,                 "F1", 1)
    CONNECTION("H1", 12,                "F1", 2)
    CONNECTION("H1", 9,                 "F1", 6)
    CONNECTION("E2", 6,                 "F1", 4)
    CONNECTION(VCC,                     "F1", 3)  //not shown
    CONNECTION(VCC,                     "F1", 5)  //not shown

    CONNECTION(_FLASH,                  "L2", 4)
    CONNECTION(_FLASH,                  "L2", 3)
    CONNECTION(_FLASH,                  "L2", 2)
    CONNECTION(_NOLEADER,               "L2", 10)
    CONNECTION(_LEADERID1,              "L2", 12)
    CONNECTION(_LEADERID2,              "L2", 13)
    CONNECTION(_LEADERID3,              "L2", 15)
    CONNECTION(GND,                     "L2", 9)
    CONNECTION(_256H,                   "L2", 11)
    CONNECTION(_64V,                    "L2", 14)
    CONNECTION(_128V,                   "L2", 1)

    CONNECTION("L2", 6,                 "E2", 5)

      //SCORE
    CONNECTION(_4V,                     "E2", 9)

    CONNECTION("E2", 8,                 "D1", 1)
    CONNECTION(_e,                      "D1", 6)
    CONNECTION(_c,                      "D1", 5)
    CONNECTION(_f,                      "D1", 4)
    CONNECTION(_b,                      "D1", 3)

    CONNECTION("E2", 8,                 "D1", 15)
    CONNECTION(_b,                      "D1", 10)
    CONNECTION(_f,                      "D1", 11)
    CONNECTION(_c,                      "D1", 12 )
    CONNECTION(_e,                      "D1", 13)
    CONNECTION(_8V,                     "D1", 14)
    CONNECTION(_16H,                    "D1", 2)

    CONNECTION(_4V,                     "D2", 8)
    CONNECTION(_8V,                     "D2", 9)

    CONNECTION(_16H,                    "A2", 13)
    CONNECTION(_8H,                     "A2", 12)
    CONNECTION(_4H,                     "A2", 11)
    CONNECTION(VCC,                     "A2", 1)
    CONNECTION(VCC,                     "A2", 2)
    CONNECTION(GND,                     "A2", 3)
    CONNECTION(VCC,                     "A2", 4)
    CONNECTION(VCC,                     "A2", 5)
    CONNECTION(GND,                     "A2", 6)
    CONNECTION(VCC,                     "A2", 7)
    CONNECTION(VCC,                     "A2", 9)
    CONNECTION("D2", 10,                "A2", 10)
     
    CONNECTION("A2", 14,                "A1", 1)
    CONNECTION(_d,                      "A1", 6)
    CONNECTION(_g,                      "A1", 5)
    CONNECTION(_g,                      "A1", 4)
    CONNECTION(_a,                      "A1", 3)
    CONNECTION("A2", 14,                "A1", 15)
    CONNECTION(_a,                      "A1", 10)
    CONNECTION(_g,                      "A1", 11)
    CONNECTION(_g,                      "A1", 12)
    CONNECTION(_d,                      "A1", 13)
    CONNECTION(_8H,                     "A1", 14)
    CONNECTION(_16H,                    "A1", 2)

    CONNECTION("D1", 7,                 "C1", 2)
    CONNECTION("A1", 7,                 "C1", 1)

    CONNECTION("D1", 9,                 "C1", 5)
    CONNECTION("A1", 9,                 "C1", 4)

    CONNECTION(_SCOREWINDOWn,           "C2", 10)
    CONNECTION(GND,                     "C2", 1)
    CONNECTION(GND,                     "C2", 2)
    CONNECTION("C1", 6,                 "C2", 3)
    CONNECTION("C1", 3,                 "C2", 4)
    CONNECTION("C1", 6,                 "C2", 5)
    CONNECTION("C1", 3,                 "C2", 6)
    CONNECTION(GND,                     "C2", 7)
    CONNECTION(GND,                     "C2", 9)
    CONNECTION(_128V,                   "C2", 13)
    CONNECTION(_64V,                    "C2", 12)
    CONNECTION(_256H,                   "C2", 11)

    CONNECTION("C2", 14,                "CAP08", 1)

    CONNECTION(_256H,                   "D6", 15)
    CONNECTION(_64V,                    "D6", 14)
    CONNECTION(_128V,                   "D6", 1)
    CONNECTION(_SCOREn,                 "D6", 2)

      //VIDEO
        //WHITE
    CONNECTION(_8H,                     "A6", 10)
    CONNECTION(_16H,                    "A6", 9)

    CONNECTION(_4V,                     "A6", 5)
    CONNECTION(_8V,                     "A6", 4)

    CONNECTION("A6", 8,                 "J6", 6)
    CONNECTION("A6", 6,                 "J6", 5)

    CONNECTION(VCC,                     "B6", 13)

    CONNECTION("J6", 4,                 "A6", 1)
    CONNECTION("B6", 12,                "A6", 2)

    CONNECTION("A6", 3,                 "C6", 11)
    CONNECTION(VCC,                     "C6", 9)
    CONNECTION("A6", 11,                "C6", 10)

    CONNECTION(_RT,                     "B6", 3)

    CONNECTION("B6", 4,                 "C6", 3)
    CONNECTION(VCC,                     "C6", 5)
    CONNECTION(VCC,                     "C6", 4)

    CONNECTION(GND,                     "B6", 11)

    CONNECTION("C6", 6,                 "A6", 13)
    CONNECTION("B6", 10,                "A6", 12)
    

        //CAR VIDEO
        //violet
    CONNECTION(_VIOLETCARn,             "F6", 12)
    CONNECTION(_VIOLETSCOREn,           "F6", 13)

    CONNECTION(GND,                     "J6", 11)
    CONNECTION("F6", 11,                "J6", 12)

        //red 
    CONNECTION(_REDCARn,                "F6", 10)
    CONNECTION(_REDSCOREn,              "F6", 9)

    CONNECTION(GND,                     "J6", 8)
    CONNECTION("F6", 8,                 "J6", 9)

        //green
    CONNECTION(_GREENCARn,              "E6", 2)
    CONNECTION(_GREENSCOREn,            "E6", 1)

    CONNECTION(GND,                     "J6", 3)
    CONNECTION("E6", 3,                 "J6", 2)
        
        //cyan
    CONNECTION(_CYANCARn,               "E6", 10)
    CONNECTION(_CYANSCOREn,             "E6", 9)

    CONNECTION(GND,                     "H6", 6)
    CONNECTION("E6", 8,                 "H6", 5)

CIRCUIT_LAYOUT_END





/**************************************************************************
**** coin board ****
**************************************************************************/
static CIRCUIT_LAYOUT( coin )
    /* * * CHIPS * * */
    CHIP("A1", 7408)
    CHIP("A2", 7402)
    CHIP("A3", 7474)
    CHIP("A4", 7474)
    CHIP("A5", 7408)
    CHIP("B1", 7408)
    CHIP("B2", 7402)
    CHIP("B3", 7474)
    CHIP("B5", 7408)
    CHIP("C1", 7404)
    CHIP("C2", 7410)
    CHIP("C3", 7474)
    CHIP("C4", 74164)
    CHIP("C5", 7430)
    CHIP("D5", 7404)
    CHIP("E1", 7404)
    CHIP("E2", 7404)
    CHIP("E4", 7400)
    CHIP("F1", 7404)
    CHIP("F4", 7400)
    CHIP("F5", 7404)
    CHIP("H1", 7402)
    CHIP("J1", 7493)
    CHIP("K1", 7493)
    CHIP("L1", 555_Astable, &coinl1_555_desc)

    CHIP("CAP35",  CAPACITOR, &coincap35_desc)

    CHIP("$LATCH",       LATCH)
    CHIP("GREEN_LATCH",  LATCH)
    CHIP("CYAN_LATCH",   LATCH)
    CHIP("VIOLET_LATCH", LATCH)
    CHIP("RED_LATCH",    LATCH)
    
    CHIP("COIN_IN", COIN_INPUT)  // pin 1=active LO, pin i1=active HI
    
    /* * * CONNECTIONS * * */
      // dollar input 
    CONNECTION("COIN_IN", Coin::DOLLAR_HI, "F1", 3) //DOLLAR READER INPUT (coin pin 1=active LO, pin i1=active HI)
     
      // LM339 comparators used as latches. set when coin inserteded, cleared on game end.
    CONNECTION(_ENDGAMEn,                "$LATCH", 1)  //latch in:set
    CONNECTION(_DOLLARCREDITn,           "$LATCH", 2)  //latch in:reset

      //green
    CONNECTION("COIN_IN", Coin::COIN1_HI, "F1", 1)

    CONNECTION(_ENDGAMEn,                "GREEN_LATCH", 1)  //latch in:set
    CONNECTION("F1", 2,                  "GREEN_LATCH", 2)  //latch in:reset

    CONNECTION("GREEN_LATCH", 3,         "E4", 5)
    CONNECTION("$LATCH", 3,              "E4", 4)

    CONNECTION(_PLAY,                    "F4", 4)
    CONNECTION("E4", 6,                  "F4", 5)

    CONNECTION("E4", 6,                  "F5", 11)

    CONNECTION("E4", 6,                  "F5", 3)

      //cyan
    CONNECTION("COIN_IN", Coin::COIN2_HI, "F1", 5)

    CONNECTION(_ENDGAMEn,                "CYAN_LATCH", 1)  //latch in:set
    CONNECTION("F1", 6,                  "CYAN_LATCH", 2)  //latch in:reset

    CONNECTION("CYAN_LATCH", 3,          "E4", 2)
    CONNECTION("$LATCH", 3,              "E4", 1)

    CONNECTION(_PLAY,                    "F4", 1)
    CONNECTION("E4", 3,                  "F4", 2)

    CONNECTION("E4", 3,                  "F5", 9)

    CONNECTION("E4", 3,                  "F5", 5)

      //violet
    CONNECTION("COIN_IN", Coin::COIN3_HI, "E2", 3)

    CONNECTION(_ENDGAMEn,                "VIOLET_LATCH", 1)  //latch in:set
    CONNECTION("E2", 4,                  "VIOLET_LATCH", 2)  //latch in:reset

    CONNECTION("VIOLET_LATCH", 3,        "E4", 9)
    CONNECTION("$LATCH", 3,              "E4", 10)

    CONNECTION(_PLAY,                    "F4", 10)
    CONNECTION("E4", 8,                  "F4", 9)

    CONNECTION("E4", 8,                  "D5", 11)

    CONNECTION("E4", 8,                  "D5", 3)

      //red
    CONNECTION("COIN_IN", Coin::COIN4_HI, "E2", 1)

    CONNECTION(_ENDGAMEn,                "RED_LATCH", 1)  //latch in:set
    CONNECTION("E2", 2,                  "RED_LATCH", 2)  //latch in:reset

    CONNECTION("RED_LATCH", 3,           "E4", 12)
    CONNECTION("$LATCH", 3,              "E4", 13)

    CONNECTION(_PLAY,                    "F4", 13)
    CONNECTION("E4", 11,                 "F4", 12)

    CONNECTION("E4", 11,                 "F5", 13)

    CONNECTION("E4", 11,                 "F5", 1)

      //
    CONNECTION(_QGREENn,                 "C5", 11)
    CONNECTION(_QGREENn,                 "C5", 12)
    CONNECTION(_QCYANn,                  "C5", 6)
    CONNECTION(_QCYANn,                  "C5", 5)
    CONNECTION(_QVIOLETn,                "C5", 3)
    CONNECTION(_QVIOLETn,                "C5", 4)
    CONNECTION(_QREDn,                   "C5", 1)
    CONNECTION(_QREDn,                   "C5", 2)

    CONNECTION(_CREDIT,                  "E1", 3)

    CONNECTION("E1", 4,                  "CAP35", 1)

      //PLAY TIMER
    CONNECTION(VCC,                      "L1", 4)

    CONNECTION("L1", 3,                  "E1", 5)
     
    CONNECTION(_1SEC,                    "K1", 14)
    CONNECTION("K1", 12,                 "K1", 1)
    CONNECTION(_CREDITn,                 "K1", 2)
    CONNECTION(_CREDITn,                 "K1", 3)
     
    CONNECTION("K1", 11,                 "J1", 14)
    CONNECTION("J1", 12,                 "J1", 1)
    CONNECTION(_CREDITn,                 "J1", 2)
    CONNECTION(_CREDITn,                 "J1", 3)

    CONNECTION("K1", 11,                 "H1", 12)
    CONNECTION(_PLAY,                    "H1", 11)

    CONNECTION("H1", 13,                 "H1", 9)
    CONNECTION(_CREDITn,                 "H1", 8)
     
    CONNECTION("H1", 13,                 "C2", 10)
    //CONNECTION(_CREDITn,                 "C2", 11)
    CONNECTION(_CREDIT,                  "C2", 11) //wrong polarity on schematic? see indy 800
    CONNECTION(_ENDGAMEn,                "C2", 9)
     
    CONNECTION("C2", 8,                  "E2", 9)
     
    CONNECTION("E2", 8,                  "D5", 1)
     
    CONNECTION("J1", 11,                 "H1", 2)
    CONNECTION(_ENDGAME,                 "H1", 3)
    CONNECTION(_ENDGAMEn,                "H1", 5)
    CONNECTION(_1SEC,                    "H1", 6)
     
      //CRC (car reset clock)
    CONNECTION(_COMPRESETCLOCK,          "D5", 13) 
     
    CONNECTION(VCC,                      "C3", 4)
    CONNECTION(VCC,                      "C3", 2)
    CONNECTION("D5", 12,                 "C3", 3)
    CONNECTION(_VSYNCn,                  "C3", 1) 
     
    CONNECTION(_VSYNCn,                  "C4", 9)
    CONNECTION("C3", 5,                  "C4", 1)
    CONNECTION("C3", 5,                  "C4", 2)
    CONNECTION("D5", 12,                 "C4", 8)

      //CAR RESET 
    CONNECTION(_BOUNDS,                  "C2", 2) 
    CONNECTION(VCC,                      "C2", 1) // unconnected?
    CONNECTION(_DOLLARCREDITn,           "C2", 13)

      //green 
    CONNECTION(_GREENCARn,               "E1", 11) 
     
    CONNECTION("E1", 10,                 "A1", 2)
    CONNECTION("C2", 12,                 "A1", 1)
     
    CONNECTION(_GREENCOIN,               "A2", 5)
    CONNECTION("A1", 3,                  "A2", 6)
     
    CONNECTION(_QGREENn,                 "A5", 5)
    CONNECTION(_CREDIT,                  "A5", 4)
     
    CONNECTION("A2", 4,                  "A4", 4)
    CONNECTION("A5", 6,                  "A4", 2)
    CONNECTION(_CRCGREEN,                "A4", 3)
    CONNECTION(VCC,                      "A4", 1)

      //cyan 
    CONNECTION(_CYANCARn,                "E1", 9) 
     
    CONNECTION("E1", 8,                  "A1", 12)
    CONNECTION("C2", 12,                 "A1", 13)
     
    CONNECTION(_CYANCOIN,                "A2", 9)
    CONNECTION("A1", 11,                 "A2", 8)
     
    CONNECTION(_QCYANn,                  "A5", 9)
    CONNECTION(_CREDIT,                  "A5", 10)
     
    CONNECTION("A2", 10,                 "A4", 10)
    CONNECTION("A5", 8,                  "A4", 12)
    CONNECTION(_CRCCYAN,                 "A4", 11)
    CONNECTION(VCC,                      "A4", 13)

      //violet 
    CONNECTION(_VIOLETCARn,              "C1", 9)
     
    CONNECTION("C1", 8,                  "A1", 9)
    CONNECTION("C2", 12,                 "A1", 10)
     
    CONNECTION(_VIOLETCOIN,              "A2", 12)
    CONNECTION("A1", 8,                  "A2", 11)
     
    CONNECTION(_QVIOLETn,                "A5", 12)
    CONNECTION(_CREDIT,                  "A5", 13)
     
    CONNECTION("A2", 13,                 "A3", 10)
    CONNECTION("A5", 11,                 "A3", 12)
    CONNECTION(_CRCVIOLET,               "A3", 11)
    CONNECTION(VCC,                      "A3", 13)
     
      //red
    CONNECTION(_REDCARn,                 "C1", 11) 
     
    CONNECTION("C1", 10,                 "B1", 5)
    CONNECTION("C2", 12,                 "B1", 4)
     
    CONNECTION(_REDCOIN,                 "B2", 2)
    CONNECTION("B1", 6,                  "B2", 3)
     
    CONNECTION(_QREDn,                   "B5", 2)
    CONNECTION(_CREDIT,                  "B5", 1)
     
    CONNECTION("B2", 1,                  "B3", 4)
    CONNECTION("B5", 3,                  "B3", 2)
    CONNECTION(_CRCRED,                  "B3", 3)
    CONNECTION(VCC,                      "B3", 1)

CIRCUIT_LAYOUT_END



/**************************************************************************
**** car board ****
**************************************************************************/
static CIRCUIT_LAYOUT( car )
    /* * * CHIPS * * */
    CHIP("A1", 74107)
    CHIP("A2", 7400)
    CHIP("A3", 7404)
    //sound CHIP("A4", 7490)
    CHIP("B1", 7493)
    CHIP("B2", 7402)
    //sound CHIP("B3", 555)
    //sound CHIP("B4", 7492)
    CHIP("C1", 7400)
    CHIP("C2", 74193)
    CHIP("C3", 7420)
    CHIP("D1", 7475)
    CHIP("D2", 7485)
    CHIP("D3", 74153)
    CHIP("E1", 7400)
    CHIP("E2", 74193)
    CHIP("E3", 7474)
    CHIP("F1", 7474)
    CHIP("F2", 74193)
    CHIP("F3", 7402)
    CHIP("F4", 7406)
    CHIP("H1", 7400)
    CHIP("H2", 9316)
    CHIP("H3", 9316)
    CHIP("H4", 7414)
    CHIP("J1", 74107)
    CHIP("J2", 9316)
    CHIP("J3", 7404)
    CHIP("J4", 7404)
    CHIP("K1", 7410)
    CHIP("K2", 82S115, &cark2_desc)  //ROM 003186
    CHIP("K3", 9316)
    CHIP("K4", 82S123, &cark4_desc)  //ROM 003187
    CHIP("L1", 555_Mono, &carl1_555_desc)
    CHIP("L2", 9312)
    CHIP("L3", 9316)

    CHIP("CAP27",  CAPACITOR, &carcap27_desc)
    CHIP("CAP31",  CAPACITOR, &carcap31_desc)
    CHIP("CAP32",  CAPACITOR, &carcap32_desc)

    OPTIMIZATION_HINT("L2", 1024, 64)
    OPTIMIZATION_HINT("CAP31", 256, 256)
    OPTIMIZATION_HINT("H1", 32, 64)



    /* * * CONNECTIONS * * */
    CONNECTION(_CLOCKn,                 "J4", 3) 

      //STEERING
    //CONNECTION("WHEEL"*, Wheel::A,       "H4", 11)

    //CONNECTION("WHEEL"*, Wheel::B,       "H4", 13)

    //CONNECTION(_ATTRACT*,           "J4", 11) 
       
    CONNECTION(_CARATTRACTn,            "E3", 10)
    CONNECTION(_CARATTRACTn,            "E3", 12)
    CONNECTION("H4", 10,                "E3", 11)
    CONNECTION(_CARVTCn,                "E3", 13)

    CONNECTION(_CARATTRACTn,            "E3", 4)
    CONNECTION("H4", 10,                "E3", 2)
    CONNECTION("H4", 12,                "E3", 3)
    CONNECTION(VCC,                     "E3", 1)

    CONNECTION(_CARCOLLISION,           "E1", 5)
    CONNECTION(_CARVTC,                 "E1", 4)

    CONNECTION("E1", 6,                 "E1", 9)
    CONNECTION("E3", 8,                 "E1", 10)

    CONNECTION("E1", 8,                 "E1", 13)
    CONNECTION("E3", 5,                 "E1", 12)

    CONNECTION("E3", 6,                 "E1", 1)
    CONNECTION("E1", 8,                 "E1", 2)
     
    CONNECTION(VCC,                     "E2", 15)
    CONNECTION(GND,                     "E2", 1)
    CONNECTION(GND,                     "E2", 10)
    CONNECTION(GND,                     "E2", 9)
    CONNECTION(_CARRESETn,              "E2", 11)
    CONNECTION("E1", 11,                "E2", 5)
    CONNECTION("E1", 3,                 "E2", 4)
    CONNECTION(GND,                     "E2", 14)
     
    CONNECTION(GND,                      "F2", 15)
    CONNECTION(VCC,                      "F2", 1)
    CONNECTION(VCC,                      "F2", 10)
    CONNECTION(VCC,                      "F2", 9)
    CONNECTION(_CARRESETn,               "F2", 11)
    CONNECTION("E2", 12,                 "F2", 5)
    CONNECTION("E2", 13,                 "F2", 4)
    CONNECTION(GND,                      "F2", 14)
     
    CONNECTION(_CARSPEED4n,              "C1", 12)
    CONNECTION(_CARSPEED5n,              "C1", 13)

    CONNECTION("C1", 11,                 "C1", 10)
    //CONNECTION(_CARBRAKE,                "C1", 9)

    CONNECTION("C1", 8,                  "C1", 5)
    CONNECTION(_CARCOLLISIONn,           "C1", 4)

    CONNECTION("C1", 6,                  "C1", 1)
    CONNECTION("C1", 6,                  "C1", 2)
     
    CONNECTION(_CARR1,                   "D1", 2)
    CONNECTION(_CARR2,                   "D1", 3)
    CONNECTION(_CARR3,                   "D1", 6)
    CONNECTION(_CARR4,                   "D1", 7)
    CONNECTION(_CARSKIDn,                "D1", 4)
    CONNECTION(_CARSKIDn,                "D1", 13)

      //GAS&BRAKE
    CONNECTION(_CARVTC,                  "C3", 1)
    CONNECTION(_CAR1F,                   "C3", 5)
    CONNECTION(_CAR2F,                   "C3", 4)
    CONNECTION(_CAR4F,                   "C3", 2)

    CONNECTION(_CARSPEED5,               "C3", 13)
    CONNECTION(_CARSPEED4,               "C3", 9)
    CONNECTION(_CARSPEED3,               "C3", 10)
    CONNECTION(_CARSPEED2,               "C3", 12)

    CONNECTION("C3", 6,                  "D3", 1)
    CONNECTION("C3", 6,                  "D3", 15)
    //CONNECTION(_CARBRAKE,                "D3", 14) 
    //CONNECTION(_CARGAS,                  "D3", 2)  
    CONNECTION(GND,                      "D3", 6)
    CONNECTION(GND,                      "D3", 5)
    CONNECTION(_CAR8F,                   "D3", 4)
    CONNECTION(GND,                      "D3", 3)
    CONNECTION(VCC,                      "D3", 10)
    CONNECTION(VCC,                      "D3", 11)
    CONNECTION(GND,                      "D3", 12)
    CONNECTION(_CAR8F,                   "D3", 13)

    CONNECTION(GND,                      "B1", 2)
    CONNECTION(GND,                      "B1", 3)
    CONNECTION(_CAR128CV,                "B1", 14)
    CONNECTION(_CAR1F,                   "B1", 1)
     
    CONNECTION(VCC,                      "A1", 1)
    CONNECTION(_CAR8F,                   "A1", 12)
    CONNECTION(VCC,                      "A1", 4)
    CONNECTION(VCC,                      "A1", 13)
     
    CONNECTION("C3", 8,                  "A2", 5)
    CONNECTION("D3", 7,                  "A2", 4)
     
    CONNECTION("D3", 9,                  "B2", 5)
    CONNECTION("D3", 9,                  "B2", 6)
     
    CONNECTION(_CARSPEED1,               "A2", 2)
    CONNECTION(_CAR16Fn,                 "A2", 1)
     
    CONNECTION(GND,                      "C2", 15) //not shown
    CONNECTION(GND,                      "C2", 1) //not shown
    CONNECTION(GND,                      "C2", 10) //not shown
    CONNECTION(GND,                      "C2", 9) //not shown
    CONNECTION("A2", 6,                  "C2", 5)
    CONNECTION("B2", 4,                  "C2", 4)
    CONNECTION(VCC,                      "C2", 11)
    CONNECTION(_CARSTOP,                 "C2", 14)
     
    CONNECTION(_CARSPEED2,               "D2", 10)
    CONNECTION(_CARSPEED3,               "D2", 12)
    CONNECTION(_CARSPEED4,               "D2", 13)
    CONNECTION(_CARSPEED5,               "D2", 15)
    CONNECTION(_CAR8F,                   "D2", 9)
    CONNECTION(_CAR4F,                   "D2", 11)
    CONNECTION(_CAR2F,                   "D2", 14)
    CONNECTION(_CAR1F,                   "D2", 1)
    CONNECTION(VCC,                      "D2", 4)
    CONNECTION("A2", 3,                  "D2", 3)
    CONNECTION(VCC,                      "D2", 2)
     
    CONNECTION("C2", 12,                 "A2", 9)
    CONNECTION("C2", 13,                 "A2", 10)
     
    CONNECTION("B2", 1,                  "B2", 9)
    CONNECTION("B2", 1,                  "B2", 8)
     
    CONNECTION(VCC,                      "A1", 8)
    CONNECTION("A2", 8,                  "A1", 9)
    CONNECTION(VCC,                      "A1", 11)
    CONNECTION("B2", 1,                  "A1", 10)
     
    CONNECTION(_CARSPEED5,               "B2", 12)
    CONNECTION("C2", 13,                 "B2", 11)
     
    CONNECTION(_CARATTRACTn,             "A2", 12)
    CONNECTION(_CARCRASHn,               "A2", 13)
     
    CONNECTION("A2", 11,                 "B2", 3)
    CONNECTION("B2", 13,                 "B2", 2)

      //
    CONNECTION(_CARSPEED4,               "A3", 11)

      //COLLISION
    CONNECTION(_CARCRASHSEQn,            "K1", 5)
    CONNECTION(_CARCAR,                  "K1", 4)
    //CONNECTION(_OTHERCARS*,          "K1", 3) 
     
    CONNECTION(_CARCRASHSEQn,            "L1", 4)
    CONNECTION("K1",            6,       "L1", 2)
     
    CONNECTION(_CARCOLLISION,            "A3", 9)

      //CRASH SEQ
    CONNECTION(_RT,                      "CAP31", 1)
     
    CONNECTION(_CARCAR,                  "H1", 13)
    CONNECTION("CAP31", 2,               "H1", 12)
     
    CONNECTION(VCC,                      "F1", 4)
    CONNECTION(VCC,                      "F1", 2)
    CONNECTION(_CARVRESETn,              "F1", 3)
    CONNECTION("H1", 11,                 "F1", 1)

    CONNECTION(VCC,                      "F1", 10)
    CONNECTION("F1", 5,                  "F1", 12)
    CONNECTION(_CARVRESETn,              "F1", 11)
    CONNECTION(VCC,                      "F1", 13)
     
    CONNECTION(_CARCRASHSEQn,            "H1", 9)
    CONNECTION("F1", 6,                  "H1", 10)

      //CAR IMAGE
    CONNECTION(_BUFFEREDVRESET,          "CAP27", 1)
     
    CONNECTION("CAP27", 2,               "J4", 9)
     
    CONNECTION(_CARGO,                   "J3", 1)
     
    CONNECTION("J3", 2,                  "K4", 15)
    CONNECTION(_CARD4,                   "K4", 14)
    CONNECTION(_CARD3,                   "K4", 13)
    CONNECTION(_CARD2,                   "K4", 12)
    CONNECTION(_CARD1,                   "K4", 11)
    CONNECTION(_CARR0,                   "K4", 10)
     
    CONNECTION("K4", 9,                  "F3", 8)
    CONNECTION(_CARVRESETn,              "F3", 9)

    CONNECTION("K4", 7,                  "F3", 11)
    CONNECTION(_CARVRESETn,              "F3", 12)

    CONNECTION("K4", 6,                  "F3", 5)
    CONNECTION(_CARVRESETn,              "F3", 6)

    CONNECTION("K4", 5,                  "F3", 2)
    CONNECTION(_CARVRESETn,              "F3", 3)
     
    CONNECTION("F3", 13,                 "J3", 9)
     
    CONNECTION("F3", 1,                  "H3", 3)
    CONNECTION("F3", 4,                  "H3", 4)
    CONNECTION("J3", 8,                  "H3", 5)
    CONNECTION("F3", 10,                 "H3", 6)
    CONNECTION("H1", 3,                  "H3", 9)
    CONNECTION(VCC,                      "H3", 7)
    CONNECTION(VCC,                      "H3", 10)
    CONNECTION(_CARCLOCK,                "H3", 2)
    CONNECTION(_CARRESETn,               "H3", 1)
     
    CONNECTION(VCC,                      "J2", 3)
    CONNECTION(VCC,                      "J2", 4)
    CONNECTION(VCC,                      "J2", 5)
    CONNECTION(GND,                      "J2", 6)
    CONNECTION("H1", 3,                  "J2", 9)
    CONNECTION("H3", 15,                 "J2", 7)
    CONNECTION(VCC,                      "J2", 10)
    CONNECTION(_CARCLOCK,                "J2", 2)
    CONNECTION(_CARRESETn,               "J2", 1)
     
    CONNECTION(GND,                      "H2", 3)
    CONNECTION(GND,                      "H2", 4)
    CONNECTION(VCC,                      "H2", 5)
    CONNECTION(VCC,                      "H2", 6)
    CONNECTION("H1", 3,                  "H2", 9)
    CONNECTION("H3", 15,                 "H2", 7)
    CONNECTION("J2", 15,                 "H2", 10)
    CONNECTION(_CARCLOCK,                "H2", 2)
    CONNECTION(_CARRESETn,               "H2", 1)
     
    CONNECTION("H3", 15,                 "H1", 2)
    CONNECTION("H2", 15,                 "H1", 1)
     
    CONNECTION("K4", 3,                  "J3", 3)

    CONNECTION("K4", 2,                  "J3", 11)

    CONNECTION("K4", 1,                  "J3", 13)
     
    CONNECTION(_VCLOCKn,                 "J4", 1)

    //CONNECTION(_CARRESET*,             "J4", 5) 
     
    CONNECTION("J3", 12,                 "K3", 3)
    CONNECTION("J3", 10,                 "K3", 4)
    CONNECTION("J3", 4,                  "K3", 5)
    CONNECTION("K4", 4,                  "K3", 6)
    CONNECTION(_CARVTCn,                 "K3", 9)
    CONNECTION(VCC,                      "K3", 7)
    CONNECTION(VCC,                      "K3", 10)
    CONNECTION("J4", 2,                  "K3", 2)
    CONNECTION(_CARRESETn,               "K3", 1)
     
    CONNECTION(GND,                      "L3", 3)
    CONNECTION(GND,                      "L3", 4)
    CONNECTION(GND,                      "L3", 5)
    CONNECTION(GND,                      "L3", 6)
    CONNECTION(_CARVTCn,                 "L3", 9)
    CONNECTION("K3", 15,                 "L3", 7)
    CONNECTION(VCC,                      "L3", 10)
    CONNECTION("J4", 2,                  "L3", 2)
    CONNECTION(_CARRESETn,               "L3", 1)
     
    CONNECTION(VCC,                      "J1", 8)
    CONNECTION("L3", 11,                 "J1", 9)
    CONNECTION(VCC,                      "J1", 11)
    CONNECTION(_CARRESETn,               "J1", 10)
     
    CONNECTION("H2", 15,                 "K1", 1)
    CONNECTION("L3", 15,                 "K1", 2)
    CONNECTION("J1", 6,                  "K1", 13)

    CONNECTION("K3", 15,                 "K1", 9)
    CONNECTION("L3", 15,                 "K1", 10)
    CONNECTION("J1", 6,                  "K1", 11)
     
    CONNECTION(_CARVTCn,                 "J4", 13) //mislabeled as pin 5 on schematic
     
    //nn CONNECTION(GND,                      "K2", 12)
    CONNECTION(_CARC3,                   "K2", 18)
    CONNECTION(_CARC4,                   "K2", 21)
    CONNECTION(_CARL0,                   "K2", 22)
    CONNECTION(_CARL1,                   "K2", 23)
    CONNECTION(_CARL2,                   "K2", 1)
    CONNECTION(_CARR0,                   "K2", 2)
    CONNECTION(_CARR1,                   "K2", 3)
    CONNECTION(_CARR2,                   "K2", 4)
    CONNECTION(_CARR3,                   "K2", 5)
    CONNECTION(_CARR4,                   "K2", 6)
    //nn CONNECTION(GND,                      "K2", 20)
    //nn CONNECTION(VCC,                      "K2", 19)
     
    CONNECTION("K2", 7,                  "CAP32", 1)
     
    CONNECTION("CAP32", 2,               "L2", 1)
    CONNECTION("K2", 8,                  "L2", 2)
    CONNECTION("K2", 9,                  "L2", 3)
    CONNECTION("K2", 10,                 "L2", 4)
    CONNECTION("K2", 14,                 "L2", 5)
    CONNECTION("K2", 15,                 "L2", 6)
    CONNECTION("K2", 16,                 "L2", 7)
    CONNECTION("K2", 17,                 "L2", 9)
    CONNECTION(_CARCARWINDOWn,           "L2", 10)
    CONNECTION(_CARC0,                   "L2", 11)
    CONNECTION(_CARC1,                   "L2", 12)
    CONNECTION(_CARC2,                   "L2", 13)
     
    CONNECTION(_CARCLOCK,                "H4", 5)
     
    CONNECTION("L2", 15,                 "J1", 1)
    CONNECTION("H4", 6,                  "J1", 12)
    CONNECTION("L2", 14,                 "J1", 4)
    CONNECTION("H2", 15,                 "J1", 13)
     
    CONNECTION(_CARCAR,                  "F4", 5)
 
     //todo: motor freq, horn(!), audio
 
CIRCUIT_LAYOUT_END




/**************************************************************************
**** main ****
**************************************************************************/
CIRCUIT_LAYOUT( indy4 )
    
    SUB_CIRCUIT(SYNC, sync)
    SUB_CIRCUIT(SCORE, score)
    SUB_CIRCUIT(COIN, coin)

    SUB_CIRCUIT(GREEN, car)
    SUB_CIRCUIT(CYAN, car)
    SUB_CIRCUIT(VIOLET, car)
    SUB_CIRCUIT(RED, car)

    CHIP("GREEN.WHEEL",   WHEEL1_INPUT, &wheel1_desc)  
    CHIP("GREEN.BUTTONS", BUTTONS1_INPUT) //pin 1,2,3=active LO, pin i1,i2,i3=active HI

    CHIP("CYAN.WHEEL",   WHEEL2_INPUT, &wheel2_desc)  
    CHIP("CYAN.BUTTONS", BUTTONS2_INPUT)

    CHIP("VIOLET.WHEEL",   WHEEL3_INPUT, &wheel3_desc)  
    CHIP("VIOLET.BUTTONS", BUTTONS3_INPUT)

    CHIP("RED.WHEEL",   WHEEL4_INPUT, &wheel4_desc)  
    CHIP("RED.BUTTONS", BUTTONS4_INPUT)

    CHIP("POT1", POT_555_ASTABLE, &pot1_desc)
    POTENTIOMETER_CONNECTION("POT1", "COIN.L1")

    VIDEO(indy4)
    INPUT(indy4)

#ifdef DEBUG
	CHIP("LOG1", VCD_LOG, &vcd_log_desc)
#endif

    /* * * CONNECTIONS * * */

    // Green car specific connections
    CONNECTION("GREEN.WHEEL", Wheel::A,     GREEN"H4", 11)
    CONNECTION("GREEN.WHEEL", Wheel::B,     GREEN"H4", 13)

    CONNECTION(_CARBRAKE(GREEN),            GREEN"C1", 9)
    CONNECTION(_CARBRAKE(GREEN),            GREEN"D3", 14) 
    CONNECTION(_CARGAS(GREEN),              GREEN"D3", 2)  

    CONNECTION(_ATTRACTGREEN,               GREEN"J4", 11) 
    CONNECTION(_CARRESETGREEN,              GREEN"J4", 5) 
    CONNECTION(_OTHERCARSGREEN,             GREEN"K1", 3) 

    // Cyan car specific connections
    CONNECTION("CYAN.WHEEL", Wheel::A,     CYAN"H4", 11)
    CONNECTION("CYAN.WHEEL", Wheel::B,     CYAN"H4", 13)

    CONNECTION(_CARBRAKE(CYAN),            CYAN"C1", 9)
    CONNECTION(_CARBRAKE(CYAN),            CYAN"D3", 14) 
    CONNECTION(_CARGAS(CYAN),              CYAN"D3", 2)  

    CONNECTION(_ATTRACTCYAN,               CYAN"J4", 11) 
    CONNECTION(_CARRESETCYAN,              CYAN"J4", 5) 
    CONNECTION(_OTHERCARSCYAN,             CYAN"K1", 3) 


    // Violet car specific connections
    CONNECTION("VIOLET.WHEEL", Wheel::A,     VIOLET"H4", 11)
    CONNECTION("VIOLET.WHEEL", Wheel::B,     VIOLET"H4", 13)

    CONNECTION(_CARBRAKE(VIOLET),            VIOLET"C1", 9)
    CONNECTION(_CARBRAKE(VIOLET),            VIOLET"D3", 14) 
    CONNECTION(_CARGAS(VIOLET),              VIOLET"D3", 2)  

    CONNECTION(_ATTRACTVIOLET,               VIOLET"J4", 11) 
    CONNECTION(_CARRESETVIOLET,              VIOLET"J4", 5) 
    CONNECTION(_OTHERCARSVIOLET,             VIOLET"K1", 3) 


    // Red car specific connections
    CONNECTION("RED.WHEEL", Wheel::A,     RED"H4", 11)
    CONNECTION("RED.WHEEL", Wheel::B,     RED"H4", 13)

    CONNECTION(_CARBRAKE(RED),            RED"C1", 9)
    CONNECTION(_CARBRAKE(RED),            RED"D3", 14) 
    CONNECTION(_CARGAS(RED),              RED"D3", 2)  

    CONNECTION(_ATTRACTRED,               RED"J4", 11) 
    CONNECTION(_CARRESETRED,              RED"J4", 5) 
    CONNECTION(_OTHERCARSRED,             RED"K1", 3)


    //********************************************
    // OUTPUT
    //********************************************

    /* video (score pc board, right) */

    CONNECTION("VIDEO", Video::VBLANK_PIN, _VSYNC)
    CONNECTION("VIDEO", Video::HBLANK_PIN, _HSYNC)

    CONNECTION(_VIDEOWHITE,   "VIDEO", 1)
    // Yellow unused in Indy 4
    CONNECTION(_VIDEOCYAN,    "VIDEO", 3)
    CONNECTION(_VIDEOGREEN,   "VIDEO", 4)
    CONNECTION(_VIDEOVIOLET,  "VIDEO", 5) // Violet == Magenta ?
    CONNECTION(_VIDEORED,     "VIDEO", 6)
    // Blue unused in Indy 4
    // Peach unused in Indy 4


#ifdef DEBUG
    CONNECTION(_SCOREWINDOWn, "LOG1", 1)
    CONNECTION(_SCOREn, "LOG1", 2)
    CONNECTION(_GREENSCOREn, "LOG1", 3)
    CONNECTION(SCORE"C1", 3, "LOG1", 4)
    CONNECTION(SCORE"C1", 6, "LOG1", 5)
    CONNECTION(_128V, "LOG1", 6)
    CONNECTION(_64V, "LOG1", 7)
    CONNECTION(_256H, "LOG1", 8)

    /*CONNECTION("COINA4", 5, "LOG1", 1)
    CONNECTION(_BOUNDS, "LOG1", 2)
    CONNECTION("COINA2", 4, "LOG1", 3)
    CONNECTION("COINA5", 6, "LOG1", 4)
    CONNECTION("COINA1", 3, "LOG1", 5)
    CONNECTION("COINC2", 12, "LOG1", 6)
    CONNECTION(_QGREENn, "LOG1", 7)
    CONNECTION(_CREDIT, "LOG1", 8)
    CONNECTION(_CRCGREEN, "LOG1", 9)
    CONNECTION("LATCH", 3, "LOG1", 10)
    CONNECTION("COINE1", 10, "LOG1", 11)*/
#endif

CIRCUIT_LAYOUT_END


