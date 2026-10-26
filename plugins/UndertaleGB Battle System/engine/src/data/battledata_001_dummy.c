#pragma bank 255


/*
#include "UTGBTypes.h"

BANKREF(battledata_001_dummy)

const battledata_t battledata_001_dummy[] = {
    {
        .introText = "You approach the Dummy.",
        .testTxt = "Some Test"
    }
};


BANKREF(battledata_001_act)
const unsigned char temp[] = {0x3C};

void battledata_001_act(void) BANKED {
    set_bkg_tiles(2, 3, 1, 1, temp);
}
*/

// REWRITE START

#include "UGB_Battle_Types.h"
#include "data/battledata_001_dummy.h"

BANKREF(battle_001_init)
BANKREF_EXTERN(battle_001_init)
void battle_001_init(void) BANKED {

}

BANKREF(battle_001_update)
BANKREF_EXTERN(battle_001_update)
void battle_001_update(void) BANKED {

}


BANKREF(battle_001)
//BANKREF_EXTERN(battle_001)
const battle_def_t battle_001 = {
    //.funcInit = TO_FAR_PTR_T(battle_001_init),
    .funcInit = {.bank = (UBYTE)&(__bank_battle_001), .ptr = (void *)&(battle_001_init)},
    .funcUpdate = TO_FAR_PTR_T(battle_001_update)
};
