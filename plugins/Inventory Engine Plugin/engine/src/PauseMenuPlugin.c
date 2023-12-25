#pragma bank 255

#include <gbdk/platform.h>
#include "vm_gameboy.h"
#include "vm.h"
#include "vm_ui.h"
#include "scroll.h"
#include <string.h>
#include "ui.h"
#include <types.h>
#include "gbs_types.h"

#include "InventoryEnginePlugin.h"

#define UI_BKG_COLOR 0 // use black background color

#define VAR_NAME1 19
#define VAR_NAME2 20
#define VAR_NAME3 21
#define VAR_NAME4 22
#define VAR_NAME5 23
#define VAR_NAME6 24



// WIP: text loader
// might not be neccesary

/*
void ugb_load_text_with_vars(SCRIPT_CTX * THIS, char * target, char * string, UBYTE * vars) {

    INT16 value;

    while (*string) {

        if (*string == '%') {
            value = *((INT16 *)VM_REF_TO_PTR(*vars));
            switch (*++string) {
            // variable value of fixed width, zero padded
            case 'D':
                target += itoa_format(value, target, *++string - '0');
                break;
            // variable value
            case 'd':
                target += itoa_format(value, target, 0);
                break;
            // char from variable
            case 'c':
                *target++ = (unsigned char)value;
                break;
            // text tempo from variable
            case 't':
                *target++ = 0x01u;
                *target++ = (unsigned char)value + 0x02u;
                break;
            // font index from variable
            case 'f':
                *target++ = 0x02u;
                *target++ = (unsigned char)value + 0x01u;
                break;
            // excape % symbol
            case '%':
                string++;
            default:
                string--;
                *target++ = *string++;
                continue;

            }

        } else {
            *target++ = *string++;
            continue;
        }
        string++; vars++;
        

    }
    *target = 0;

}

*/



/*
Copies the specified area of the screen to the overlay
Useful for erasing parts of a fullscreen interface
*/
void copy_screen_area_to_overlay(SCRIPT_CTX * THIS, UBYTE x, UBYTE y, UBYTE w, UBYTE h) OLDCALL BANKED {
    
    UBYTE scene_x = (scroll_x >> 3) + x;
    UBYTE scene_y = (scroll_y >> 3) + y;

    vm_overlay_set_submap(THIS, x, y, w, h, scene_x, scene_y);
}



// Shorten ui_run_menu() commands
#define PM_DEFAULT THIS->bank, MENU_CANCEL_B


// Main pause menu:
#define PAUSE_MENU_MAIN_SIZE 3
const struct menu_item_t pauseMenuMain[] = {

    {2, 2, 0, 0, 0, 2}, // ITEM
    {2, 3, 0, 0, 1, 3}, // STAT
    {2, 4, 0, 0, 2, 0}  // CALL
};

/*
__asm





__endasm
*/

const char menuMainStartPos[] = "\003\003\003";

void pauseMenuMainShow(SCRIPT_CTX * THIS) OLDCALL BANKED {

    // WIP: render textbox + text
    vm_overlay_clear(THIS, 2, 2, 8, 8, UI_BKG_COLOR, UI_DRAW_FRAME);

}





void ugb_show_pause_menu(SCRIPT_CTX * THIS) OLDCALL BANKED {

    uint8_t menu_level = 1;
    uint8_t choice1 = 1; // default selected menu item
    uint8_t choice2 = 1;
    uint8_t choice3 = 1;

    copy_screen_area_to_overlay(THIS, 0, 0, 20, 18);

    while (menu_level == 1) {

        pauseMenuMainShow(THIS);
        choice1 = ui_run_menu(pauseMenuMain, PM_DEFAULT, PAUSE_MENU_MAIN_SIZE, choice1);

        switch (choice1)
        {
        case 1: // ITEM
            menu_level++;
            while (menu_level == 2) {


                // TODO: Code for Item menu

                vm_input_wait(THIS, 32); // Wait for B press
                choice2 = 1; //debug // reset default selected menu item
                menu_level--; //debug
            }
            break;

        case 2: // STAT
            // TODO: Code for Stat menu

            


            vm_input_wait(THIS, 32);
            break;

        case 3: // CALL
            // TODO: Code for Call menu
            break;

        default:
            choice1 = 1; // reset default selected menu item
            menu_level--; // back by 1 menu
            break;
        }
    }






}