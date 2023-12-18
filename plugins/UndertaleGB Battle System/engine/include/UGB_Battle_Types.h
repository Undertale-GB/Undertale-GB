#ifndef UGB_BATTLE_TYPES_H
#define UGB_BATTLE_TYPES_H

#include "gbs_types.h"


typedef enum {
    UGB_TURN_PLAYER = 0,
    UGB_TURN_ENEMY
}ugb_battle_state_e;


typedef enum {
    UGB_WAIT = 0,
    UGB_FIRE_PROJECTILE,
    UGB_MOVE_ACTOR_TO,
    UGB_FIRE_PROJECTILE_FROM_ACTOR

}ugb_pattern_command_e;


typedef struct ugb_battle_t {
    uint8_t n_actors, n_projectiles, n_sprites;
    far_ptr_t background_enemy, background_ui;
    far_ptr_t actors, sprites;
    far_ptr_t patterns[];
}ugb_battle;


typedef struct ugb_pattern_t {
    far_ptr_t projectiles;
    uint8_t bbox_width, bbox_height;
    uint8_t time;
}ugb_patterns;


typedef struct ugb_pattern_command_t {
    ugb_pattern_command_e command;
    uint8_t parameters[];

}ugb_pattern_command_t;



#endif