#include "circuit_desc.h"

EXTERN_CIRCUIT_LAYOUT( pong );
EXTERN_CIRCUIT_LAYOUT( rebound );
EXTERN_CIRCUIT_LAYOUT( gotcha );
EXTERN_CIRCUIT_LAYOUT( spacerace );
EXTERN_CIRCUIT_LAYOUT( stuntcycle );
EXTERN_CIRCUIT_LAYOUT( pongdoubles );
EXTERN_CIRCUIT_LAYOUT( tvbasketball );
EXTERN_CIRCUIT_LAYOUT( breakout );
EXTERN_CIRCUIT_LAYOUT( antiaircraft );
EXTERN_CIRCUIT_LAYOUT( attack );
EXTERN_CIRCUIT_LAYOUT( sharkjaws );
EXTERN_CIRCUIT_LAYOUT( quadrapong );
EXTERN_CIRCUIT_LAYOUT( jetfighter );
EXTERN_CIRCUIT_LAYOUT( crashnscore );
EXTERN_CIRCUIT_LAYOUT( crossfire );
EXTERN_CIRCUIT_LAYOUT( pinpong );
EXTERN_CIRCUIT_LAYOUT( cleansweep );
EXTERN_CIRCUIT_LAYOUT( wipeout );
EXTERN_CIRCUIT_LAYOUT( hiway );
EXTERN_CIRCUIT_LAYOUT( steeplechase );
EXTERN_CIRCUIT_LAYOUT( indy4 );

struct GameDesc
{
    const char* name;
    const CircuitDesc* desc;
    const char* command_line;
    
    const char* manufacturer;
    unsigned year;

    bool operator<(const GameDesc& g) const { return strcmp(name, g.name) < 0; }
};

#define GAME( name, id, manufacturer, year ) { name, circuit_##id(), #id, manufacturer, year }

static GameDesc game_list[] =
{
    GAME( "Pong",               pong,            "Atari",              1972 ),
    GAME( "Pong Doubles",       pongdoubles,     "Atari",              1973 ),
    GAME( "Rebound",            rebound,         "Atari",              1974 ),
    GAME( "Gotcha",             gotcha,          "Atari",              1973 ),
    GAME( "Space Race",         spacerace,       "Atari",              1973 ),
    GAME( "Stunt Cycle",        stuntcycle,      "Atari",              1976 ),
    GAME( "TV Basketball",      tvbasketball,    "Midway",             1974 ),
    GAME( "Breakout",           breakout,        "Atari",              1976 ),
    GAME( "Anti-Aircraft",      antiaircraft,    "Atari",              1975 ),
    GAME( "Attack",             attack,          "Exidy",              1977 ),
    GAME( "Shark Jaws",         sharkjaws,       "Atari",              1975 ),
    GAME( "Quadrapong",         quadrapong,      "Atari",              1974 ),
    GAME( "Jet Fighter",        jetfighter,      "Atari",              1975 ),
    GAME( "Crash 'N Score",     crashnscore,     "Atari",              1975 ),
    GAME( "Crossfire",          crossfire,       "Atari",              1975 ),
    GAME( "Pin Pong",           pinpong,         "Atari",              1974 ),
    GAME( "Clean Sweep",        cleansweep,      "Ramtek",             1974 ),
    GAME( "Wipe Out",           wipeout,         "Ramtek",             1974 ),
    GAME( "Hi-Way",             hiway,           "Atari",              1975 ),
    GAME( "Steeplechase",       steeplechase,    "Atari",              1975 ),
    GAME( "Indy 4",             indy4,           "Atari",              1976 )
};


static const int game_list_size = sizeof(game_list) / sizeof(GameDesc);

