#ifndef UTGB_VARIABLES_H
#define UTGB_VARIABLES_H


//Get value of global Int16 variable
#define VAR_VAL(ID) (*(INT16 *)VM_REF_TO_PTR(ID))

//Get pointer to global variable (int8/16)
#define VAR_PTR_8(ID) ((INT8 *)VM_REF_TO_PTR(ID))
#define VAR_PTR_16(ID) ((INT16 *)VM_REF_TO_PTR(ID))


//Global Variables:
#define VAR_PROGRESS 0
#define VAR_GAMESTATE 1
#define VAR_CURRENT_HP 2
#define VAR_MAX_HP 3

#define VAR_GOLD_MONEY_ 16
#define VAR_LEVEL 17
#define VAR_NAME1 19

#define VAR_ATTACK_ITEM_ 26
#define VAR_DEFENSE_ITEM_ 27
#define VAR_EXP 28
#define VAR_EXP_TO_NEXT_LEVEL 29
#define VAR_ATTACK_BASE_ 30
#define VAR_DEFENSE_BASE_ 31

#define InvMainPtr 34

#define EquippedItemsPtr 41


#endif