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
#include <stdlib.h>

#include "InventoryEnginePlugin.h"

#define UI_BKG_COLOR 0 // use black background color

#define VAR_NAME1 19
#define VAR_NAME2 20
#define VAR_NAME3 21
#define VAR_NAME4 22
#define VAR_NAME5 23
#define VAR_NAME6 24



/*
concatanates an INT16 number to a string
use like "strcat()"
*/
void utgb_cat_var_to_string(INT16 value, UBYTE * string) OLDCALL BANKED {

    //find end of string
    while (*string) string++;

    //add number to string
    itoa(value, string, 10);

    return;
}


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
#define PM_DEFAULT THIS->bank, (MENU_CANCEL_B | MENU_SET_START)
const char pauseMenuInstSpeed[] = "\001\001";


const char pauseMenuMainStartPos[] = "\003\003\013";

void pauseMenuMainShow(SCRIPT_CTX * THIS) OLDCALL BANKED {

    // WIP: render textbox + text
    vm_overlay_clear(THIS, 0, 9, 7, 5, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, pauseMenuInstSpeed);
    strcat(d, pauseMenuMainStartPos);
    strcat(d, "ITEM\nSTAT\nCALL");
    strcat(d, "\n");

    //utgb_cat_var_to_string(30, d);

    vm_display_text(THIS, 0, 0);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

}





void ugb_show_pause_menu(SCRIPT_CTX * THIS) OLDCALL BANKED {

    uint8_t menu_level = 1;
    uint8_t choice1 = 1; // default selected menu item
    uint8_t choice2 = 1;
    uint8_t choice3 = 1;
    
    copy_screen_area_to_overlay(THIS, 0, 0, 20, 18);
    //copy_screen_area_to_overlay(THIS, 0, 0, 19, 17);
    vm_overlay_setpos(THIS, 0, 0);

    
    while (menu_level == 1) {

        
        pauseMenuMainShow(THIS);
        //choice1 = ui_run_menu(pauseMenuMain, PM_DEFAULT, PAUSE_MENU_MAIN_SIZE, choice1);

        // Main pause menu:
        
        struct menu_item_t pauseMenuMain[] = {
            {.X=1u, .Y=10u, .iL=0u, .iR=0u, .iU=0u, .iD=2u}, // ITEM
            {.X=1u, .Y=11u, .iL=0u, .iR=0u, .iU=1u, .iD=3u}, // STAT
            {.X=1u, .Y=12u, .iL=0u, .iR=0u, .iU=2u, .iD=0u}  // CALL
        };

        choice1 = ui_run_menu(pauseMenuMain, PM_DEFAULT, 3, choice1);
        

        
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
        

        //menu_level = 0;
    }
    

    vm_overlay_setpos(THIS, 0, 18);

    


}

