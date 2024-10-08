#pragma bank 255

#include <gbdk/platform.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include "vm_gameboy.h"
#include "vm.h"
#include "vm_ui.h"
#include "scroll.h"
#include <string.h>
#include "ui.h"
#include <types.h>
#include "gbs_types.h"
#include "palette.h"
#include <stdlib.h>
#include "bankdata.h"
#include "data/frame_image.h"
#include "data/game_globals.h"

#include "UTGBVariables.h"
#include "UTGBFonts.h"
#include "InventoryEnginePlugin.h"

#include "data/frame_image_yellow.h"

#define UI_BKG_COLOR 0 // use black background color

#define UTGB_UI_WHITE CGB_COLOR(31, 31, 31)
#define UTGB_UI_YELLOW CGB_COLOR(31, 31, 4)
#define UTGB_UI_RED CGB_COLOR(31, 0, 0)
#define UTGB_UI_BLACK CGB_COLOR(0, 0, 0)


const palette_entry_t UTGB_yellow_text = CGB_PALETTE(UTGB_UI_WHITE, UTGB_UI_YELLOW, UTGB_UI_RED, UTGB_UI_BLACK);
const palette_entry_t UTGB_white_text = CGB_PALETTE(UTGB_UI_WHITE, UTGB_UI_WHITE, UTGB_UI_RED, UTGB_UI_BLACK);

#define UTGB_UI_SET_TEXT_COLOR(newPalette) (set_bkg_palette(7, 1, (void *)&newPalette))

#define ui_frame_tl_tiles 0xC0u
#define UTGB_Set_Border_Yellow SetBankedBkgData(ui_frame_tl_tiles, 9, frame_image_yellow, BANK(frame_image_yellow))
#define UTGB_Set_Border_Default SetBankedBkgData(ui_frame_tl_tiles, 9, frame_image, BANK(frame_image))



/*
concatanates an INT16 number to a string
use like "strcat()"
*/
void utgb_cat_var_to_string(UBYTE * string, INT16 value) BANKED {

    //find end of string
    while (*string) string++;

    //add number to string
    ltoa(value, string, 10);

    return;
}

/*
concatanates an INT16 number to a string as a char
use like "strcat()"
*/
void utgb_cat_var_as_char(UBYTE * string, INT16 value) BANKED {

    while (*string) string++;

    *string = (char)value;

    *++string = 0;

}

/*
Moves a section of VRAM, updating IDs on tilemap to new position
New VRAM position can only be on Bank 2
Updating tile IDs only works on overlay
*/
void utgb_move_overlay_content_vram(uint8_t source_tile, uint8_t target_tile, uint8_t nb_tiles, uint8_t vram_reg) BANKED {

    uint8_t TempTileStorage[16]; // stores 1 tile

    VBK_REG = vram_reg;

    for(uint8_t i = 0; i < nb_tiles; i++) { // copy VRAM area

        get_bkg_data(source_tile + i, 1, TempTileStorage);

        set_bkg_data(target_tile + i, 1, TempTileStorage);

    }
    
    VBK_REG = 0;

    for(uint8_t y = 0; y < 18; y++) {
        for(uint8_t x = 0; x < 20; x++) { // change tile IDs on overlay to point to new location
            
            VBK_REG = 1;

            uint8_t tile_vram_banked = (get_win_tile_xy(x, y) & 0b00001000) >> 3; //get VRAM bank of tile

            VBK_REG = 0;
            
            uint8_t tile_index = get_win_tile_xy(x, y); // get tile ID

            if(tile_index >= source_tile && tile_index < source_tile + nb_tiles && tile_vram_banked) {
                set_win_tile_xy(x, y, tile_index + target_tile - source_tile);
            }
            
        }
    }
}


/*
Copies the specified area of the screen to the overlay
Useful for erasing parts of a fullscreen interface
*/
void copy_screen_area_to_overlay(SCRIPT_CTX * THIS, UBYTE x, UBYTE y, UBYTE w, UBYTE h) BANKED {
    
    UBYTE scene_x = ((scroll_x - 1) >> 3) + 1 + x;
    UBYTE scene_y = ((scroll_y - 1) >> 3) + 1 + y;

    vm_overlay_set_submap(THIS, x, y, w, h, scene_x, scene_y);
}



// Shorten ui_run_menu() commands
#define PM_DEFAULT _current_bank, (MENU_CANCEL_B | MENU_SET_START)
const char PM_InstSpeed[] = "\001\001";
const char PM_SmallFont[] = FONT_SMALL_COLOR;
const char PM_ColoredFont[] = FONT_VARWIDTH_COLOR;




