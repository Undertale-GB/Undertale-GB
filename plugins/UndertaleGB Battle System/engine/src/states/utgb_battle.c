#pragma bank 255


#include "data/states_defines.h"
#include "states/utgb_battle.h"
#include "data/game_globals.h"

#include "utgb_battle_definitions.h"

#include "system.h"
#include "camera.h"
#include "scroll.h"
#include "vm.h"
#include "ui.h"


void utgb_battle_init(void) BANKED {
    camera_offset_x = 0;
    camera_offset_y = 0;
    camera_deadzone_x = 0;
    camera_deadzone_y = 0;
    //PLAYER.dir = DIR_RIGHT;
    //actor_set_anim(&PLAYER, ANIM_CURSOR);

}

void UTGB_battle_data_init(SCRIPT_CTX * THIS) BANKED {
    THIS;
    // TODO: Add battle loading code
}


void utgb_battle_update(void) BANKED {


}


uint8_t call_far(const far_ptr_t *ptr) NONBANKED NAKED {
    ptr;
__asm
    ld h, d
    ld l, e
    ld a, (hl+)
    ld e, a         ; e = ptr->SEG
    ld a, (hl+)
    ld h, (hl)
    ld l, a         ; hl = ptr->SEG
    or h
    jp nz, ___sdcc_bcall_ehl
    ret
__endasm;
}

