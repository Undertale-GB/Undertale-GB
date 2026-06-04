#ifndef UTGB_TYPES_H
#define UTGB_TYPES_H

#include "gbs_types.h"


typedef struct battledata_t {
    
    unsigned char* introText;
    unsigned char* testTxt;

} battledata_t;

typedef struct battle_t
{
    unsigned char* introText;
    UBYTE enemyCount;
    far_ptr_t enemies[2];
    unsigned char* randomDialogue[];

} battle_t;

typedef struct enemy_t
{
    unsigned char* name;
    uint16_t health;
    uint16_t attack;
    uint16_t defense;

    act_t acts[];

} enemy_t;

typedef struct act_t {

    unsigned char* title;
    UBYTE actVar;
    UBYTE minActVarValue;
    UBYTE actVarInc;
    unsigned char* test;

} act_t;

#endif