#pragma bank 255

#include "data/states_defines.h"
#include "states/utgb_battle.h"

#include "actor.h"
#include "camera.h"
#include "data_manager.h"
#include "game_time.h"
#include "input.h"
#include "trigger.h"
#include "vm.h"

//debug only!
#include "data/sprite_toriel.h"

UBYTE bbox_left, bbox_right;
UBYTE bbox_up, bbox_down;
uint16_t attackTimer;



const metasprite_t var_metasprite_end = {metasprite_end};

metasprite_t attack_metasprite[10][10];
UBYTE num_attack_metasprites;


inline void add_attack_metasprite(metasprite_t* sprite, UBYTE spriteNum) {
    UBYTE i;
    for(i = 0; i < spriteNum; i++) {
        attack_metasprite[num_attack_metasprites][i] = sprite[i];
    }
    attack_metasprite[num_attack_metasprites][i] = var_metasprite_end;
    
    num_attack_metasprites++;
}




inline UBYTE load_sprite_tileset(UBYTE base_tile, const tileset_t * tileset, UBYTE bank) {
    UBYTE n_tiles = ReadBankedUBYTE(&(tileset->n_tiles), bank);
    if (n_tiles) SetBankedSpriteData(base_tile, n_tiles, tileset->tiles, bank);
    return n_tiles;
}
/*
//loads metasprite from banked code
//Debug for now, might clean up later
uint8_t ugb_load_banked_metasprite(far_ptr_t spritesheet_ptr, UINT8 base_tile, UINT8 base_sprite, UINT8 x, UINT8 y) NONBANKED {
    
    UBYTE _save = CURRENT_BANK;
    SWITCH_ROM(spritesheet_ptr.bank);

    spritesheet_t* current_spritesheet = spritesheet_ptr.ptr;
    metasprite_t* current_metasprite = current_spritesheet->metasprites[0];
    
    UBYTE metasprite_size = move_metasprite(
        current_metasprite,
        base_tile,
        base_sprite,
        x,
        y
    );

    SWITCH_ROM(_save);

    return metasprite_size;
}
*/

// run late in main game loop
// called in core.c during shadow_oam
void utgb_draw_attack(void) BANKED {

    for (UBYTE i = 0; i < num_attack_metasprites; i++) {

        allocated_hardware_sprites += move_metasprite(
            attack_metasprite[i],
            0,
            allocated_hardware_sprites,
            0,
            0
        );
    }
}


void utgb_battle_init(void) BANKED {
    camera_offset_x = 0;
    camera_offset_y = 0;
    camera_deadzone_x = 0;
    camera_deadzone_y = 0;
    //PLAYER.dir = DIR_RIGHT;
    //actor_set_anim(&PLAYER, ANIM_CURSOR);

    //========================== Debug Start ==========================//
    bbox_left = 37;
    bbox_right = 122;
    bbox_up = 77;
    bbox_down = 114;

    attackTimer = 0;
    num_attack_metasprites = 0;

    //load sprites
    far_ptr_t spritesheet_FarPtr = TO_FAR_PTR_T(sprite_toriel);

    //ReadBankedFarPtr(&spritesheet_ptr, sprite.ptr, sprite.bank); // FarPtr to Spritesheet data 

    spritesheet_t* spritesheetPtr = spritesheet_FarPtr.ptr;

    far_ptr_t data;
    ReadBankedFarPtr(&data, (void *)&spritesheetPtr->tileset, spritesheet_FarPtr.bank);
    
    VBK_REG = 1;
    UBYTE n_tiles = load_sprite_tileset(0, data.ptr, data.bank);
    VBK_REG = 0;


    //========================== Debug End ==========================//
}

void UTGB_battle_data_init(SCRIPT_CTX * THIS) BANKED {
    
    // TODO: Add battle loading code
}


void utgb_battle_update(void) BANKED {


    //========================== Debug Start ==========================//

    /*

    //load sprites
    far_ptr_t spritesheet_FarPtr = TO_FAR_PTR_T(sprite_toriel);

    //ReadBankedFarPtr(&spritesheet_ptr, sprite.ptr, sprite.bank); // FarPtr to Spritesheet data 

    spritesheet_t* spritesheetPtr = spritesheet_FarPtr.ptr;
    
    //draw sprites
    allocated_hardware_sprites += ugb_load_banked_metasprite(
        spritesheet_FarPtr,
        0,
        allocated_hardware_sprites,
        64,
        64
    );

    */


    attackTimer++;
    num_attack_metasprites = 1;

    for (int8_t i = 0; i < 6; i++) {

        metasprite_t tmp = { .dy = 77 + 16, .dx = 37 + 8 - 2 + ((i << 4) + attackTimer) % (64 + 16), .dtile = 16, .props = 11 };
        if(tmp.dx < (122 + 6)) {
            add_attack_metasprite(&tmp, 1);
        }
        
    }


    //========================== Debug End ==========================//

    player_moving = FALSE;

    UBYTE angle;
    
    // Handle input
    if (INPUT_LEFT) {
        player_moving = TRUE;
        if (INPUT_UP) {
            angle = ANGLE_315DEG;
        } else if (INPUT_DOWN) {
            angle = ANGLE_225DEG;
        } else {
            angle = ANGLE_270DEG;
        }
    } else if (INPUT_RIGHT) {
        player_moving = TRUE;
        if (INPUT_UP) {
            angle = ANGLE_45DEG;
        } else if (INPUT_DOWN) {
            angle = ANGLE_135DEG;
        } else {
            angle = ANGLE_90DEG;
        }
    } else if (INPUT_UP) {
        player_moving = TRUE;
        angle = ANGLE_0DEG;
    } else if (INPUT_DOWN) {
        player_moving = TRUE;
        angle = ANGLE_180DEG;
    } else {
        angle = ANGLE_0DEG;
    }

    /*
    if (player_moving) {
        point_translate_angle(&(PLAYER.pos), angle, PLAYER.move_speed);
        // Clamp X
        if ((PLAYER.pos.x >> 4) + PLAYER.bounds.left < bbox_left) {
            PLAYER.pos.x = (-PLAYER.bounds.left + bbox_left) << 4;
        } else if ((PLAYER.pos.x >> 4) + PLAYER.bounds.right > bbox_right) {
            PLAYER.pos.x = (bbox_right - PLAYER.bounds.right) << 4;
        }
        // Clamp Y
        if ((PLAYER.pos.y >> 4) + PLAYER.bounds.top < bbox_up) {
            PLAYER.pos.y = -(PLAYER.bounds.top << 4) + (bbox_up << 4);
        } else if ((PLAYER.pos.y >> 4) + PLAYER.bounds.bottom > bbox_down) {
            PLAYER.pos.y = (bbox_down - PLAYER.bounds.bottom) << 4;
        }    
               
    }
    */

    if (player_moving) {
        point_translate_angle(&(PLAYER.pos), angle, PLAYER.move_speed);
        // Clamp X
        if ((PLAYER.pos.x >> 4) < bbox_left) {
            PLAYER.pos.x = (bbox_left) << 4;
        } else if ((PLAYER.pos.x >> 4) + 7 > bbox_right) {
            PLAYER.pos.x = (bbox_right - 7) << 4;
        }
        // Clamp Y
        if ((PLAYER.pos.y >> 4) < bbox_up) {
            PLAYER.pos.y = (bbox_up << 4);
        } else if ((PLAYER.pos.y >> 4) + 7 > bbox_down) {
            PLAYER.pos.y = (bbox_down - 7) << 4;
        }    
               
    }



}










