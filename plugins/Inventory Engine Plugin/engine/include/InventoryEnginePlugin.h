#ifndef INVENTORY_ENGINE_PLUGIN
#define INVENTORY_ENGINE_PLUGIN

#include <gb/gb.h>
#include "gbs_types.h"

/*
Concatanates the Item name to the end of the loaded text
*/
void inv_load_item_name(SCRIPT_CTX * THIS, uint8_t InvSlot, uint8_t InvType) OLDCALL BANKED;

/*
Loads the text string for the list of items
Stack: None
*/
void inv_load_pause_menu(SCRIPT_CTX * THIS) OLDCALL BANKED;

/*
Adds item to inventory, no dialogue
Stack: 
- 0:ItemID
(Sets top of stack to 0 if no space is availible) 
*/
void inv_add_item(SCRIPT_CTX * THIS) OLDCALL BANKED;

/*
Adds item to inventory, with dialogue
Stack: 
- 0:ItemID
(Sets top of stack to 0 if no space is availible)
Use during dialogue
*/
void inv_obtain_item(SCRIPT_CTX * THIS) OLDCALL BANKED;

/*
Removes Item from specified Slot
Stack: 
- 0:Inventory Slot
*/
void inv_remove_item(SCRIPT_CTX * THIS) OLDCALL BANKED;
void inv_remove_item_new(SCRIPT_CTX * THIS, uint8_t invSlot) OLDCALL BANKED;

/*
Loads item use text into string
*/
bool inv_load_use_main_text(SCRIPT_CTX * THIS, UBYTE * string, uint8_t invSlot, uint8_t textNum) OLDCALL BANKED;

/*
Uses an Item in the Inventory
Stack: 
- 0: Inventory Slot 
- 1: Bool inBattle
*/
void inv_use_item(SCRIPT_CTX * THIS) OLDCALL BANKED;
bool inv_use_item_new(SCRIPT_CTX * THIS, UBYTE * string, uint8_t invSlot) OLDCALL BANKED;

/*
Drops an Item, with dialogue
Stack: 
- 0:Inventory Slot
*/
void inv_drop_item(SCRIPT_CTX * THIS) OLDCALL BANKED;

/*
Writes the Item's description
Only used when using "INFO"
Stack: 
- 0:Inventory Slot
*/
void inv_write_item_desc(SCRIPT_CTX * THIS) OLDCALL BANKED;


#endif