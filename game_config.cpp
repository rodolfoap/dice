#include <nall/platform.hpp>
#include <nall/directory.hpp>

#include "game_config.h"
#include "chips/dipswitch.h"

using namespace nall;

bool GameConfig::isDipswitch(const ChipDesc* chip)
{
    return chip == chip_DIPSWITCH ||
           chip == chip_DIPSWITCH_SP4T ||
           chip == chip_53137 || 
           chip == chip_DIPSWITCH_4SPST;
}

bool GameConfig::isPotentiometer(const ChipDesc* chip)
{
    return chip == chip_POT_555_ASTABLE ||
           chip == chip_POT_555_MONO;
}

GameConfig::GameConfig(const CircuitDesc* desc, const char* name)
{
    // Load config file
    nall::string config_path = configpath();
    config_path.append("dice/");
    directory::create(config_path);

    filename = {config_path, name, ".cfg"};

    bool has_config = false;

    // Find all DIP switches in circuit desc, append to game configuration
    for(const ChipInstance& instance : desc->get_chips())
    {
        if(isDipswitch(instance.chip))
        {
            DipswitchBase* d = (DipswitchBase*)instance.custom_data;
            append(d->state, d->name);
            has_config = true;
        }
        else if(isPotentiometer(instance.chip))
        {
            PotentiometerBase* d = (PotentiometerBase*)instance.custom_data;
            append(d->current_val, d->name);
            has_config = true;
        }
    }

    if(has_config) load();
}