/*

#include "actor.h"
#include "camera.h"
#include "data_manager.h"
#include "game_time.h"
#include "input.h"
#include "trigger.h"
#include "vm.h"

#ifndef POINT_N_CLICK_CAMERA_DEADZONE
#define POINT_N_CLICK_CAMERA_DEADZONE 24
#endif

UBYTE last_hit_trigger = MAX_TRIGGERS;

void pointnclick_init() BANKED {
    camera_offset_x = 0;
    camera_offset_y = 0;
    camera_deadzone_x = POINT_N_CLICK_CAMERA_DEADZONE;
    camera_deadzone_y = POINT_N_CLICK_CAMERA_DEADZONE;
    PLAYER.dir = DIR_RIGHT;
    actor_set_anim(&PLAYER, ANIM_CURSOR);
}

void pointnclick_update() BANKED {
    UBYTE angle, hit_trigger, is_hover_trigger, is_hover_actor;
    actor_t *hit_actor;

    player_moving = FALSE;

    // Handle input
    if (INPUT_LEFT) {
        player_moving = TRUE;
        if (INPUT_UP) {
            angle = ANGLE_315DEG;
        } else if (INPUT_DOWN) {
            angle = ANGLE_225DEG;
        } else {
            angle = ANGLE_270DEG;
        }
    } else if (INPUT_RIGHT) {
        player_moving = TRUE;
        if (INPUT_UP) {
            angle = ANGLE_45DEG;
        } else if (INPUT_DOWN) {
            angle = ANGLE_135DEG;
        } else {
            angle = ANGLE_90DEG;
        }
    } else if (INPUT_UP) {
        player_moving = TRUE;
        angle = ANGLE_0DEG;
    } else if (INPUT_DOWN) {
        player_moving = TRUE;
        angle = ANGLE_180DEG;
    } else {
        angle = ANGLE_0DEG;
    }

    // Move cursor
    if (player_moving) {
        point_translate_angle(&(PLAYER.pos), angle, PLAYER.move_speed);
        // Clamp X
        if ((PLAYER.pos.x >> 4) - PLAYER.bounds.left > image_width) {
            PLAYER.pos.x = (PLAYER.bounds.left << 4);
        } else if ((PLAYER.pos.x >> 4) + PLAYER.bounds.right > image_width) {
            PLAYER.pos.x = (image_width - PLAYER.bounds.right) << 4;
        }
        // Clamp Y
        if ((PLAYER.pos.y >> 4) + PLAYER.bounds.top > image_height) {
            PLAYER.pos.y = -(PLAYER.bounds.top << 4);
        } else if ((PLAYER.pos.y >> 4) + PLAYER.bounds.bottom > image_height) {
            PLAYER.pos.y = (image_height - PLAYER.bounds.bottom) << 4;
        }             
    }

    // Check for trigger collisions
    hit_trigger = trigger_at_intersection(&PLAYER.bounds, &PLAYER.pos);

    // Check for actor collisions
    hit_actor = actor_overlapping_player(FALSE);

    is_hover_trigger = (hit_trigger != NO_TRIGGER_COLLISON)
        && (triggers[hit_trigger].script.bank);

    is_hover_actor = hit_actor && hit_actor->script.bank;

    // Set cursor animation
    if (is_hover_trigger || is_hover_actor) {
        actor_set_anim(&PLAYER, ANIM_CURSOR_HOVER);
    } else {
        actor_set_anim(&PLAYER, ANIM_CURSOR);
    }

    if (INPUT_A_PRESSED) {
        player_moving = FALSE;
        if (is_hover_actor) {
            // Run actor script
            script_execute(hit_actor->script.bank, hit_actor->script.ptr, 0, 1, 0);
        }
        else if (is_hover_trigger) {
            // Run trigger script
            trigger_interact(hit_trigger);
        }
    }
}


*/