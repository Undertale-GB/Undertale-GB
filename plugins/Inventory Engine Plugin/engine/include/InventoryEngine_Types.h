#ifndef INVENTORY_ENGINE_TYPES_H
#define INVENTORY_ENGINE_TYPES_H

#include "gbs_types.h"


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



#endif