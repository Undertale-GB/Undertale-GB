#ifndef UTGB_BATTLE_DRAWING_H
#define UTGB_BATTLE_DRAWING_H

#include "data_manager.h"
//#include "gb/gb.h"
#include "ui.h"
// from ui.c:


void utgb_draw_battle_border(uint8_t x, uint8_t y, uint8_t width, uint8_t height) BANKED;

void utgb_draw_battle_attackbg(void) BANKED;

void vram_buffer_draw_rect(uint8_t* buffer, uint8_t startX, uint8_t endX, uint8_t startY, uint8_t endY, uint8_t colorBlack) BANKED;

#endif