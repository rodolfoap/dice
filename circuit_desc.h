#ifndef CIRCUITDESC_H
#define CIRCUITDESC_H

#include "chip_desc.h"
#include "chip_list.h"
#include "video_desc.h"
#include "audio_desc.h"
#include "input_desc.h"

enum CircuitDescType : uint8_t
{
    CHIP_INST = 0,
    CONNECTION,
    NET_LISTING,
    VIDEO_INST,
    AUDIO_INST,
    INPUT_INST,
    OPTIMIZATION_HINT,
    SUB_CIRCUIT
};

struct ChipInstance
{
    const char* name;
    const ChipDesc* chip;
	uintptr_t custom_data;
};

struct ConnectionDesc
{
    const char* name1;
    const char* name2;
    uint8_t pin1;
	uint8_t pin2;
};

struct OptimizationHintDesc
{
    const char* chip;
    int queue_size;
    int subcycle_size;
};

struct NetDesc
{
    const char* name;
    const char* chip;
    uint8_t pin;
};

class CircuitDesc;
struct SubcircuitDesc
{
    const char* prefix;
    const CircuitDesc* (*desc)();
};

struct CircuitEntry
{
    union U
    {
		ChipInstance instance;
    	ConnectionDesc connection;
        NetDesc net;
        const VideoDesc* video;
        const AudioDesc* audio;
        const InputDesc* input;
        const SubcircuitDesc sub_circuit;
        OptimizationHintDesc hint;        

        constexpr U(const char* n, const ChipDesc* c, uintptr_t d) : instance({n, c, d}) { }
        constexpr U(const char* n1, uint8_t p1, const char* n2, uint8_t p2) : connection({n1, n2, p1, p2}) { }
        constexpr U(const char* n, const char* c, uint8_t p) : net({n, c, p}) { }
        constexpr U(const VideoDesc* v) : video(v) { }
        constexpr U(const AudioDesc* a) : audio(a) { }
        constexpr U(const InputDesc* i) : input(i) { }
        constexpr U(const char* c, int q, int s) : hint({c, q, s}) { }
        constexpr U(const char* n, const CircuitDesc* (*s)()) : sub_circuit({n, s}) { }
	} u;
    CircuitDescType type;

    constexpr CircuitEntry(const char* n, const ChipDesc* c) : type(CHIP_INST), u(n, c, 0) { }
    template<typename T> constexpr CircuitEntry(const char* n, const ChipDesc* c, T* d) : type(CHIP_INST), u(n, c, (uintptr_t)d) { }
    constexpr CircuitEntry(const char* n, const ChipDesc* c, uintptr_t d) : type(CHIP_INST), u(n, c, d) { }
	constexpr CircuitEntry(const char* n1, uint8_t p1, const char* n2, uint8_t p2) : type(CONNECTION), u(n1, p1, n2, p2) { }
    constexpr CircuitEntry(const char* n, const char* c, uint8_t p) : type(NET_LISTING), u(n, c, p) { }
    constexpr CircuitEntry(const char* c, int q, int s) : type(OPTIMIZATION_HINT), u(c, q, s) { }
    constexpr CircuitEntry(const char* n, const CircuitDesc* (*s)()) : type(SUB_CIRCUIT), u(n, s) { }
    constexpr CircuitEntry(const VideoDesc* v) : type(VIDEO_INST), u(v) { }
    constexpr CircuitEntry(const AudioDesc* a) : type(AUDIO_INST), u(a) { }
    constexpr CircuitEntry(const InputDesc* i) : type(INPUT_INST), u(i) { }
};

struct CircuitDesc
{
    const VideoDesc* video;
    const AudioDesc* audio;
    const InputDesc* input;
    
    const CircuitEntry* begin;
    const CircuitEntry* end;

    template<typename T> static CircuitDescType GetType();