#define PM_Dialogue_BBox 0, 13, 20, 5
#define PM_Dialogue_StartPos "\003\002\017"

void PM_Dialogue_Write(SCRIPT_CTX * THIS) BANKED {
    vm_idle(THIS);
    vm_overlay_clear(THIS, PM_Dialogue_BBox, UI_BKG_COLOR, UI_DRAW_FRAME);
    vm_display_text(THIS, 0, 52);
    vm_overlay_wait(THIS, 1, (UI_WAIT_TEXT | UI_WAIT_BTN_A));
}

#define PM_Dialogue_Hide(THIS) copy_screen_area_to_overlay(THIS, PM_Dialogue_BBox)



// ==================== Main Pause Menu ====================== //

const struct menu_item_t PM_Main[] = {
    {.X=1u, .Y=9u,  .iL=0u, .iR=0u, .iU=0u, .iD=2u}, // ITEM
    {.X=1u, .Y=10u, .iL=0u, .iR=0u, .iU=1u, .iD=3u}, // STAT
    {.X=1u, .Y=11u, .iL=0u, .iR=0u, .iU=2u, .iD=0u}  // CELL
};

const char PM_Main_StartPos[] = "\003\003\012";

void PM_Main_Show(SCRIPT_CTX * THIS) BANKED {

    // WIP: render textbox + text
    vm_overlay_clear(THIS, 0, 8, 7, 5, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, PM_Main_StartPos);
    strcat(d, PM_ColoredFont);
    strcat(d, "ITEM\nSTAT\nCELL");
    strcat(d, "\n");

    //utgb_cat_var_to_string(30, d);

    vm_display_text(THIS, 0, 17);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

}


void PM_Quick_Overview_Show(SCRIPT_CTX * THIS) BANKED {

    // WIP: render textbox + text
    vm_overlay_clear(THIS, 0, 1, 8, 7, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, "\003\002\003");
    strcat(d, PM_ColoredFont);
    
    INT16* namePtr = (INT16 *)VM_REF_TO_PTR(VAR_NAME1);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr  );
    
    

    strcat(d, "\n");
    strcat(d, PM_SmallFont);

    strcat(d, "\nLV: ");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_LEVEL));

    strcat(d, "\nHP: ");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_CURRENT_HP));
    strcat(d, "/");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_MAX_HP));

    strcat(d, "\nG: ");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_GOLD_MONEY_));


    vm_display_text(THIS, 0, 0);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

}



// =================== Item Menu ========================= //

const struct menu_item_t PM_Item_Selection[] = {
    {.X=10u, .Y=2u, .iL=0u, .iR=0u, .iU=0u, .iD=2u},
    {.X=10u, .Y=3u, .iL=0u, .iR=0u, .iU=1u, .iD=3u},
    {.X=10u, .Y=4u, .iL=0u, .iR=0u, .iU=2u, .iD=4u},
    {.X=10u, .Y=5u, .iL=0u, .iR=0u, .iU=3u, .iD=5u},
    {.X=10u, .Y=6u, .iL=0u, .iR=0u, .iU=4u, .iD=6u},
    {.X=10u, .Y=7u, .iL=0u, .iR=0u, .iU=5u, .iD=7u},
    {.X=10u, .Y=8u, .iL=0u, .iR=0u, .iU=6u, .iD=8u},
    {.X=10u, .Y=9u, .iL=0u, .iR=0u, .iU=7u, .iD=0u}
};

const struct menu_item_t PM_Item_Interact[] = {
    {.X=10u, .Y=11u, .iL=0u, .iR=2u, .iU=0u, .iD=0u},
    {.X=13u, .Y=11u, .iL=1u, .iR=3u, .iU=0u, .iD=0u},
    {.X=16u, .Y=11u, .iL=2u, .iR=0u, .iU=0u, .iD=0u}
};

#define PM_Item_BBox 9, 1, 11, 12
#define PM_Item_StartPos "\003\014\003"

void PM_Item_Show(SCRIPT_CTX * THIS) BANKED {

    vm_idle(THIS);
    vm_overlay_clear(THIS, PM_Item_BBox, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, PM_SmallFont);

    strcat(d, "\003\014\014USE");
    strcat(d, "\003\017\014INFO");
    strcat(d, "\003\022\014DROP");

    strcat(d, PM_Item_StartPos);

    inv_load_pause_menu(THIS);


    vm_display_text(THIS, 0, 29);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

    utgb_move_overlay_content_vram(192, 128, 64, 1);

}

