#pragma bank 255

#include <gbdk/platform.h>
#include "vm.h"
#include "vm_ui.h"
#include "scroll.h"
#include <string.h>
#include "ui.h"
#include <types.h>
#include "gbs_types.h"

#define InvMainPtr 34
#define VAR_CURRENT_HP 2
#define VAR_MAX_HP 3

const char textSpeedStr[] = "\001\001";
const char fontSmallStr[] = "\002\005";
const char invMainStartPosStr[] = "\003\013\002";

typedef struct item_t {
    unsigned char name[32];
    unsigned char nameShort[10];
    unsigned char nameSerious[10];
    uint8_t       useType; //1 = Heal, 2 = Weapon, 3 = Armor
    uint8_t       amount;
    unsigned char amountStr[5];

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
    .useMain = "*You re-applied\n|the bandage.\n*Still kind of gooey."
    },
    {
    .name = "Monster Candy",
    .nameShort = "MnstrCndy",
    .useType = 1,
    .amount = 10,
    .amountStr = "10",
    .desc = "*Has a distinct,\n|non-licorice\n|flavor.",
    .useMain = "*You ate the\n|Monster Candy.",
    .useMain2 = "*Very\n|un-licorice-like."
    },
    {
    .name = "Spider Donut",
    .nameShort = "SpidrDont",
    .useType = 1,
    .amount = 12,
    .amountStr = "12",
    .desc = "*A donut made with\n|Spider Cider in\n|the batter.",
    .useMain = "*You ate the\n|Spider Donut."
    },
    {
    .name = "Spider Cider",
    .nameShort = "SpidrCidr",
    .useType = 1,
    .amount = 24,
    .amountStr = "24",
    .desc = "*Made with whole\n|spiders, not just\n|the juice.",
    .useMain = "*You drank the\n|Spider Cider."
    },
    {
    .name = "Butterscotch Pie",
    .nameShort = "ButtsPie",
    .useType = 1,
    .amount = 127,
    .amountStr = "100%",
    .desc = "Butterscotch-\n|cinnamon pie,\n|one slice.",
    .useMain = "*You ate the\n|Butterscotch Pie."
    },
    {
    .name = "Stick",
    .nameShort = "Stick",
    .useType = 0,
    .amount = 0,
    .desc = "*Its bark is\n|worse than its\n|bite.",
    .useMain = "*You threw the\n|stick away."
    },
    {
    .name = "Toy Knife",
    .nameShort = "Toy Knife",
    .useType = 2,
    .amount = 3,
    .amountStr = "3",
    .desc = "*Made of plastic.\n*A rarity nowadays.",
    .useMain = "*You equipped the\n|Toy Knife."
    },
    {
    .name = "Faded Ribbon",
    .nameShort = "Ribbon",
    .useType = 3,
    .amount = 3,
    .amountStr = "3",
    .desc = "*If you're cuter,\n|monsters won't hit\n|you as hard.",
    .useMain = "*You equipped the\n|Faded Ribbon."
    },
};

const char maxHPStr[] = "*Your HP was\n|maxed out!";

//Unused
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

void inv_load_pause_menu(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;

    unsigned char * d = ui_text_data;
    *d = 0;

    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);

    strcat(d, textSpeedStr);
    strcat(d, fontSmallStr);
    strcat(d, invMainStartPosStr);
    for(uint8_t i = 0; i < 8; i++){

        strcat(d, items[invPtr[i]].name);
        strcat(d, "\n");
    }
}

void inv_remove_item(SCRIPT_CTX * THIS) OLDCALL BANKED {
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    uint8_t rSlot = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);

    for (uint8_t i = rSlot; i<7; i++){
        invPtr[i] = invPtr[i + 1];
    }
    invPtr[7] = 0;
}

void inv_use_item(SCRIPT_CTX * THIS) OLDCALL BANKED {

    unsigned char * d = ui_text_data;
    uint8_t * invPtr = (uint8_t *)VM_REF_TO_PTR(InvMainPtr);
    int16_t * currentHP = (int16_t*)VM_REF_TO_PTR(VAR_CURRENT_HP);
    int16_t * maxHP = (int16_t*)VM_REF_TO_PTR(VAR_MAX_HP);
    uint8_t slot = *(int16_t*)VM_REF_TO_PTR(FN_ARG0);
    uint8_t item = invPtr[slot];

    if(item == 0) return;

    *d = 0;
    strcat(d, "\003\02\11");
    strcat(d, items[item].useMain);

    vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1);
    vm_display_text(THIS, 0, 68);
    vm_overlay_wait(THIS, 1, 6);

    if(items[item].useMain2[0] != 0){
        *d = 0;
        strcat(d, "\003\02\11");
        strcat(d, items[item].useMain2);

        vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1);
        vm_display_text(THIS, 0, 68);
        vm_overlay_wait(THIS, 1, 6);
    }
    *d = 0;

    if(items[item].useType == 1){//If item is consumable:
        strcat(d, "\003\02\11");
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
        vm_overlay_clear(THIS, 0, 7, 19, 5, 0, 1);
        vm_display_text(THIS, 0, 68);
        vm_overlay_wait(THIS, 1, 6);

        inv_remove_item(THIS);
    }



}

void inv_use_item_2(SCRIPT_CTX * THIS) OLDCALL BANKED {
    THIS;

}

//Struct in TestEngineMod.h
/*

typedef struct item_t {
    unsigned char name[23];
    unsigned char nameShort[10];
    unsigned char desc[64];
} item_t;

*/
