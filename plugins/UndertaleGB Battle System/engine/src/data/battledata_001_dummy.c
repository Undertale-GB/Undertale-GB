#pragma bank 255

// Yellow Frame

#include "UTGBTypes.h"

BANKREF(battledata_001_dummy)

const battledata_t battledata_001_dummy[] = {
    {
        .introText = "You approach the Dummy.",
        .testTxt = "Some Test"
    }
};

/*
BANKREF(battledata_001_act)
const unsigned char temp[] = {0x3C};

void battledata_001_act(void) BANKED {
    set_bkg_tiles(2, 3, 1, 1, temp);
}
*/
