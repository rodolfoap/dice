#include "../circuit_desc.h"
#include "../circuit.h"

#define DEBUG
#undef DEBUG

static Mono555Desc b8_555_desc(K_OHM(82.0), U_FARAD(1.0));
static Mono555Desc c8_555_desc(K_OHM(82.0), U_FARAD(1.0));
static Mono555Desc k10_555_desc(K_OHM(100.0), U_FARAD(1.0));

static Astable555Desc d8_555_desc(K_OHM(297.0), K_OHM(2.0), U_FARAD(5.0));
static Astable555Desc e8_555_desc(K_OHM(200.0), K_OHM(100.0), U_FARAD(1.0));

static Mono9602Desc b7_desc(K_OHM(47.0), U_FARAD(0.5), K_OHM(47.0), U_FARAD(50.0));

static BufferDesc buf1_desc(DELAY_NS(30.0), DELAY_NS(30.0));

static PotentimeterAstable555Desc pot1_desc("playtime", "Play Time", K_OHM(297.0), K_OHM(47.0), K_OHM(297.0), d8_555_desc);

#ifdef DEBUG
static VcdLogDesc vcd_log_desc
(
    "output_gotcha.vcd",
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

MixerDesc mixer1_desc({K_OHM(1.0)}, K_OHM(1.0));
MixerDesc mixer2_desc({K_OHM(1.0), K_OHM(1.5) /*Thevenin resistance*/});

static CUSTOM_LOGIC( proximity )
{
    // Thevenin voltage + resistance
    static const double v_th[4] = { 1.337, 1.521, 2.737, 3.815 };
    static const double r_th = K_OHM(1.5); // TODO: Figure this out more accurately?
    static const double rc = r_th*U_FARAD(0.1);

    static double v_cap = 0.0;
    static const double v_base = 2.0; // TODO: Make adjustable?

    double v = v_th[chip->inputs & 3]; 
    if(v > v_base) // Cutoff
        v = 4.8;

    double dt = double(chip->circuit->global_time - chip->last_output_event) * Circuit::timescale;
    double rc_exp = 1.0 - exp(-dt / rc);
    
    v_cap += (v - v_cap) * rc_exp;

    if(mask == 4)
    {
        e8_555_desc.ctrl = v_cap;
        chip->pending_event = chip->circuit->queue_push(chip, 1);
    }

    chip->last_output_event = chip->circuit->global_time;
    chip->inputs ^= mask;
}

extern CUSTOM_LOGIC( clock );

static CHIP_DESC( PROXIMITY ) = 
{
    // Timer to handle capacitor charging
    CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 2.0, 2.0 ),

    CUSTOM_CHIP_START(&proximity)
        INPUT_PINS( 1, 2, i1 )
        OUTPUT_PIN( i3 ),

   	CHIP_DESC_END
};

static AUDIO_DESC( gotcha )
    AUDIO_GAIN(10.0)
    AUDIO_SPEAKER_CONFIG(MONO)
AUDIO_DESC_END

static INPUT_DESC( gotcha )
    INPUT_INFO(JOYSTICK1_INPUT, "Move Cross")
    INPUT_INFO(JOYSTICK2_INPUT, "Move Square")

    INPUT_INFO(COIN_INPUT, {{ 1 }}, "Insert Coin")
    INPUT_INFO(START_INPUT, {{ 1 }}, "Start Game")
INPUT_DESC_END

CIRCUIT_LAYOUT( gotcha )
    /**************************************************************************
		Chips
	**************************************************************************/
    CHIP("CLOCK", CLOCK_14_318_MHZ)
    
    CHIP("A1",7402)
    CHIP("A10",7400)

    CHIP("B1",7420)
    CHIP("B2",7474)
    CHIP("B3",7474)
    CHIP("B4",74107)
    CHIP("B5",74107)
    CHIP("B6",7474)
    CHIP("B7",9602, &b7_desc)
    CHIP("B8",555_Mono,&b8_555_desc)
    
    CHIP("C2",7474)
    CHIP("C3",7474)
    CHIP("C4",9316)
    CHIP("C5",7400)
    CHIP("C6",7474)
    CHIP("C7",7474)
    CHIP("C8",555_Mono,&c8_555_desc)
    
    CHIP("D1",9316)
    CHIP("D2",7486)
    CHIP("D3",7402)
    CHIP("D4",9316)
    CHIP("D5",74107)
    CHIP("D6",7400)
    CHIP("D7",7404)
	CHIP("D8", 555_Astable, &d8_555_desc)	
	    
    CHIP("E1",7400)
    CHIP("E2",7400)
    CHIP("E3",9316)
    CHIP("E4",7400)
    CHIP("E5",7430)
    CHIP("E6",7427)
    CHIP("E7",7410)
    CHIP("E8", 555_Astable, &e8_555_desc)
    
    CHIP("F1",7402)
    CHIP("F2",7474)
    CHIP("F3",9316)
    CHIP("F4",7474)
    CHIP("F5",7493)
    CHIP("F6",7402)
    CHIP("F7",7410)
    CHIP("F8",7430)
    
    CHIP("G1",9316)
    CHIP("G8",7490)
    
    CHIP("H1",7404)
    CHIP("H2",7420)
    CHIP("H3",9316)
    CHIP("H4",7410)
    CHIP("H5",7493)
    CHIP("H6",7474)
    CHIP("H7",7410)
    CHIP("H8",7404)
    
    CHIP("I7",74157)
    
    CHIP("J1",7410)
    CHIP("J2",74107)
    CHIP("J3",9316)
    CHIP("J4",7408)
    CHIP("J5",7404)
    CHIP("J6",74107)
    CHIP("J7",7448)
    CHIP("J8",74107)
    CHIP("J10",7400)
    
    CHIP("K1",9316)
    CHIP("K2",7486)
    CHIP("K3",7402)
    CHIP("K4",7402)
    CHIP("K5",7486)
    CHIP("K6",7430)
    CHIP("K7",7474)
    CHIP("K8",7490)
    CHIP("K10",555_Mono, &k10_555_desc)
    
    CHIP("L1",9316)
    CHIP("L2",7474)
    CHIP("L3",7474)
    CHIP("L4",7402)
    CHIP("L5",7400)
    CHIP("L6",7493)
    CHIP("L7",74153)
    CHIP("L8",7490)
    
    CHIP("M1",7402)
    CHIP("M2",7474)
    CHIP("M3",7474)
    CHIP("M4",7400)
    CHIP("M5",7400)
    CHIP("M6",7493)
    CHIP("M7",74153)
    
    CHIP("XY",7410) // Right wall collision fix
    CHIP("BUF1",BUFFER,&buf1_desc) // Catch signal delay
    
    CHIP("LATCH", LATCH)

    CHIP("COIN1", COIN_INPUT)
    CHIP("START1", START_INPUT)

    CHIP("STICK1", JOYSTICK1_INPUT)
    CHIP("STICK2", JOYSTICK2_INPUT)

    CHIP("POT1", POT_555_ASTABLE, &pot1_desc)
    POTENTIOMETER_CONNECTION("POT1", "D8") 

    CHIP("PROXIMITY", PROXIMITY)
    POTENTIOMETER_CONNECTION("PROXIMITY", "E8") 

    CHIP("MIXER1", MIXER, &mixer1_desc)
    CHIP("MIXER2", MIXER, &mixer2_desc)

    CHIP("CLK_GATE1", CLK_GATE) // Speed hack
    //CHIP("CLK_GATE2", CLK_GATE)

    OPTIMIZATION_HINT("J4", 64, 64)

    AUDIO(gotcha)
    INPUT(gotcha)