#define PM_Item_Hide(THIS) copy_screen_area_to_overlay(THIS, PM_Item_BBox)


// =================== Item Submenu Functions ========================= //

void PM_Item_Use(SCRIPT_CTX * THIS, uint8_t itemSlot) BANKED { //on "USE"

    unsigned char * d = ui_text_data;
    *d = 0;
    strcat(d, PM_Dialogue_StartPos);
    strcat(d, PM_ColoredFont);

    if(inv_load_use_main_text(THIS, d, itemSlot, 1)){ //is there use text?
        PM_Dialogue_Write(THIS);
        *d = 0; //reset text buffer
        strcat(d, PM_Dialogue_StartPos);
        strcat(d, PM_ColoredFont);
    }


    if(inv_load_use_main_text(THIS, d, itemSlot, 2)){ //is there a second box of use text?
        PM_Dialogue_Write(THIS);
        *d = 0; //reset text buffer
        strcat(d, PM_Dialogue_StartPos);
        strcat(d, PM_ColoredFont);
    }

    bool hasStatText = inv_use_item_new(THIS, d, itemSlot); //use Item 
    if(hasStatText){ //does this item show stats on use? (like "Healed 10 HP!")
        PM_Dialogue_Write(THIS);

        PM_Quick_Overview_Show(THIS); //Update stats (left panel)
    }


    PM_Dialogue_Hide(THIS); // hide text box

}

void PM_Item_Info(SCRIPT_CTX * THIS, uint8_t itemSlot) BANKED { //on "INFO"

    unsigned char * d = ui_text_data;
    *d = 0;

    strcat(d, PM_Dialogue_StartPos);
    strcat(d, PM_ColoredFont);
    inv_load_info_stats(THIS, d, itemSlot); // Load stat text like "*ItemName\nHeals X HP"
    PM_Dialogue_Write(THIS);

    *d = 0; //clear text buffer

    strcat(d, PM_Dialogue_StartPos);
    strcat(d, PM_ColoredFont);
    inv_load_info_desc(THIS, d, itemSlot); // Load item description
    PM_Dialogue_Write(THIS);

    PM_Dialogue_Hide(THIS); // hide text box
}

void PM_Item_Drop(SCRIPT_CTX * THIS, uint8_t itemSlot) BANKED { //on "DROP"

    unsigned char * d = ui_text_data;
    *d = 0;
    strcat(d, PM_Dialogue_StartPos);
    strcat(d, PM_ColoredFont);

    inv_drop_item_new(THIS, d, itemSlot);

    PM_Dialogue_Write(THIS);

    PM_Dialogue_Hide(THIS); // hide text box
}




// ===================== Stat Menu =============================== //

#define PM_Stat_BBox 10, 1, 10, 12
#define PM_Stat_StartPos "\003\014\003"

void PM_Stat_Show(SCRIPT_CTX * THIS) BANKED {

    vm_overlay_clear(THIS, PM_Stat_BBox, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;

    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, PM_Stat_StartPos);
    strcat(d, PM_ColoredFont);

    // Write name:
    strcat(d, "\"");
    INT16* namePtr = (INT16 *)VM_REF_TO_PTR(VAR_NAME1);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr++);
    utgb_cat_var_as_char(d, *namePtr  );
    strcat(d, "\"\n\n");

    strcat(d, PM_SmallFont);

    INT16 attack = VAR_VAL(VAR_ATTACK_BASE_) + VAR_VAL(VAR_ATTACK_ITEM_);
    INT16 defense = VAR_VAL(VAR_DEFENSE_BASE_) + VAR_VAL(VAR_DEFENSE_ITEM_);

    strcat(d, "AT: ");
    utgb_cat_var_to_string(d, attack);
    strcat(d, "(");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_ATTACK_BASE_));
    strcat(d, ")\n");

    strcat(d, "DF: ");
    utgb_cat_var_to_string(d, defense);
    strcat(d, "(");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_DEFENSE_BASE_));
    strcat(d, ")\n");

    strcat(d, "EXP: ");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_EXP));
    strcat(d, "\n");

    strcat(d, "NEXT: ");
    utgb_cat_var_to_string(d, VAR_VAL(VAR_EXP_TO_NEXT_LEVEL));
    strcat(d, "\n\n");

    strcat(d, "WPN: ");
    inv_load_item_name(THIS, 0, 1);
    strcat(d, "\nAMR: ");
    inv_load_item_name(THIS, 1, 1);




    //strcat(d, "\n\nPut\nStats\nhere");



    vm_display_text(THIS, 0, 29);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);

}

