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

//check "game_globals.i" in exported data to find values
#define InvMainPtr 34
#define EquippedItemsPtr 41
#define VAR_CURRENT_HP 2
#define VAR_MAX_HP 3
#define VAR_ATTACK_ITEM_ 26
#define VAR_DEFENSE_ITEM_ 27

const char textSpeedStr[] = "\001\001";
const char fontSmallStr[] = "\002\005";
const char menuMainStartPosStr[] = "\003\013\002";
const char menuInvStartPosStr[] = "\003\002\011";
const char battleInvStartPosStr[] = "\003\004\002";


typedef struct item_t {
    unsigned char name[32];
    unsigned char nameShort[10];
    unsigned char nameSerious[10];
    uint8_t       useType; //1 = Heal, 2 = Weapon, 3 = Armor
    uint8_t       amount;
    unsigned char amountStr[16];

    unsigned char desc[64];
    unsigned char useBattle[64];
    unsigned char useMain[64];
    unsigned char useMain2[64];
} item_t;


const struct item_t items[] = {
    {
    .name = "",
    .nameShort = "",
    .desc = ""
    },
    {
    .name = "Bandage",
    .nameShort = "Bandage",
    .useType = 1,
    .amount = 10,
    .amountStr = "10",
    .desc = "*It has already\n|been used many\n|times.",
    .useMain = "*You re-applied\n|the bandage.\n*Still kind of gooey.",
    .useBattle = "*You re-applied\n|the bandage.\n*Still kind of\n|gooey.",
    },
    {
    .name = "Monster Candy",
    .nameShort = "MnstrCndy",
    .useType = 1,
    .amount = 10,
    .amountStr = "10",
    .desc = "*Has a distinct,\n|non-licorice\n|flavor.",
    .useMain = "*You ate the\n|Monster Candy.",
    .useMain2 = "*Very\n|un-licorice-like.",
    .useBattle = "*You ate the\n|Monster Candy.\n*Very\n|un-licorice-like",
    },
    {
    .name = "Spider Donut",
    .nameShort = "SpidrDont",
    .useType = 1,
    .amount = 12,
    .amountStr = "12",
    .desc = "*A donut made with\n|Spider Cider in\n|the batter.",
    .useMain = "*You ate the\n|Spider Donut.",
    .useBattle = "",
    },
    {
    .name = "Spider Cider",
    .nameShort = "SpidrCidr",
    .useType = 1,
    .amount = 24,
    .amountStr = "24",
    .desc = "*Made with whole\n|spiders, not just\n|the juice.",
    .useMain = "*You drank the\n|Spider Cider.",
    .useBattle = "",
    },
    {
    .name = "Butterscotch Pie",
    .nameShort = "ButtsPie",
    .useType = 1,
    .amount = 127,
    .amountStr = "All",
    .desc = "*Butterscotch-\n|cinnamon pie,\n|one slice.",
    .useMain = "*You ate the\n|Butterscotch Pie.",
    .useBattle = "*You ate the\n|Butterscotch\n|Pie.",
    },
    {
    .name = "Stick",
    .nameShort = "Stick",
    .useType = 0,
    .amount = 0,
    .amountStr = "|Weapon AT 0",
    .desc = "*Its bark is\n|worse than its\n|bite.",
    .useMain = "*You threw the\n|stick away.",
    .useMain2 = "*Then picked it\n|back up.",
    .useBattle = "*You threw the\n|stick away.\n*Then picked it\n|back up.",
    },
    {
    .name = "Toy Knife",
    .nameShort = "Toy Knife",
    .useType = 2,
    .amount = 3,
    .amountStr = "3",
    .desc = "*Made of plastic.\n*A rarity nowadays.",
    .useMain = "*You equipped the\n|Toy Knife.",
    .useBattle = "",
    },
    {
    .name = "Faded Ribbon",
    .nameShort = "Ribbon",
    .useType = 3,
    .amount = 3,
    .amountStr = "3",
    .desc = "*If you're cuter,\n|monsters won't hit\n|you as hard.",
    .useMain = "*You equipped the\n|Faded Ribbon.",
    .useBattle = "",
    },
};

const char maxHPStr[] = "*Your HP was\n|maxed out!";