#ifdef DEBUG
	CHIP("LOG1", VCD_LOG, &vcd_log_desc)
#endif

    /**************************************************************************
		Signals
	**************************************************************************/	
    #define CLK "J6", 3
    #define CLK_n "J6", 2

    #define H1  "L6", 12
    #define H2  "L6", 9
    #define H4  "L6", 8
    #define H8  "L6", 11
    #define H16  "M6", 12
    #define H32  "M6", 9
    #define H64  "M6", 8
    #define H128  "M6", 11	
    #define H256  "J6", 5
    #define H256_n  "J6", 6
    
    #define V1  "H5", 12
    #define V2  "H5", 9
    #define V4  "H5", 8
    #define V8  "H5", 11
    #define V16  "F5", 12
    #define V32  "F5", 9
    #define V64  "F5", 8
    #define V128  "F5", 11
    #define V256  "D5", 3
    #define V256_n  "D5", 2
    
    #define COIN "D7", 2
    #define COIN_n "D7", 12
    #define START "B6", 5
    #define START_n "B6", 6
    #define ATTRACT "C7", 9
    #define ATTRACT_n "C7", 8
	 
    #define M1 "J5", 6
    #define M2 "H8", 6
    #define M4 "H4", 6
    #define S "B5", 5
    
    #define A "E4", 3  
    #define B "D3", 4
    #define C "B3", 9
    #define D "D2", 8
    
    #define F "K3", 10
    #define E "M4", 8
    #define G "M2", 5
    #define H "K2", 6
    
    #define J "E2", 6
    #define K "K2", 3
    #define L "G1", 13
    #define M "E3", 12
    #define N "L1", 13
    #define O "J3", 13
    #define Q "J5", 12
    #define X "D1", 11
    #define Y "J2", 6
    
    #define U "B5", 3
    
    #define BUMP1 "H4", 8
    #define BUMP2 "J1", 8
    #define CATCH_n "E1", 3
    #define VIDEO1 "F1", 1
    #define VVIDEO1_n "B1", 6
    #define HBLANK_n "M5", 6
    #define VBLANK_n "F6", 4
    #define VRESET "H6", 8
    #define VRESET_n "H6", 9
    #define CATCHOS "B7", 10
    #define CATCHOS_n  "B7", 9
    #define MAZE "J4", 3
    #define VIDEO2 "F1", 4
    #define HLD1 "E2", 8
    #define HLD2 "J1", 12
    #define PRES "E5", 8
    #define OO "E1", 11
    #define HSYNC_n "F4", 6
    #define MHBLANK_n "D5", 6
    #define HBLANK "M5", 8
    

 	/**************************************************************************
		Layout
	**************************************************************************/
    /* H-Counter */
    CONNECTION("CLOCK", 1, "J6", 12)
    CONNECTION(VCC, "J6", 1)
    CONNECTION(VCC, "J6", 4)
    CONNECTION(VCC, "J6", 13)

    CONNECTION(CLK,"L6", 14)
    CONNECTION("L6", 12,"L6", 1)
 
    CONNECTION("L6", 11,"M6", 14)
    CONNECTION("M6", 12,"M6", 1)
    CONNECTION("M6", 11,"J6", 9)
    CONNECTION(VCC,"J6", 8)
    CONNECTION(VCC,"J6", 11)
 
    CONNECTION("L6", 12,"K6", 1)
    CONNECTION("L6", 9,"K6", 5)
    CONNECTION("L6", 8,"K6", 6)
    CONNECTION("M6", 8,"K6", 4)
    CONNECTION("M6", 11,"K6", 3)
    CONNECTION("J6", 5,"K6", 11)
    CONNECTION(VCC,"K6", 2)
    CONNECTION(VCC,"K6", 12)
 
    CONNECTION("K6", 8,"H6", 2)
    CONNECTION(CLK,"H6", 3)
    CONNECTION(VCC,"H6", 4)
    CONNECTION(VCC,"H6", 1)
    CONNECTION("H6", 5,"J6", 10)
    CONNECTION("H6", 6,"L6", 2)
    CONNECTION("H6", 6,"L6", 3)
    CONNECTION("H6", 6,"M6", 2)
    CONNECTION("H6", 6,"M6", 3)
    CONNECTION("H6", 6,"H5", 14)
    
    /* V-Counter */
    CONNECTION("H5", 12,"H5", 1)
    CONNECTION("H5", 11,"F5", 14)
    
    CONNECTION("F5", 12,"F5", 1)
    CONNECTION("F5", 11,"D5", 12)
    CONNECTION(VCC,"D5", 1)
    CONNECTION(VCC,"D5", 4)
    
    CONNECTION("H5", 12,"H4", 2)
    CONNECTION("H5", 8,"H4", 13)
    CONNECTION("D5", 3,"H4", 1)
    CONNECTION("H4", 12,"H6", 12)
    
    CONNECTION(VCC,"H6", 10)
    CONNECTION(VCC,"H6", 13)
    CONNECTION("H6", 6,"H6", 11)
    CONNECTION("H6", 9,"D5", 13)
    CONNECTION("H6", 8,"H5", 2)
    CONNECTION("H6", 8,"H5", 3)
    CONNECTION("H6", 8,"F5", 2)
    CONNECTION("H6", 8,"F5", 3)
    
    /* Hsync */
    CONNECTION(H16,"M5", 1)
    CONNECTION(H64,"M5", 2)
    CONNECTION("H6", 5,"M5", 10)
    CONNECTION("M5", 8,"M5", 5)
    CONNECTION("M5", 6,"M5", 9)
    CONNECTION("M5", 3,"M5", 4)
    
    CONNECTION(H64,"J5", 3)
    CONNECTION("J5", 4,"L5", 5)
    CONNECTION(H32,"L5", 4)
    CONNECTION("J5", 4,"J4", 12)
    CONNECTION("M5", 8,"J4", 13)
    
    CONNECTION("L5", 6,"F4", 2)
    CONNECTION("J4", 11,"F4", 1)
    CONNECTION(H2, "F4", 3)
    CONNECTION(VCC,"F4", 4)
    
    /* Vsync */
    CONNECTION("H6", 8,"F6", 6)
    CONNECTION("F6", 4,"F6", 9)
    CONNECTION("F6", 10,"F6", 5)
    CONNECTION(V16,"F6", 8)
    CONNECTION("F6", 10,"E7", 3)
    CONNECTION(V4,"E7", 5)
    CONNECTION(V8,"E7", 4) 
    
    /* Coin */

   	CONNECTION("COIN1", 1, "D7", 1)
	CONNECTION("D7", 2,"D7", 13)
	
	CONNECTION("D7", 12,"B7", 5)
	CONNECTION(GND,"B7", 4)
	CONNECTION(VCC,"B7", 3)
	CONNECTION("B7", 7,"K7", 3)
	CONNECTION("D7", 2,"K7", 2)
	CONNECTION("D7", 2,"K7", 1)
	CONNECTION(VCC,"K7", 4)
	
	CONNECTION("LATCH", 3,"J5", 13)
	CONNECTION("K7", 6, "LATCH", 2)
	CONNECTION("D6", 11, "LATCH", 1)
	
	CONNECTION("START1", 1,"B6", 11)
	CONNECTION("B6", 9,"B6", 2)
	CONNECTION(VCC,"B6", 10)
	CONNECTION(VCC,"B6", 4)
	CONNECTION(COIN_n,"B6", 1)
	CONNECTION("B6", 6,"B6", 13)
	CONNECTION(ATTRACT,"B6", 12)
	CONNECTION(V256,"B6", 3)

    CONNECTION(ATTRACT,"C6", 11)
    CONNECTION(ATTRACT,"C6", 3)
	CONNECTION(VCC,"C6", 12)
	CONNECTION(VCC,"C6", 10)   /* Should be connected to dip switch */
	CONNECTION("C6", 9,"C6", 2)
	CONNECTION(Q,"C6", 4)    
	CONNECTION("K7", 6,"C6", 13)
	CONNECTION("K7", 6,"C6", 1)
	CONNECTION("C6", 5,"D6", 13)
	CONNECTION(ATTRACT,"D6", 12)
	CONNECTION("C6", 6,"D6", 9)
	CONNECTION(START,"D6", 10)
	
	CONNECTION("D6", 8,"C7", 13)
	CONNECTION("L8", 11,"D7", 11)
	CONNECTION("D7", 10,"C7", 11)
	CONNECTION(VCC,"C7", 12)
	CONNECTION(Q,"C7", 10)

    /* Playfield */
    CONNECTION(H4,"D6", 4)
    CONNECTION(H8,"D6", 5)
    
    CONNECTION(H32,"M5", 12)
    CONNECTION(H64,"M5", 13)
    CONNECTION("M5", 11,"E6", 5)
    CONNECTION(V64,"F4", 11)
    CONNECTION(VCC,"F4", 12)
    CONNECTION(VCC,"F4", 10)
    CONNECTION("H6", 9,"F4", 13)
    CONNECTION("F4", 8,"E6", 4)
    CONNECTION("D6", 6,"E6", 3)
    CONNECTION(H16,"K5", 1)
    CONNECTION(H256,"K5", 2)
    CONNECTION("K5", 3,"K4", 3)
    CONNECTION(H128,"K4", 2)
    
    CONNECTION("E6", 6,"C5", 1)
    CONNECTION("K4", 1,"C5", 2)
    
    CONNECTION(VCC,"B4", 8)
    CONNECTION("F4", 8,"B4", 9)
    CONNECTION(GND,"B4", 11)
    CONNECTION("B4", 5,"E4", 12)
    CONNECTION(V2,"E4", 13)
    
    CONNECTION(V4,"J5", 11)
    CONNECTION("J5", 10,"B4", 10)
    CONNECTION("J5", 10,"K4", 12)
    CONNECTION(V256_n,"K4", 11)
    CONNECTION("K4", 13,"J5", 9)
    
    CONNECTION("E4", 11,"C5", 13) 
    CONNECTION("J5", 8,"C5", 12)
    
    CONNECTION("C5", 3,"D5", 9)
    CONNECTION(VCC,"D5", 8)
    CONNECTION(VCC,"D5", 11)
    CONNECTION("H6", 5,"D5", 10)
    CONNECTION("D5", 5,"C5", 9)
    CONNECTION("C5", 11,"C5", 10)
    CONNECTION("C5", 8,"C5", 5)
    CONNECTION("C5", 3,"C5", 4)
    CONNECTION("C5", 6,"H8", 11)
    
    /* Play timer */
    CONNECTION(ATTRACT_n,"D8", 4)   
    CONNECTION("D8", 3,"K8", 14)
    CONNECTION("K8", 12,"K8", 1)
    CONNECTION(GND,"K8", 6)
    CONNECTION(GND,"K8", 7)   
    CONNECTION(START,"K8", 2)
    CONNECTION(START,"K8", 3)
    
    CONNECTION("K8", 11,"L8", 14)
    CONNECTION("L8", 12,"L8", 1)
    CONNECTION(GND,"L8", 6)
    CONNECTION(GND,"L8", 7)   
    CONNECTION(START,"L8", 2)
    CONNECTION(START,"L8", 3)
    
    CONNECTION(CATCHOS_n,"G8", 14)
    CONNECTION("G8", 12,"G8", 1)
    CONNECTION(GND,"G8", 6)
    CONNECTION(GND,"G8", 7)   
    CONNECTION(START,"G8", 2)
    CONNECTION(START,"G8", 3)
    
    CONNECTION("G8", 11,"J8", 12)
    CONNECTION("J8", 3,"J8", 9)
    CONNECTION(VCC,"J8", 1)
    CONNECTION(VCC,"J8", 4)
    CONNECTION(VCC,"J8", 8)
    CONNECTION(VCC,"J8", 11)
    CONNECTION(START_n,"J8", 10) 
    CONNECTION(START_n,"J8", 13) 
    
    
    /* Playfield mux */
    CONNECTION("J8", 3,"L7", 6)
    CONNECTION("G8", 12,"L7", 5)
    CONNECTION("L8", 12,"L7", 4)
    CONNECTION("K8", 12,"L7", 3)
    CONNECTION("J8", 5,"L7", 10)
    CONNECTION("G8", 9,"L7", 11)
    CONNECTION("L8", 9,"L7", 12)
    CONNECTION("K8", 9,"L7", 13)
    CONNECTION(H256,"L7", 2)
    CONNECTION(H32,"L7", 14)
    CONNECTION(GND,"L7", 1)
    CONNECTION(GND,"L7", 15)
    
    CONNECTION(GND,"M7", 6)
    CONNECTION("G8", 8,"M7", 5)
    CONNECTION("L8", 8,"M7", 4)
    CONNECTION("K8", 8,"M7", 3)
    CONNECTION(GND,"M7", 10)
    CONNECTION("G8", 11,"M7", 11)
    CONNECTION("L8", 11,"M7", 12)
    CONNECTION("K8", 11,"M7", 13)
    CONNECTION(H256,"M7", 2)
    CONNECTION(H32,"M7", 14)
    CONNECTION(GND,"M7", 1)
    CONNECTION(GND,"M7", 15)
    
    /* M1,M3 */
    CONNECTION(V32,"J5", 5)
    CONNECTION("J5", 6,"K5", 5)
    CONNECTION(H32,"K5", 4)
    
    /* M4 */
    CONNECTION(V256,"B4", 12)
    CONNECTION(VCC,"B4", 4)
    CONNECTION(VCC,"B4", 13)
    CONNECTION("B4", 3,"B5", 8)
    CONNECTION(V256,"B5", 9)
    CONNECTION(VCC,"B5", 11)
    CONNECTION(VCC,"B5", 10)
    CONNECTION("B5", 6,"B4", 1)
    CONNECTION("F6", 4,"C4", 10)
    CONNECTION(VCC,"C4", 7)
    CONNECTION(VCC,"C4", 1)
    CONNECTION("F4", 6,"C4", 2)
    CONNECTION("B5", 5,"C4", 3)
    CONNECTION("B5", 6,"C4", 4)
    CONNECTION(GND,"C4", 5)
    CONNECTION(VCC,"C4", 6)
    
    CONNECTION("C4", 15,"D4", 7)
    CONNECTION("F4", 6,"D4", 2)
    CONNECTION(VCC,"D4", 1)
    CONNECTION(VCC,"D4", 10)
    CONNECTION(GND,"D4", 3)
    CONNECTION(GND,"D4", 4)
    CONNECTION(GND,"D4", 5)
    CONNECTION(GND,"D4", 6)
    
    CONNECTION("C4", 15,"E4", 9)
    CONNECTION("D4", 15,"E4", 10)
    CONNECTION("E4", 8,"C4", 9)
    CONNECTION("E4", 8,"D4", 9)
    
    CONNECTION("D4", 11,"H4", 5)
    CONNECTION(H256,"L5", 2)
    CONNECTION(H32,"L5", 1)
    CONNECTION("L5", 3,"H4", 4)
    CONNECTION(V256,"K5", 10)
    CONNECTION("B4", 6,"K5", 9)
    CONNECTION("K5", 8,"H4", 3)
        
    /* Playfield mux part 2 */
    CONNECTION("F4", 8,"B1", 9)
    CONNECTION(V32,"B1", 13)
    CONNECTION(H64,"J5", 1)
    CONNECTION("J5", 2,"B1", 10)
    CONNECTION(H128,"L5", 12)
    CONNECTION(H256,"L5", 13)
    CONNECTION("L5", 11,"B1", 12)
    CONNECTION("B1", 8,"I7", 1)
    CONNECTION(GND,"I7", 15)
    CONNECTION("L7", 7,"I7", 2)
    CONNECTION("J5", 6,"I7", 3)
    CONNECTION("H8", 6,"I7", 6)
    CONNECTION("L7", 9,"I7", 5)
    CONNECTION("M7", 7,"I7", 11)
    CONNECTION("K5", 6,"I7", 10)
    CONNECTION("M7", 9,"I7", 14)
    CONNECTION(M4,"I7", 13)
    
    CONNECTION(VCC,"J7", 3)
    CONNECTION(H32,"J7", 5)
    CONNECTION("I7", 4,"J7", 7)
    CONNECTION("I7", 7,"J7", 1)
    CONNECTION("I7", 9,"J7", 2)
    CONNECTION("I7", 12,"J7", 6)
    CONNECTION("B1", 8,"L5", 10)
    CONNECTION("D5", 6,"L5", 9)
    CONNECTION("L5", 8,"J7", 4)
    
    CONNECTION(H16,"D7", 5)
    CONNECTION("D7", 6,"F6", 2)
    CONNECTION("D6", 6,"F6", 3)
    CONNECTION(H8,"E6", 9)
    CONNECTION(H4,"E6", 10)
    CONNECTION("D7", 6,"E6", 11)
    CONNECTION("D7", 6,"E6", 13)
    CONNECTION(V8,"E6", 2)
    CONNECTION(V4,"E6", 1)
    CONNECTION(V4,"E7", 9)
    CONNECTION(V8,"E7", 10)
    CONNECTION(H16,"E7", 11)
    CONNECTION("E7", 8,"H8", 9)
    CONNECTION(V16,"H8", 5)
    
    CONNECTION("J7", 15,"H7", 2) 
    CONNECTION("E6", 8,"H7", 13)
    CONNECTION("H8", 6,"H7", 1)
    
    CONNECTION("J7", 9,"H7", 9)
    CONNECTION(V16,"H7", 10)
    CONNECTION("E6", 8,"H7", 11)
    
    CONNECTION("J7", 12,"F7", 5)
    CONNECTION("F6", 1,"F7", 4)
    CONNECTION("H8", 6,"F7", 3)

    CONNECTION("J7", 11,"H7", 5)
    CONNECTION("F6", 1,"H7", 4)
    CONNECTION(V16,"H7", 3)
    
    CONNECTION("J7", 13,"E7", 2)
    CONNECTION("H8", 6,"E7", 1)
    CONNECTION("E6", 12,"E7", 13)
    
    CONNECTION("J7", 14,"F7", 1)
    CONNECTION("H8", 8,"F7", 13)
    CONNECTION("H8", 6,"F7", 2)
    
    CONNECTION("J7", 10,"F7", 9)
    CONNECTION("H8", 8,"F7", 11)
    CONNECTION(V16,"F7", 10)
    
    CONNECTION("H7", 12,"F8", 5)
    CONNECTION("H7", 8,"F8", 6)
    CONNECTION("F7", 6,"F8", 3) 
    CONNECTION("H7", 6,"F8", 4)
    CONNECTION("E7", 12,"F8", 12)
    CONNECTION("F7", 12,"F8", 11)
    CONNECTION("F7", 8,"F8", 2)
    CONNECTION("H8", 10,"F8", 1)
    
    CONNECTION("F8", 8,"D6", 2)
    CONNECTION(CATCHOS_n,"D6", 1)
    CONNECTION("D6", 3,"F6", 12)
    CONNECTION(HBLANK,"F6", 11)
    CONNECTION("F8", 8,"J4", 2)
    CONNECTION(CLK_n, "J4", 1)
  
   
    /* Right Control */
    CONNECTION(V256,"B5", 12)
    CONNECTION(VCC,"B5", 1)
    CONNECTION(VCC,"B5", 4)
    CONNECTION(VCC,"B5", 13)

    CONNECTION(BUMP1,"D3", 11)
    CONNECTION(BUMP1,"D3", 9)
    CONNECTION(J,"H1", 3)
    CONNECTION(J,"D3", 8)
    CONNECTION("H1", 4,"D3", 12)
    CONNECTION("D3", 13,"C3", 3)
    CONNECTION(VCC,"C3", 2)
    CONNECTION(VCC,"C3", 4)
    CONNECTION(VRESET_n,"C3", 1)
    CONNECTION("D3", 10,"C3", 11)
    CONNECTION(VCC,"C3", 12)
    CONNECTION(VCC,"C3", 10)
    CONNECTION(VRESET_n,"C3", 13)
    
    CONNECTION("STICK1", Joystick::LEFT,"A1", 5)
    CONNECTION("STICK1", Joystick::RIGHT,"A1", 2)
    CONNECTION("STICK1", Joystick::UP, "A1", 8)
    CONNECTION("STICK1", Joystick::DOWN,"A1", 11)

    /*CONNECTION(U,"A1", 6)
    CONNECTION(U,"A1", 3)
    CONNECTION(U,"A1", 9)
    CONNECTION(U,"A1", 12)*/
    // U causes right player to move slower. Is this a configurable option?
    CONNECTION(S,"A1", 6)
    CONNECTION(S,"A1", 3)
    CONNECTION(S,"A1", 9)
    CONNECTION(S,"A1", 12)
    
    CONNECTION("A1", 4,"B2", 12)
    CONNECTION("E4", 6,"B2", 11)
    CONNECTION("C3", 8,"B2", 10)
    CONNECTION(VCC,"B2", 13)
    
    CONNECTION("A1", 1,"B2", 2)
    CONNECTION("E4", 6,"B2", 3)
    CONNECTION("B2", 4,"J4", 8)
    CONNECTION(VCC,"B2", 1)
    
    CONNECTION("A1", 10,"B3", 12)
    CONNECTION("E4", 6,"B3", 11)
    CONNECTION("D3", 1,"B3", 10)
    CONNECTION(VCC,"B3", 13)
    
    CONNECTION("A1", 13,"B3", 2)
    CONNECTION("E4", 6,"B3", 3)
    CONNECTION("C3", 6,"B3", 4)
    CONNECTION(VCC,"B3", 1)
    
    CONNECTION(L,"C2", 2)
    CONNECTION("C3", 5,"C2", 3)
    CONNECTION(VCC,"C2", 4)
    CONNECTION("E4", 6,"C2", 1)
    
    CONNECTION(M,"C2", 12)
    CONNECTION("C3", 9,"C2", 11)
    CONNECTION("E4", 6,"C2", 10)
    CONNECTION(VCC,"C2", 13)
    
    CONNECTION(ATTRACT_n,"J4", 10)
    CONNECTION("B2", 8,"J4", 9)
    CONNECTION("B2", 8,"D2", 4)
    CONNECTION("C2", 8,"D2", 5)
    
    CONNECTION(VRESET_n,"D3", 5)
    CONNECTION("D2", 6,"D3", 6)
    CONNECTION("B2", 5,"E4", 2)
    CONNECTION(VRESET,"E4", 1)
    
    CONNECTION(ATTRACT,"D3", 2)
    CONNECTION("B3", 5,"D3", 3)
    CONNECTION("B3", 6,"D2", 9)
    CONNECTION("C2", 5,"D2", 10)
    CONNECTION(ATTRACT_n,"E4", 5)
    CONNECTION("K4", 10,"E4", 4)
    
    CONNECTION(VIDEO1,"H4", 10)
    CONNECTION(VIDEO1,"H4", 11)
    CONNECTION(MAZE,"H4", 9)
    CONNECTION("H4", 8,"B8", 2)
    CONNECTION(VCC,"B8", 4)
    CONNECTION("B8", 3,"K4", 8)
    CONNECTION(HLD1,"K4", 9)
    
    CONNECTION("H6", 6,"E5", 1)
    CONNECTION(CATCHOS,"E5", 11)
    CONNECTION(V256_n,"E5", 12)
    CONNECTION(V128,"E5", 4)
    CONNECTION(V64,"E5", 6)
    CONNECTION(M1,"E5", 2)
    CONNECTION(M2,"E5", 5)
    CONNECTION(V8,"E5", 3)
    
    CONNECTION(START,"E1", 13)
    CONNECTION(PRES,"H1", 9)
    CONNECTION("H1", 8,"E1", 12)

    /* Right counters */
    CONNECTION(VCC,"E3", 7)
    CONNECTION(OO,"E3", 1)
    CONNECTION(HBLANK_n,"E3", 10)
    CONNECTION(CLK,"E3", 2)
    CONNECTION(A,"E3", 3)
    CONNECTION(B,"E3", 4)

    CONNECTION(GND,"E3", 5)
    CONNECTION(VCC,"E3", 6)
    CONNECTION("E2", 8,"E3", 9)
    
    CONNECTION("E3", 15,"F3", 7)
    CONNECTION(OO,"F3", 1)
    CONNECTION(VCC,"F3", 10)
    CONNECTION(CLK,"F3", 2)
    CONNECTION(GND,"F3", 3)
    CONNECTION(GND,"F3", 4)
    CONNECTION(GND,"F3", 5)
    CONNECTION(VCC,"F3", 6)
    CONNECTION("E2", 8,"F3", 9)
    CONNECTION("F3", 15,"J2", 9)
    CONNECTION(VCC,"J2", 8)
    CONNECTION(VCC,"J2", 11)
    CONNECTION(OO,"J2", 10)
    CONNECTION("J2", 6,"J4", 5)
    CONNECTION("F3", 15,"J4", 4)
    CONNECTION("J4", 6,"E2", 9)
    CONNECTION("E3", 15,"E2", 10)

    CONNECTION("E3", 14,"E2", 12)
    CONNECTION("E3", 13,"E2", 13)
    CONNECTION("E3", 12,"F2", 12)
    CONNECTION("E3", 12,"F2", 13)
    CONNECTION(CLK,"F2", 11)
    CONNECTION(VCC,"F2", 10) 
    
    CONNECTION("E2", 11,"E2", 2)
    CONNECTION("G1", 14,"E2", 5)
    CONNECTION("G1", 13,"H1", 13)
    CONNECTION("H1", 12,"E2", 4)
    CONNECTION("E2", 6,"E2", 1)

    CONNECTION("F2", 8,"F2", 2)
    CONNECTION(CLK,"F2", 3)
    CONNECTION(VCC,"F2", 4)
    CONNECTION(VCC,"F2", 1)
    
    CONNECTION("J4", 6,"H2", 12)
    CONNECTION("E2", 3,"H2", 13)
    CONNECTION("F2", 5,"H2", 10)
    CONNECTION("E3", 11,"H2", 9)
    
    CONNECTION(HSYNC_n,"G1", 2)
    CONNECTION(VBLANK_n,"G1", 10)
    CONNECTION(OO,"G1", 1)
    CONNECTION(VCC,"G1", 7)
    
    CONNECTION(C,"G1", 3)
    CONNECTION(D,"G1", 4)
    
    CONNECTION(GND,"G1", 5)
    CONNECTION(VCC,"G1", 6)
    CONNECTION("E1", 8,"G1", 9)
    CONNECTION("G1", 15,"D1", 7)
    CONNECTION(HSYNC_n,"D1", 2)
    CONNECTION(VCC,"D1", 10)
    CONNECTION(OO,"D1", 1)
    CONNECTION(GND,"D1", 3)
    CONNECTION(GND,"D1", 4)
    CONNECTION(GND,"D1", 5)
    CONNECTION(GND,"D1", 6)
    CONNECTION("E1", 8,"D1", 9)
    CONNECTION("G1", 15,"E1", 9)
    CONNECTION("D1", 15,"E1", 10)
    CONNECTION("D1", 15,"B1", 1)
    CONNECTION("D1", 15,"B1", 2)
    CONNECTION("G1", 12,"B1", 4)
    CONNECTION("G1", 11,"B1", 5)
    
    CONNECTION("B1", 6,"F1", 2)
    CONNECTION("H2", 8,"F1", 3)
    CONNECTION("F1", 1,"E1", 1)
    CONNECTION("F1", 4,"E1", 2)
    
    /* Left control */

    CONNECTION(BUMP2,"K3", 3)
    CONNECTION(BUMP2,"K3", 5)
    CONNECTION(K,"K3", 2)
    CONNECTION(K,"H1", 1)
    CONNECTION("H1", 2,"K3", 6)
    CONNECTION("K3", 1,"L3", 3)
    CONNECTION(VCC,"L3", 2)
    CONNECTION(VCC,"L3", 4)
    CONNECTION(VRESET_n,"L3", 1)
    CONNECTION("K3", 4,"L3", 11)
    CONNECTION(VCC,"L3", 12)
    CONNECTION(VCC,"L3", 10)
    CONNECTION(VRESET_n,"L3", 13)
    
    CONNECTION("STICK2", Joystick::LEFT, "M1", 2)
    CONNECTION("STICK2", Joystick::RIGHT, "M1", 5)
    CONNECTION("STICK2", Joystick::UP, "M1", 11)
    CONNECTION("STICK2", Joystick::DOWN,"M1", 8)

    CONNECTION(S,"M1", 3)
    CONNECTION(S,"M1", 6)
    CONNECTION(S,"M1", 12)
    CONNECTION(S,"M1", 9)
    
    CONNECTION("M1", 1,"M3", 2)
    CONNECTION("M4", 11,"M3", 3)
    CONNECTION("L3", 8,"M3", 4)
    CONNECTION(VCC,"M3", 1)
    
    CONNECTION("M1", 4,"M3", 12)
    CONNECTION("M4", 11,"M3", 11)
    CONNECTION("K3", 13,"M3", 10)
    CONNECTION(VCC,"M3", 13)
    
    CONNECTION("M1", 13,"M2", 2)
    CONNECTION("M4", 11,"M2", 3)
    CONNECTION("K4", 4,"M2", 4)
    CONNECTION(VCC,"M2", 1)
    
    CONNECTION("M1", 10,"M2", 12)
    CONNECTION("M4", 11,"M2", 11)
    CONNECTION("L3", 6,"M2", 10)
    CONNECTION(VCC,"M2", 13)
    
    CONNECTION(N,"L2", 2)
    CONNECTION("L3", 5,"L2", 3)
    CONNECTION(VCC,"L2", 4)
    CONNECTION("M4", 11,"L2", 1)
    
    CONNECTION(O,"L2", 12)
    CONNECTION("L3", 9,"L2", 11)
    CONNECTION("M4", 11,"L2", 10)
    CONNECTION(VCC,"L2", 13)
    
    CONNECTION(ATTRACT,"K3", 12)
    CONNECTION("M3", 5,"K3", 11)
    CONNECTION(ATTRACT,"K4", 6)
    CONNECTION("M2", 9,"K4", 5)
    CONNECTION("M2", 8,"K2", 5)
    CONNECTION("L2", 5,"K2", 4)
    
    CONNECTION("M3", 6,"K2", 10)
    CONNECTION("L2", 8,"K2", 9)
    CONNECTION("K2", 8,"K3", 8)
    CONNECTION(VRESET_n,"K3", 9)
    CONNECTION("M3", 9,"M4", 9)
    CONNECTION(VRESET,"M4", 10)

    /*Left counters */
    CONNECTION(CLK,"J3", 2)
    CONNECTION(HBLANK_n,"J3", 10)
    CONNECTION("J10", 6,"J3", 1)
    CONNECTION(VCC,"J3", 7)
    CONNECTION(E,"J3", 3)
    CONNECTION(F,"J3", 4)
    CONNECTION(GND,"J3", 5)
    CONNECTION(VCC,"J3", 6)
    CONNECTION("J1", 12,"J3", 9)
    CONNECTION("J3", 15,"H3", 7)
    CONNECTION("J10", 6, "H3", 1)
    CONNECTION(VCC,"H3", 10)
    CONNECTION(CLK,"H3", 2)
    CONNECTION("J1", 12,"H3", 9)
    CONNECTION(GND,"H3", 3)
    CONNECTION(GND,"H3", 4)
    CONNECTION(GND,"H3", 5)
    CONNECTION(VCC,"H3", 6)
    CONNECTION("H3", 15,"J2", 12)
    CONNECTION(VCC,"J2", 1)
    CONNECTION(VCC,"J2", 4)
    CONNECTION(PRES,"J2", 13)
    CONNECTION("J2", 3,"J1", 1)
    CONNECTION("H3", 15,"J1", 2)
    CONNECTION("J3", 15,"J1", 13)
    CONNECTION("J2", 3,"H2", 5)
    CONNECTION("H3", 15,"H2", 4)
    CONNECTION("J3", 11,"H2", 1)
    CONNECTION("J3", 12,"H2", 2)
    
    CONNECTION(HSYNC_n,"L1", 2)
    CONNECTION(VBLANK_n,"L1", 10)
    CONNECTION("J10", 6,"L1", 1)
    CONNECTION(VCC,"L1", 7)
    CONNECTION(G,"L1", 3)
    CONNECTION(H,"L1", 4)
    CONNECTION(GND,"L1", 5)
    CONNECTION(VCC,"L1", 6)
    CONNECTION("E1", 6,"L1", 9)
    CONNECTION("L1", 15,"K1", 7)
    CONNECTION(HSYNC_n,"K1", 2)
    CONNECTION(VCC,"K1", 10)
    CONNECTION("J10", 6,"K1", 1)
    CONNECTION(GND,"K1", 3)
    CONNECTION(GND,"K1", 4)
    CONNECTION(GND,"K1", 5)
    CONNECTION(GND,"K1", 6)
    CONNECTION("E1", 6,"K1", 9)
    CONNECTION("L1", 15,"E1", 4)
    CONNECTION("K1", 15,"E1", 5)
    
    CONNECTION("K1", 15,"J1", 3)
    CONNECTION("L1", 11,"H1", 5)
    CONNECTION("H1", 6,"J1", 5)
    CONNECTION("L1", 12,"J1", 4)
    CONNECTION("J1", 6,"F1", 6)
    CONNECTION("H2", 6,"F1", 5)

    CONNECTION("L1", 14,"K2", 2)
    CONNECTION("L1", 13,"K2", 1)
    CONNECTION("J3", 14,"K2", 13)
    CONNECTION("J3", 13,"K2", 12)
    CONNECTION("K2", 11,"F1", 11)
    // Speed hack - Doesn't work?
    /*CONNECTION("F1", 4, "CLK_GATE2", 1)
    CONNECTION("K2", 11, "CLK_GATE2", 2)
    CONNECTION("CLK_GATE2", 3, "F1",11)*/ 

    CONNECTION("K2", 3,"F1", 12)
    CONNECTION("F1", 13,"H1", 11)
    CONNECTION("H1", 10,"J1", 10)
    CONNECTION("F1", 4,"J1", 11)
    //CONNECTION(MAZE,"J1", 9)
    // Speed hack
    CONNECTION("F1", 4, "CLK_GATE1", 1)
    CONNECTION(MAZE, "CLK_GATE1", 2)
    CONNECTION("CLK_GATE1", 3, "J1",9)
       
    //CONNECTION(VIDEO2,"J10", 12)
    //CONNECTION(MHBLANK_n,"J10", 13)
    // Not shown on schematics, but not sure how right wall collisions would work without something like this
    // TODO: Verify correct functionality?
    CONNECTION(MHBLANK_n, "XY", 9)
    CONNECTION(VIDEO2, "XY", 10)
    CONNECTION("H8", 10, "XY", 11)
    CONNECTION("XY", 8,"J10", 10)

    //CONNECTION("J10", 11,"J10", 10)
    CONNECTION(CATCH_n,"J10", 9)
    CONNECTION("J10", 8,"J10", 1)
    CONNECTION("J10", 8,"J10", 2)
    CONNECTION("J10", 3,"K10", 2)
    CONNECTION(VCC,"K10", 4)
    CONNECTION("K10", 3,"J10", 4)
    CONNECTION("K10", 3,"J10", 5)
    
    //CONNECTION(CATCH_n,"A10", 13)
    // Need capacitance here to prevent 12ns pulse from triggering B7, but not K10?
    CONNECTION(CATCH_n, "BUF1", 1)
    CONNECTION("BUF1", 2, "A10", 13)

    CONNECTION("A10", 8,"A10", 12)
    CONNECTION("A10", 11,"A10", 10)
    CONNECTION(VRESET_n,"A10", 9)
    CONNECTION("A10", 8,"B7", 11) 
    CONNECTION(START,"B7", 12)
    CONNECTION(ATTRACT_n,"B7", 13)
                     
    CONNECTION(VCC,"C8", 4)
    CONNECTION(BUMP2,"C8", 2)
    CONNECTION("C8", 3,"L4", 5)
    CONNECTION(HLD2,"L4", 6)
    CONNECTION("L4", 4,"M4", 12)
    CONNECTION(ATTRACT_n,"M4", 13)


    // Video
    CONNECTION("VIDEO", 1, "F6", 13)
    CONNECTION("VIDEO", 2, "F1", 1)
    CONNECTION("VIDEO", 3, "F1", 4)
    
    CONNECTION("VIDEO", Video::HBLANK_PIN, "M5", 8)
    CONNECTION("VIDEO", Video::VBLANK_PIN, "F6", 10)

    
    // Sound
    CONNECTION("J2", 3, "D2", 2)
    CONNECTION(Y, "D2", 1)
            
    CONNECTION("K1", 11, "D2", 12)
    CONNECTION(X, "D2", 13)

    CONNECTION("D2", 11, "PROXIMITY", 1)
    CONNECTION("D2", 3, "PROXIMITY", 2)

    CONNECTION(ATTRACT_n, "E8", 4)

    CONNECTION(ATTRACT, "L4", 3)
    CONNECTION("E8", 3, "L4", 2)

    CONNECTION(V8, "M4", 2)
    CONNECTION("L4", 1, "M4", 1)

    CONNECTION(V8, "M4", 4)
    CONNECTION(CATCHOS, "M4", 5)

    CONNECTION("M4", 6, "AUDIO", 1)
    CONNECTION("M4", 3, "AUDIO", 2)

    CONNECTION("AUDIO", i2, "MIXER1", 1)
    CONNECTION(GND, "MIXER1", 2)

    CONNECTION("AUDIO", i1, "MIXER2", 1)
    CONNECTION("MIXER1", i1, "MIXER2", 2)

    CONNECTION("MIXER2", i1, "AUDIO", Audio::OUTPUT_MONO)

#ifdef DEBUG
    CONNECTION("D2", 11, "LOG1", 1)
    CONNECTION("D2", 3, "LOG1", 2)

    /*CONNECTION("LOG1", 1, "H7", 12)
    CONNECTION("LOG1", 2, "H7", 8)
    CONNECTION("LOG1", 3, "F7", 6)
    CONNECTION("LOG1", 4, "H7", 6)
    CONNECTION("LOG1", 5, "E7", 12)
    CONNECTION("LOG1", 6, "F7", 12)
    CONNECTION("LOG1", 7, "F7", 8)
    CONNECTION("LOG1", 8, "F8", 8)
    //CONNECTION("LOG1", 9, "H8", 10)
    CONNECTION("LOG1", 10, VBLANK_n)*/
#endif

CIRCUIT_LAYOUT_END
  
