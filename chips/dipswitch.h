#ifndef DIPSWITCH_H
#define DIPSWITCH_H

#include <array>
#include "../chip_desc.h"
#include "555astable.h"
#include "555mono.h"

struct DipswitchBase
{
    unsigned state;
    const char* name;
    const char* desc;

    constexpr DipswitchBase(const char* n, const char* d, int default_state)
        : name(n), desc(d), state(default_state) { }

    virtual const char* const* getSettings() const = 0;
    virtual const size_t settingsSize() const = 0;
};

template <size_t N> struct DipswitchTemplate : DipswitchBase
{
    std::array<const char*, N> settings;

    constexpr DipswitchTemplate(const char* n, const char* d, int default_state, std::array<const char*, N> s)
        : DipswitchBase(n, d, default_state), settings(s) { }

    const char* const* getSettings() const { return settings.data(); }
    const size_t settingsSize() const { return N; }
};


/* Standard 1-bit DIP Switch. Can be SPDT or DPDT. */
class DipswitchDesc : DipswitchTemplate<2>
{
public:
    constexpr DipswitchDesc(const char* n, const char* d, int default_state,
                            const char* setting1, const char* setting2) 
        : DipswitchTemplate(n, d, default_state, std::array<const char*, 2>{{setting1, setting2}}) { }
    
    static CUSTOM_LOGIC( logic );
};

extern CHIP_DESC( DIPSWITCH );



/* SP4T Switch. Assumes outputs are tied high when not selected by switch. */
class DipswitchSP4TDesc : DipswitchTemplate<4>
{
public:
    DipswitchSP4TDesc(const char* n, const char* d, int default_state,
                      const char* setting1, const char* setting2,
                      const char* setting3, const char* setting4) 
        : DipswitchTemplate(n, d, default_state,
          std::array<const char*, 4>{{setting1, setting2, setting3, setting4}}) { }

    template<int T> static CUSTOM_LOGIC( logic );
};

extern CHIP_DESC( DIPSWITCH_SP4T );




/* AMP 53137. 4-bit Rotary Hex DIP Switch. */
class Dipswitch53137Desc : DipswitchTemplate<16>
{
public:
    Dipswitch53137Desc(const char* n, const char* d, int default_state,
                       const char* setting1, const char* setting2,
			           const char* setting3 = NULL, const char* setting4 = NULL,
			           const char* setting5 = NULL, const char* setting6 = NULL,
			           const char* setting7 = NULL, const char* setting8 = NULL,
			           const char* setting9 = NULL, const char* setting10 = NULL,
			           const char* setting11 = NULL, const char* setting12 = NULL,
			           const char* setting13 = NULL, const char* setting14 = NULL,
			           const char* setting15 = NULL, const char* setting16 = NULL)
        : DipswitchTemplate(n, d, default_state,
          std::array<const char*, 16>
	  {{setting1, setting2, setting3, setting4,
           setting5, setting6, setting7, setting8,
           setting9, setting10, setting11, setting12,
           setting13, setting14, setting15, setting16}}) { }

    template<int BIT> static CUSTOM_LOGIC( logic );
};

extern CHIP_DESC( 53137 );



/* 4SPST 4-bit Hex DIP Switch. */
typedef Dipswitch53137Desc Dipswitch4SP4TDesc;

extern CHIP_DESC( DIPSWITCH_4SPST );




struct PotentiometerBase
{
    double min_val, max_val, current_val;
    const char* name;
    const char* desc;

    PotentiometerBase(const char* n, const char* d, double default_val, double min, double max)
        : name(n), desc(d), current_val(default_val), min_val(min), max_val(max) { }
};

/* Potentiometer */
template <typename T, double (T::*r)>
class PotentiometerDesc : PotentiometerBase
{
public:
    T& output;

    PotentiometerDesc(const char* n, const char* d, double default_val, double min, double max, T& t) 
        : PotentiometerBase(n, d, default_val, min, max), output(t) { }

    static CUSTOM_LOGIC( logic );
};

typedef PotentiometerDesc<Astable555Desc, &Astable555Desc::r1> PotentimeterAstable555Desc; 
typedef PotentiometerDesc<Mono555Desc, &Mono555Desc::r> PotentimeterMono555Desc; 

extern CHIP_DESC( POT_555_ASTABLE );
extern CHIP_DESC( POT_555_MONO );

#define POTENTIOMETER_CONNECTION( c1, c2 ) CONNECTION(c1, i3, c2, i3)

#endif
