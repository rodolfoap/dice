#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <phoenix.hpp>
#include <nall/config.hpp>
#include "circuit_desc.h"

using nall::configuration;

struct GameConfig : configuration
{
    nall::string filename;
    
    GameConfig(const CircuitDesc* desc, const char* name);

    bool load()
    {
        configuration::load(filename);
        return save();  // Create file if it doesn't exist
    }

    bool save()
    {
        return configuration::save(filename);
    }

    bool operator==(const GameConfig& g) { return filename == g.filename; }

    static bool isDipswitch(const ChipDesc* chip);
    static bool isPotentiometer(const ChipDesc* chip);
};

#endif