//Unused
/*
void inv_load_item_name(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;

    unsigned char * d = ui_text_data;

    int16_t itemID = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);
    uint8_t i = 0;
    while (textSpeedStr[i] != '\0')
    {
        *d++ = textSpeedStr[i++];
    }
    i = 0;
    while (items[itemID].name[i] != '\0')
    {
        *d++ = items[itemID].name[i++];
    }

    *d = 0;

}
*/

void inv_load_item_name(SCRIPT_CTX * THIS, uint8_t InvSlot, uint8_t InvType) OLDCALL BANKED {
    THIS;
    
    unsigned char * d = ui_text_data;

    uint8_t* invPtr;

    switch (InvType)
    {
    case 0:
        invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
        break;

    case 1:
        invPtr = (uint8_t *)VM_REF_TO_PTR(EquippedItemsPtr);
        break;
    
    default:
        invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr); // default to base inventory
        break;
    }

    uint8_t itemID = invPtr[InvSlot];

    strcat(d, items[itemID].name);
}


void inv_read_item(SCRIPT_CTX * THIS) OLDCALL BANKED {//On Stack: Inventory Slot, Inventory Type

    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);

    if (*(int16_t*)VM_REF_TO_PTR(FN_ARG1) == 1){
        invPtr = (uint8_t *)VM_REF_TO_PTR(EquippedItemsPtr);//This function is for basically everything i haven't remade yet.
    }                                                       //I'm remaking it for legacy code, but not putting too much work into it.
    uint8_t slot = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);

    *(int16_t*)VM_REF_TO_PTR(FN_ARG0) = invPtr[slot];

}

void inv_load_pause_menu(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;

    unsigned char * d = ui_text_data;
    //*d = 0;

    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);

    //strcat(d, textSpeedStr);
    //strcat(d, fontSmallStr);
    //strcat(d, menuMainStartPosStr);
    for(uint8_t i = 0; i < 8; i++){

        strcat(d, items[invPtr[i]].name);
        strcat(d, "\n");
    }
}

void inv_add_item(SCRIPT_CTX * THIS) OLDCALL BANKED {//On Stack: Item ID

    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    int16_t * itemPtr = (int16_t*)VM_REF_TO_PTR(FN_ARG0);
    if(invPtr[7] == 0){//if any slot empty

        uint8_t i = 0;
        while(invPtr[i] != 0) i++;
        invPtr[i] = *itemPtr;
        return;
    }
    *itemPtr = 0;//if no empty slot
    
} //Stack Out: 0 if inventory full, otherwise Item ID

void inv_obtain_item(SCRIPT_CTX * THIS) OLDCALL BANKED {//On Stack: Item ID
    
    unsigned char * d = ui_text_data;
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    
    int16_t * itemPtr = (int16_t*)VM_REF_TO_PTR(FN_ARG0);

    *d = 0;
    //strcat(d, menuInvStartPosStr);
    strcat(d, "*You got the\n|");
    strcat(d, items[*itemPtr].name);
    strcat(d, "!");

    vm_overlay_clear(THIS, 0, 0, 20, 5, 0, 1);
    vm_overlay_move_to(THIS, 0, 13, UI_IN_SPEED);
    vm_display_text(THIS, 0, 68);
    vm_overlay_wait(THIS, 1, 6);

    if(invPtr[7] != 0) {
        *d = 0;
        strcat(d, "*...but your\n|Inventory was\n|full!");

        vm_overlay_clear(THIS, 0, 0, 20, 5, 0, 1);
        vm_display_text(THIS, 0, 68);
        vm_overlay_wait(THIS, 1, 6);
        vm_overlay_move_to(THIS, 0, 18, UI_OUT_SPEED);
        return;
    }
    vm_overlay_move_to(THIS, 0, 18, UI_OUT_SPEED);
    inv_add_item(THIS);

} //Stack Out: 0 if inventory full, otherwise Item ID

void inv_remove_item_new(SCRIPT_CTX * THIS, uint8_t invSlot) OLDCALL BANKED {
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);

    for (uint8_t i = invSlot; i<7; i++){
        invPtr[i] = invPtr[i + 1];
    }
    invPtr[7] = 0;
}

