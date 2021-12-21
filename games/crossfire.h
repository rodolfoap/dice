#include "../circuit_desc.h"
#include "../circuit.h"

extern CUSTOM_LOGIC( clock );

/*
 * Crossfire analog circuitry.
 *
 * Most values determined from guesswork + simulation,
 * exact rates of change may not be entirely correct!
 * TODO: improve accuracy, verify on real hardware?
 */

/*
 * Joystick voltage
 * Depending on joystick state 10 uF capacitor is
 * either discharged through BJT current source/drain.
 * Approximate min voltage = 0.57, max = 4.85.
 * Rate of change approx 2.25 V/s
 *
 * Pin 1: Joystick Up
 * Pin 2: Joystick Down
 * Pin 3: Output voltage
 */
static CUSTOM_LOGIC( joystick_v )
{
    chip->inputs ^= mask;

    if(mask == 0) chip->analog_output = 2.7; // init
    if(mask != 4) return;

    if(!(chip->inputs & 1)) // Joystick Up, Discharge Cap
        chip->analog_output -= 2.25E-3;
    else if(!(chip->inputs & 2)) // Joystick Down, Charge Cap
        chip->analog_output += 2.25E-3;


    // 0.57 V determined from simulation but gun is immobile below ~1.2 V ???
    //if(chip->analog_output < 0.57)      chip->analog_output = 0.57;
    if(chip->analog_output < 1.15)      chip->analog_output = 1.15;
    else if(chip->analog_output > 4.86) chip->analog_output = 4.86;

    //printf("t:%lld joystick v:%g\n", chip->circuit->global_time, chip->analog_output);
}

static CHIP_DESC( JOYSTICK_VOLTAGE ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 1.0, 1.0 ),

    CUSTOM_CHIP_START(&joystick_v)
        INPUT_PINS( 1, 2, i1 )
        OUTPUT_PIN( 3 ),

	CHIP_DESC_END
};


/*
 * Scan comparator
 * Game generates HSCAN & VSCAN signals - linear voltage ramps
 * that increase from 0 - 5 V over the course of a horizontal line or frame.
 * LM339 op amps compare these scan signals to Joystick voltages and bullet voltages
 * to create joystick / bullet positions.
 * 
 * Comparator goes low when scan voltage > ref voltage.
 * Calculate the time at which this will occur at the end of each blanking period,
 * and generate an event for that time.
 *
 * Pin 1: Scan control signal (HSYNC or VSYNC)
 * Pin 2: Ref signal
 * Pin 3: Output
 *
 * TODO: Op amp characteristics? Determine effect of feedback on capacitors?
 */
static CUSTOM_LOGIC( scan_comp )
{
    static const uint64_t t_lres = uint64_t(300.0E-9 / Circuit::timescale); // LM339 large signal response time

    if(mask == 1 && !(chip->inputs & 1)) // Rising edge of SYNC signal
    {
        // Compute scan period, hijack delay variable to store
        chip->delay[1] = chip->circuit->global_time - chip->delay[0];

        chip->pending_event = chip->circuit->queue_push(chip, 1);        
    }
    else if(mask == 1 && (chip->inputs & 1)) // Falling edge of SYNC signal
    {
        chip->delay[0] = chip->circuit->global_time;

        double v_ref = chip->input_links[1].chip->analog_output;

        // Compute intersection of scan with v_ref
        // TODO: check for negative time or > sync period?
        uint64_t time = (v_ref / 5.0) * chip->delay[1];

        chip->pending_event = chip->circuit->queue_push(chip, time + t_lres);
    }

    chip->inputs ^= mask;
}

static CHIP_DESC( SCAN_COMPARATOR ) = 
{
    CUSTOM_CHIP_START(&scan_comp)
        INPUT_PINS( 1, 2 )
        OUTPUT_PIN( 3 )
        OUTPUT_DELAY_MS( 1.0, 1.0 ),

	CHIP_DESC_END
};


/*
 * Bullet voltage
 * Depending on FIRE signal state 5 uF capacitor is
 * either discharged through BJT current source/drain.
 * Approximate min voltage = 0, max = ~4.5 ? (4.32 player 1, ~4.8 player 2?)
 * Rate of change approx +/- 4.05 V/s
 *
 * Player 1 fire charges rapidly when input is low, discharges slowly when input is high (bullet moving left).
 * Player 2 fire discharges rapidly when input is high, scharges slowly when input is low (bullet moving right).
 *
 * Pin 1: Button state
 * Pin 2: Output voltage
 */
static CUSTOM_LOGIC( bullet1_v )
{
    chip->inputs ^= mask;

    if(mask != 2) return;

    if(chip->inputs & 1) // Input High, Discharge Cap
        chip->analog_output -= 4.05E-3;
    else // Input Low, Charge Cap
        chip->analog_output = 4.3;

    if(chip->analog_output < 0.0) chip->analog_output = 0.0;

    //printf("t:%lld bullet2 v:%g\n", chip->circuit->global_time, chip->analog_output);
}

