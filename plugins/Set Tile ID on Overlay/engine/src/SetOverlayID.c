#pragma bank 255

#include <gbdk/platform.h>
#include "vm.h"
#include "scroll.h"

void deab_overlay_tile_set(SCRIPT_CTX * THIS) OLDCALL BANKED {

    uint8_t x = *(int16_t*)VM_REF_TO_PTR(FN_ARG2);
    uint8_t y = *(int16_t*)VM_REF_TO_PTR(FN_ARG1);
    int16_t tile = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);

    set_win_tile_xy(x, y, tile);

}