//deprecated
//TODO: Remove all calls to this function
void inv_remove_item(SCRIPT_CTX * THIS) OLDCALL BANKED {//On Stack: Inventory Slot
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    uint8_t rSlot = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);

    for (uint8_t i = rSlot; i<7; i++){
        invPtr[i] = invPtr[i + 1];
    }
    invPtr[7] = 0;
}

bool inv_load_use_main_text(SCRIPT_CTX * THIS, UBYTE * string, uint8_t invSlot, uint8_t textNum) OLDCALL BANKED {
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);

    uint8_t item = invPtr[invSlot];

    const UBYTE * textPtr;

    switch (textNum)
    {
    case 1:
        textPtr = items[item].useMain;
        break;

    case 2:
        textPtr = items[item].useMain2;
        break;
    
    default:
        return false;
        break;
    }

    if(textPtr[0]){

        strcat(string, textPtr);
        return true;
    }

    return false;
}

bool inv_use_item_new(SCRIPT_CTX * THIS, UBYTE * string, uint8_t invSlot) OLDCALL BANKED {

    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    uint8_t * equipPtr = (uint8_t *)VM_REF_TO_PTR(EquippedItemsPtr);
    int16_t * currentHP = (int16_t*)VM_REF_TO_PTR(VAR_CURRENT_HP);
    int16_t * maxHP = (int16_t*)VM_REF_TO_PTR(VAR_MAX_HP);

    uint8_t item = invPtr[invSlot];
    bool hasText = false;

    while (*string) string++;

    switch (items[item].useType){

        case 1://Consumable

            hasText = true;
            *currentHP += items[item].amount;
            if(*currentHP >= *maxHP)
            {
                *currentHP = *maxHP;
                strcat(string, maxHPStr);
            }else{
                strcat(string, "You Healed ");
                strcat(string, items[item].amountStr);
                strcat(string, " HP.");
            }

            inv_remove_item_new(THIS, invSlot);
            break;

        case 2://Weapon
        case 3://Armor

            if(1){//to allow variable declaration in switch
                uint8_t equipSlot = items[item].useType & 0b1;//0 = Weapon, 1 = Armor
                uint8_t itemTemp = equipPtr[equipSlot];
                equipPtr[equipSlot] = item; // store item in equip slot
                inv_remove_item_new(THIS, invSlot); // remove item from inventory

                uint8_t i = 0;
                while (invPtr[i] != 0) i++;
                invPtr[i] = itemTemp; // store unequipped item in inventory

                if(equipSlot == 0){ // set attack / defense stat
                    *(int16_t*)VM_REF_TO_PTR(VAR_ATTACK_ITEM_) = (int16_t)items[item].amount;
                }else{
                    *(int16_t*)VM_REF_TO_PTR(VAR_DEFENSE_ITEM_) = (int16_t)items[item].amount;
                }

            }
            
            break;
        default:
            break;
    }

    return hasText;
    
}