static CHIP_DESC( BULLET1_VOLTAGE ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 1.0, 1.0 ),

    CUSTOM_CHIP_START(&bullet1_v)
        INPUT_PINS( 1, i1 )
        OUTPUT_PIN( 2 ),

	CHIP_DESC_END
};

static CUSTOM_LOGIC( bullet2_v )
{
    chip->inputs ^= mask;

    if(mask != 2) return;

    if(chip->inputs & 1) // Input High, Discharge Cap
        chip->analog_output = 0.2;
    else // Input Low, Charge Cap
        chip->analog_output += 4.05E-3;

    if(chip->analog_output > 4.8) chip->analog_output = 4.8;

    //printf("t:%lld bullet2 v:%g\n", chip->circuit->global_time, chip->analog_output);
}

static CHIP_DESC( BULLET2_VOLTAGE ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 1.0, 1.0 ),

    CUSTOM_CHIP_START(&bullet2_v)
        INPUT_PINS( 1, i1 )
        OUTPUT_PIN( 2 ),

	CHIP_DESC_END
};


// FIRE Sound CV
// If input signal is high, charge capacitor to 3V through 390k resistor.
// If input is low, discharge rapidly to 1 V through diodes.
// 2F signal gates transistor. When 2F is high CV = Cap voltage + 0.7,
// when 2F is low, CV = min(Cap voltage + 0.7, 2.25)
// TODO: Make more accurate?
//
// Input pin 1: FIRE signal
// Input pin 2: 2F signal
// Output pin 3: CV, connect to 555 Astable pin i3
struct FireSoundDesc
{
    double cap_v;
    Astable555Desc* desc;

public:
    FireSoundDesc(Astable555Desc* d) : desc(d) { }
};

static CUSTOM_LOGIC( fire_cv )
{
    Astable555Desc* desc = ((FireSoundDesc*)chip->custom_data)->desc;
    double& cap_v = ((FireSoundDesc*)chip->custom_data)->cap_v;
    static const double RC = K_OHM(390.0) * U_FARAD(5.0) / 2.25; // Roughly 1/2 charge rate due to BJT base current?
    static const double RC_EXP = 1.0 - exp(-1.0E-3 / RC);

    chip->inputs ^= mask;

    if(!(chip->inputs & 2)) cap_v = 1.0; // Discharge cap
    else cap_v += (3.0 - cap_v) * RC_EXP;

    double cv = cap_v + 0.7;
    if(!(chip->inputs & 4) && cv > 2.25) cv = 2.25;

    //printf("t:%lld vcap:%g cv:%g\n", chip->circuit->global_time, cap_v, cv);

    desc->ctrl = cv;
    chip->pending_event = chip->circuit->queue_push(chip, 0);
}

static CHIP_DESC( FIRE_SOUND_CV ) = 
{
	CUSTOM_CHIP_START(&clock)
        OUTPUT_PIN( i1 )
        OUTPUT_DELAY_MS( 1.0, 1.0 ),

    CUSTOM_CHIP_START(&fire_cv)
        INPUT_PINS( i1, 1, 2  )
        OUTPUT_PIN( i3 ),

    CHIP_DESC_END
};


// GNE
// Transistor network used to seet GNE high when N9 capacitor reaches ~2.8 V.
// Input pin 1: 555 Output
// Output pin 2: GNE
static CUSTOM_LOGIC( gne )
{
    static const double TC = -log((5.0 - 2.8) / 5.0);
    Mono555Desc* desc = (Mono555Desc*)chip->custom_data;

    if(mask == 1 && !(chip->inputs & 1)) // Rising edge 555 output
    {
        uint64_t delay = uint64_t(TC*desc->r*desc->c / Circuit::timescale);
        chip->pending_event = chip->circuit->queue_push(chip, delay);
       
        //printf("t:%lld delay:%lld\n", chip->circuit->global_time, delay);
    }
    else if(mask == 1 && (chip->inputs & 1)) // Falling edge 555 output
    {
        chip->pending_event = chip->circuit->queue_push(chip, uint64_t(1.0E-9 / Circuit::timescale));
    }

    chip->inputs ^= mask;
}

static CHIP_DESC( GNE ) = 
{
	CUSTOM_CHIP_START(&gne)
        INPUT_PINS( 1, i3 )
        OUTPUT_PIN( 2 ),

    CHIP_DESC_END
};




// HSYNC Mod
// Prevent leftmost screen from being visible,
// would probably be in overscan area of monitor.
// TODO: Adjustable monitor params?
static CHIP_LOGIC( HSYNC_MOD )
{
    // Set on rising edge HSYNC, clear on rising edge 32H after HSYNC ends
    if(POS_EDGE_PIN(1))
        pin[3] = 1;
    else if(POS_EDGE_PIN(2) && !pin[1])
        pin[3] = 0;
    else
        pin[3] = prev_pin[3];
}

static CHIP_DESC( HSYNC_MOD ) = 
{
    CHIP_START(HSYNC_MOD)
        INPUT_PINS( 1, 2 )
        EVENT_PINS( 1, 2 )
        OUTPUT_PIN( 3 )
        PREV_OUTPUT_PIN( 3 )
        OUTPUT_DELAY_NS( 1.0, 150.0 ),

	CHIP_DESC_END
}; 