    template<typename T> struct Set
    {
        struct SetIterator
        { 
            const CircuitEntry* c;
            const CircuitEntry* end;
            CircuitDescType type;
            
            SetIterator(const CircuitEntry* C, const CircuitEntry* E) : c(C), end(E) { while(c->type != GetType<T>() && c != end) c++; }
            void operator++() { do { c++; } while(c->type != GetType<T>() && c != end); }
            bool operator!=(const SetIterator& s) { return c != s.c; }
            bool operator!=(const CircuitEntry* e) { return c != e; }
            const T& operator*() { return (const T&)c->u; }
        };
        
        const CircuitEntry* _begin;
        const CircuitEntry* _end;

        const SetIterator begin() const { return SetIterator(_begin, _end); }
        const SetIterator end() const { return SetIterator(_end, _end); }

        constexpr Set(const CircuitEntry* b, const CircuitEntry* e) : _begin(b), _end(e) { }
    };

    template<size_t N> CircuitDesc(const CircuitEntry(&b)[N]) : begin(b), end(b + N)
    {
        Set<VideoDesc*>::SetIterator v(begin, end);
        Set<AudioDesc*>::SetIterator a(begin, end);
        Set<InputDesc*>::SetIterator i(begin, end);

        video = (v != end) ? *v : nullptr;
        audio = (a != end) ? *a : nullptr;
        input = (i != end) ? *i : nullptr;
    }

    const Set<ChipInstance> get_chips() const { return Set<ChipInstance>(begin, end); }
    const Set<ConnectionDesc> get_connections() const { return Set<ConnectionDesc>(begin, end); }
    const Set<OptimizationHintDesc> get_hints() const { return Set<OptimizationHintDesc>(begin, end); }
    const Set<SubcircuitDesc> get_sub_circuits() const { return Set<SubcircuitDesc>(begin, end); }
};

template<> inline CircuitDescType CircuitDesc::GetType<ChipInstance>() { return CHIP_INST; }
template<> inline CircuitDescType CircuitDesc::GetType<ConnectionDesc>() { return CONNECTION; }
template<> inline CircuitDescType CircuitDesc::GetType<NetDesc>() { return NET_LISTING; }
template<> inline CircuitDescType CircuitDesc::GetType<VideoDesc*>() { return VIDEO_INST; }
template<> inline CircuitDescType CircuitDesc::GetType<AudioDesc*>() { return AUDIO_INST; }
template<> inline CircuitDescType CircuitDesc::GetType<InputDesc*>() { return INPUT_INST; }
template<> inline CircuitDescType CircuitDesc::GetType<OptimizationHintDesc>() { return OPTIMIZATION_HINT; }
template<> inline CircuitDescType CircuitDesc::GetType<SubcircuitDesc>() { return SUB_CIRCUIT; }

#define VCC "_VCC", 1
#define GND "_GND", 1

#define CIRCUIT_LAYOUT( name ) const CircuitDesc* circuit_##name() { static /*constexpr*/ CircuitEntry netlist[] = {
#define CIRCUIT_LAYOUT_END }; static CircuitDesc desc(netlist); return &desc; }

#define EXTERN_CIRCUIT_LAYOUT( name ) extern CircuitDesc* circuit_##name();

#define CHIP( name, desc... )     CircuitEntry(name, chip_##desc),
#define CONNECTION( args... )     CircuitEntry(args),
#define SUB_CIRCUIT( name, desc ) CircuitEntry(name, &circuit_##desc),
//#define NET( args... )        CircuitEntry(args),
#define VIDEO( name )             CircuitEntry(&video_##name),
#define AUDIO( name )             CircuitEntry(&audio_##name),
#define INPUT( name )             CircuitEntry(input_##name),

#define DISABLE_OPTIMIZATION(chip, pin) CircuitEntry("_DEOPTIMIZER", 1, chip, pin),
#define OPTIMIZATION_HINT(chip, e, s) CircuitEntry(chip, e, s),

#define OHM( val ) double(val)
#define K_OHM( val ) (double(val)*1e3)
#define M_OHM( val ) (double(val)*1e6)

#define M_FARAD( val ) (double(val)*1e-3)
#define U_FARAD( val ) (double(val)*1e-6)
#define N_FARAD( val ) (double(val)*1e-9)
#define P_FARAD( val ) (double(val)*1e-12)

#define DELAY_MS( t ) (double(t)*1e-3)
#define DELAY_US( t ) (double(t)*1e-6)
#define DELAY_NS( t ) (double(t)*1e-9)
#define DELAY_PS( t ) (double(t)*1e-12)

#endif