//deprecated
//TODO: Remove all calls to this function from GBVM
void inv_use_item(SCRIPT_CTX * THIS) OLDCALL BANKED {//On Stack: Inventory Slot, bool isInBattle

    unsigned char * d = ui_text_data;
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    uint8_t * equipPtr = (uint8_t *)VM_REF_TO_PTR(EquippedItemsPtr);
    int16_t * currentHP = (int16_t*)VM_REF_TO_PTR(VAR_CURRENT_HP);
    int16_t * maxHP = (int16_t*)VM_REF_TO_PTR(VAR_MAX_HP);
    uint8_t slot = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);
    uint8_t inBattle = *(uint8_t*)VM_REF_TO_PTR(FN_ARG1);
    uint8_t item = invPtr[slot];

    if(item == 0) return;

    const char * startPosString = menuInvStartPosStr;
    if(inBattle) startPosString = battleInvStartPosStr;

    *d = 0;

    strcat(d, startPosString); // Set text starting position
    if(inBattle && items[item].useBattle[0] != 0){

        strcat(d, items[item].useBattle);

    }else{

        strcat(d, items[item].useMain);
    }

    if(inBattle){

        vm_overlay_clear(THIS, 3, 1, 14, 4, 0, 0);
        vm_display_text(THIS, 0, 5);

    }else{

        vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1);
        vm_display_text(THIS, 0, 68);
    }

    vm_overlay_wait(THIS, 1, 6);

    if(items[item].useMain2[0] != 0 && !inBattle){//if text has 2 parts
        *d = 0;
        strcat(d, startPosString);
        strcat(d, items[item].useMain2);

        if(inBattle){
            vm_overlay_clear(THIS, 3, 1, 14, 4, 0, 0);
            vm_display_text(THIS, 0, 5);
        }else{
            vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1);
            vm_display_text(THIS, 0, 68);
        }

        vm_overlay_wait(THIS, 1, 6);
    }
    *d = 0;

    switch (items[item].useType){

        case 1://Consumable
            strcat(d, startPosString);
            *currentHP += items[item].amount;
            if(*currentHP >= *maxHP)
            {
                *currentHP = *maxHP;
                strcat(d, maxHPStr);
            }else{
                strcat(d, "You Healed ");
                strcat(d, items[item].amountStr);
                strcat(d, " HP.");
            }
            if(inBattle){
                vm_overlay_clear(THIS, 3, 1, 14, 4, 0, 0);
                vm_display_text(THIS, 0, 5);
            }else{
                vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1);
                vm_display_text(THIS, 0, 68);
            }
            
            vm_overlay_wait(THIS, 1, 6);

            inv_remove_item(THIS);
            break;

        case 2://Weapon
        case 3://Armor

            if(1){//to allow variable declaration in switch
                uint8_t equipSlot = items[item].useType & 0b1;//0 = Weapon, 1 = Armor
                uint8_t itemTemp = equipPtr[equipSlot];
                equipPtr[equipSlot] = item; // store item in equip slot
                inv_remove_item(THIS); // remove item from inventory

                uint8_t i = 0;
                while (invPtr[i] != 0) i++;
                invPtr[i] = itemTemp; // store unequipped item in inventory

                if(equipSlot == 0){ // set attack / defense stat
                    *(int16_t*)VM_REF_TO_PTR(VAR_ATTACK_ITEM_) = (int16_t)items[item].amount;
                }else{
                    *(int16_t*)VM_REF_TO_PTR(VAR_DEFENSE_ITEM_) = (int16_t)items[item].amount;
                }

            }
            
            break;
        default:
            return;
    }
    
}


void inv_drop_item(SCRIPT_CTX * THIS) OLDCALL BANKED {//On Stack: Inventory Slot
    
    unsigned char * d = ui_text_data;
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    
    uint8_t slot = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);
    uint8_t item = invPtr[slot];

    *d = 0;
    strcat(d, menuInvStartPosStr);
    strcat(d, "*You dropped the\n");
    strcat(d, items[item].name);
    strcat(d, ".");

    vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1);
    vm_display_text(THIS, 0, 68);

    inv_remove_item(THIS);

}

void inv_write_item_desc(SCRIPT_CTX * THIS) OLDCALL BANKED {//On Stack: Inventory Slot

    unsigned char * d = ui_text_data;
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    
    uint8_t slot = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);
    uint8_t item = invPtr[slot];

    if(item == 0) return;

    *d = 0;
    strcat(d, menuInvStartPosStr);
    strcat(d, "*");
    strcat(d, items[item].name);
    strcat(d, "\n\n");

    switch (items[item].useType){

        case 1://Consumable

            strcat(d, "|Heals ");
            strcat(d, items[item].amountStr);
            strcat(d, " HP");
            break;

        case 2://Weapon

            strcat(d, "|Weapon AT ");
            strcat(d, items[item].amountStr);
            break;

        case 3://Armor

            strcat(d, "|Armor DF ");
            strcat(d, items[item].amountStr);
            break;

        default://special cases

            strcat(d, items[item].amountStr);
            break;
    }
    vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1); // show text
    vm_display_text(THIS, 0, 68);
    vm_overlay_wait(THIS, 1, 6);

    *d = 0;
    strcat(d, menuInvStartPosStr);
    strcat(d, items[item].desc);

    vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1); // show text
    vm_display_text(THIS, 0, 68);
    vm_overlay_wait(THIS, 1, 6);
}

//untested
uint8_t inv_get_item_count(SCRIPT_CTX * THIS) OLDCALL BANKED {

    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);

    uint8_t i = 0;
    
    while(i < 8) {

        if (invPtr[i] == 0) break;

        i++;
    }

    return i + 1;

}