#define PM_Stat_Hide(THIS) copy_screen_area_to_overlay(THIS, PM_Stat_BBox)




// ===================== Cell Menu =============================== //

#define PM_Cell_BBox 8, 9, 11, 4
#define PM_Cell_StartPos "\003\012\013"

void PM_Cell_Show(SCRIPT_CTX * THIS) BANKED {
    vm_overlay_clear(THIS, PM_Cell_BBox, UI_BKG_COLOR, UI_DRAW_FRAME);

    unsigned char * d = ui_text_data;
    *d = 0;

    strcat(d, PM_InstSpeed);
    strcat(d, PM_Cell_StartPos);
    strcat(d, PM_ColoredFont);

    strcat(d, "Cell Menu\nplaceholder");

    vm_display_text(THIS, 0, 29);
    vm_overlay_wait(THIS, 1, UI_WAIT_TEXT);
}

#define PM_Cell_Hide(THIS) copy_screen_area_to_overlay(THIS, PM_Cell_BBox)



// ===================== Run Menu System =============================== //

void ugb_show_pause_menu(SCRIPT_CTX * THIS) BANKED {

    uint8_t menu_level = 1; //stacked menu level
    uint8_t choice1 = 1; // default selected menu item
    uint8_t choice2 = 1;
    uint8_t choice3 = 1;

    UTGB_UI_SET_TEXT_COLOR(UTGB_white_text);
    UTGB_Set_Border_Yellow;
    
    copy_screen_area_to_overlay(THIS, 0, 0, 20, 18);
    vm_overlay_setpos(THIS, 0, 0);

    //fix overlay position on moving camera
    //neccesary for adventure mode conversion
    win_pos_x = 8 - scroll_x & 0b0111;
    win_dest_pos_x = 8 - scroll_x & 0b0111;
    win_pos_y = 8 - scroll_y & 0b0111;
    win_dest_pos_y = 8 - scroll_y & 0b0111;

    //Show main menu
    PM_Quick_Overview_Show(THIS);

    PM_Main_Show(THIS);

    while (menu_level == 1) {

        //Run main menu
        choice1 = ui_run_menu(PM_Main, PM_DEFAULT, 3, choice1);
        
        switch (choice1)
        {
        case 1: // ITEM

            menu_level++;
            
            while (menu_level == 2) {

                PM_Item_Show(THIS); // Re-render on backwards entry
                
                choice2 = ui_run_menu(PM_Item_Selection, PM_DEFAULT, 8, choice2);

                if(choice2 == 0) { //if B pressed:
                    PM_Item_Hide(THIS);

                    choice2 = 1; // reset default selected menu item
                    menu_level--; // back 1 menu
                    continue;
                }

                if(!inv_get_item(THIS, choice2 - 1)) continue; // skip if slot has no item

                // If Item Selected:
                choice3 = 1;
                choice3 = ui_run_menu(PM_Item_Interact, PM_DEFAULT, 3, choice3);

                switch (choice3)
                {
                case 1: // USE
                    
                    PM_Item_Use(THIS, choice2 - 1);
                    break;

                case 2: // INFO
                    
                    PM_Item_Info(THIS, choice2 - 1);
                    break;

                case 3: // DROP
                    
                    PM_Item_Drop(THIS, choice2 - 1);
                    break;
                
                default: // B pressed
                    break;
                }
                
            }
            break;

        case 2: // STAT

            menu_level++;
            while (menu_level == 2) {

                PM_Stat_Show(THIS);

                vm_overlay_wait(THIS, 1, UI_WAIT_BTN_B);// Wait for B press

                PM_Stat_Hide(THIS);

                choice2 = 1; // reset default selected menu item
                menu_level--; // back 1 menu
            }
            break;

        case 3: // CELL
            // TODO: Code for Cell menu
            menu_level++;
            while (menu_level == 2) {


                // TODO: Code for Cell menu

                PM_Cell_Show(THIS);

                vm_overlay_wait(THIS, 1, UI_WAIT_BTN_B);// Wait for B press

                PM_Cell_Hide(THIS);

                choice2 = 1; // reset default selected menu item
                menu_level--; // back 1 menu
            }
            break;

        default:
            choice1 = 1; // reset default selected menu item
            menu_level--; // back by 1 menu
            break;
        }
        
    }
    

    vm_overlay_setpos(THIS, 0, 18);

    UTGB_UI_SET_TEXT_COLOR(UTGB_yellow_text);
    UTGB_Set_Border_Default;
    


}

