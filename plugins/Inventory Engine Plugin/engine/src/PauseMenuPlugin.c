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
#define PM_DEFAULT _current_bank, (MENU_CANCEL_B | MENU_SET_START)
const char PM_InstSpeed[] = "\001\001";





// ==================== Main Pause Menu ====================== //

const struct menu_item_t PM_Main[] = {
    {.X=1u, .Y=10u, .iL=0u, .iR=0u, .iU=0u, .iD=2u}, // ITEM
    {.X=1u, .Y=11u, .iL=0u, .iR=0u, .iU=1u, .iD=3u}, // STAT
    {.X=1u, .Y=12u, .iL=0u, .iR=0u, .iU=2u, .iD=0u}  // CALL
};

const char PM_Main_StartPos[] = "\003\003\013";

void PM_Main_Show(SCRIPT_CTX * THIS) OLDCALL BANKED {

    // WIP: render textbox + text
    vm_overlay_clear(THIS, 0, 9, 7, 5, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, PM_Main_StartPos);
    strcat(d, "ITEM\nSTAT\nCALL");
    strcat(d, "\n");

    //utgb_cat_var_to_string(30, d);

    vm_display_text(THIS, 0, 0);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

}




// =================== Item Menu ========================= //

const struct menu_item_t PM_Item_Selection[] = {
    {.X=9u, .Y=3u, .iL=0u, .iR=0u, .iU=0u, .iD=2u},
    {.X=9u, .Y=4u, .iL=0u, .iR=0u, .iU=1u, .iD=3u},
    {.X=9u, .Y=5u, .iL=0u, .iR=0u, .iU=2u, .iD=4u},
    {.X=9u, .Y=6u, .iL=0u, .iR=0u, .iU=3u, .iD=5u},
    {.X=9u, .Y=7u, .iL=0u, .iR=0u, .iU=4u, .iD=6u},
    {.X=9u, .Y=8u, .iL=0u, .iR=0u, .iU=5u, .iD=7u},
    {.X=9u, .Y=9u, .iL=0u, .iR=0u, .iU=6u, .iD=8u},
    {.X=9u, .Y=10u, .iL=0u, .iR=0u, .iU=7u, .iD=0u}
};

#define PM_Item_BBox 8, 2, 11, 12
#define PM_Item_StartPos "\003\013\004"

void PM_Item_Show(SCRIPT_CTX * THIS) OLDCALL BANKED {

    vm_overlay_clear(THIS, PM_Item_BBox, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, PM_Item_StartPos);
    //strcat(d, "put\nItems\nhere");

    inv_load_pause_menu(THIS);

    vm_display_text(THIS, 0, 13);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

}

#define PM_Item_Hide(THIS) copy_screen_area_to_overlay(THIS, PM_Item_BBox)




// ===================== Stat Menu =============================== //

#define PM_Stat_BBox 10, 2, 8, 10
#define PM_Stat_StartPos "\003\013\003"

void PM_Stat_Show(SCRIPT_CTX * THIS) OLDCALL BANKED {

    vm_overlay_clear(THIS, PM_Stat_BBox, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, PM_Stat_StartPos);
    //strcat(d, "put\nItems\nhere");

    strcat(d, "Put Stats\nhere");

    vm_display_text(THIS, 0, 13);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

}

#define PM_Stat_Hide(THIS) copy_screen_area_to_overlay(THIS, PM_Stat_BBox)









void ugb_show_pause_menu(SCRIPT_CTX * THIS) OLDCALL BANKED {

    uint8_t menu_level = 1; //stacked menu level
    uint8_t choice1 = 1; // default selected menu item
    uint8_t choice2 = 1;
    uint8_t choice3 = 1;
    
    copy_screen_area_to_overlay(THIS, 0, 0, 20, 18);
    vm_overlay_setpos(THIS, 0, 0);


    //Show main menu
    PM_Main_Show(THIS);

    while (menu_level == 1) {

        //Run main menu
        choice1 = ui_run_menu(PM_Main, PM_DEFAULT, 3, choice1);
        
        switch (choice1)
        {
        case 1: // ITEM

            menu_level++;
            PM_Item_Show(THIS);
            
            while (menu_level == 2) {
                
                choice2 = ui_run_menu(PM_Item_Selection, PM_DEFAULT, 8, choice2);

                if(choice2 == 0) { //if B pressed:
                    PM_Item_Hide(THIS);

                    choice2 = 1; // reset default selected menu item
                    menu_level--; // back 1 menu
                    continue;
                }
                // If Item Selected:

                // TODO: Code for Item menu
                
            }
            break;

        case 2: // STAT
            // TODO: Code for Stat menu
            menu_level++;
            while (menu_level == 2) {


                // TODO: Code for Item menu

                PM_Stat_Show(THIS);

                vm_overlay_wait(THIS, 1, UI_WAIT_BTN_B);// Wait for B press

                PM_Stat_Hide(THIS);

                choice2 = 1; //debug // reset default selected menu item
                menu_level--; //debug
            }
            break;

        case 3: // CALL
            // TODO: Code for Call menu
            menu_level++;
            while (menu_level == 2) {


                // TODO: Code for Item menu

                //PM_Call_Show(THIS);

                vm_overlay_wait(THIS, 1, UI_WAIT_BTN_B);// Wait for B press
                choice2 = 1; //debug // reset default selected menu item
                menu_level--; //debug
            }
            break;

        default:
            choice1 = 1; // reset default selected menu item
            menu_level--; // back by 1 menu
            break;
        }
        
    }
    

    vm_overlay_setpos(THIS, 0, 18);

    


}

