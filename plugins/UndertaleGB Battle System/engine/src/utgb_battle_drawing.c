// must be in the same bank with ui_a.s
#pragma bank 1

#include "data_manager.h"
#include "gb/gb.h"
//#include "string.h"
#include "ui.h"
#include "scroll.h"
#include "system.h"

//included from ui.c:
void ui_draw_frame_row(void * dest, UBYTE tile, UBYTE width) OLDCALL;

#include "utgb_battle_drawing.h"

//custom data files:
//#include "data/utgb_battle_attack_bg_tilemap.h"

const unsigned char utgb_battle_attack_bg_tilemap[] = {
    0x20, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x24, 0x27, 0x28, 0x29, 0x20, 0x20, 0x2A, 0x2B,
    0x2C, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x38,
    0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x3F, 0x40, 0x41, 0x20, 0x20, 0x42, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x45, 0x48, 0x49, 0x4A, 0x20, 0x20
};

#define ui_frame_tl_tiles 0xC0u
#define ui_frame_bl_tiles 0xC6u
#define ui_frame_tr_tiles 0xC2u
#define ui_frame_br_tiles 0xC8u
#define ui_frame_t_tiles  0xC1u
#define ui_frame_b_tiles  0xC7u
#define ui_frame_l_tiles  0xC3u
#define ui_frame_r_tiles  0xC5u
#define ui_frame_bg_tiles 0xC4u




void utgb_draw_battle_attackbg(void) BANKED {

    set_bkg_tiles(3, 10, 14, 4, utgb_battle_attack_bg_tilemap);

}

void vram_buffer_draw_rect(uint8_t* buffer, uint8_t startX, uint8_t endX, uint8_t startY, uint8_t endY, uint8_t colorBlack) BANKED {

    uint8_t* tile_data_temp = buffer;

    uint8_t row = 0;

    for (uint8_t x = startX; x <= endX; x++)
    {
        row = row | (1 << (7-x));
    }

    //get_bkg_data(tile, 1, vwf_tile_data);

    tile_data_temp += startY<<1;

    if(colorBlack) {

        for (uint8_t y = startY; y <= endY; y++)
        {
            
            //uint8_t row = ~(1 << startX);
            *tile_data_temp = *tile_data_temp | row;
            tile_data_temp++;
            *tile_data_temp = *tile_data_temp | row;
            tile_data_temp++;
            //tile_data_temp[(y<<1)+1] = tile_data_temp[(y<<1)+1] | row;

        }

    } else {

        row = ~row;

        for (uint8_t y = startY; y <= endY; y++)
        {
            //uint8_t row = ~(1 << startX);
            *tile_data_temp = *tile_data_temp & row;
            tile_data_temp++;
            *tile_data_temp = *tile_data_temp & row;
            tile_data_temp++;
        }
    }

    
    //set_bkg_data(tile, 1, vwf_tile_data);
}

void utgb_draw_frame_bkg(UBYTE x, UBYTE y, UBYTE width, UBYTE height) BANKED {
    if (height == 0) return;
#ifdef CGB
    if (_is_CGB) {
        //VBK_REG = 1;
        //fill_bkg_rect(x, y, width, height, overlay_priority | (text_palette & 0x07u));
        //VBK_REG = 0;
    }
#endif
    UBYTE * base_addr = GetBkgAddr() + (y << 5) + x;
    ui_draw_frame_row(base_addr, ui_frame_tl_tiles, width);
    if (--height == 0) return;
    if (height > 1)
        for (UBYTE i = height - 1; i != 0; i--) {
            base_addr += 32;
            ui_draw_frame_row(base_addr, ui_frame_l_tiles, width);
        }
    base_addr += 32;
    ui_draw_frame_row(base_addr, ui_frame_bl_tiles, width);
}


void utgb_draw_battle_border(uint8_t x, uint8_t y, uint8_t width, uint8_t height) BANKED {
    
    uint8_t tileX = x >> 3;
    uint8_t tileY = y >> 3;
    uint8_t tileWidth = ((width) >> 3) - tileX + 1;
    uint8_t tileHeight = ((height) >> 3) - tileY + 1;

    utgb_draw_frame_bkg(tileX, tileY, tileWidth, tileHeight);

    //top-left tile
    get_bkg_data(ui_frame_tl_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, x%8, x%8, y%8, 7, 0);
    vram_buffer_draw_rect(vwf_tile_data, x%8, 7, y%8, y%8, 0);
    set_bkg_data(ui_frame_tl_tiles, 1, vwf_tile_data);
    //top tile
    get_bkg_data(ui_frame_t_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, y%8, y%8, 0);
    set_bkg_data(ui_frame_t_tiles, 1, vwf_tile_data);
    //top-right tile
    get_bkg_data(ui_frame_tr_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, width%8, width%8, y%8, 7, 0);
    vram_buffer_draw_rect(vwf_tile_data, 0, width%8, y%8, y%8, 0);
    set_bkg_data(ui_frame_tr_tiles, 1, vwf_tile_data);
    //left tile
    get_bkg_data(ui_frame_l_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, x%8, x%8, 0, 7, 0);
    set_bkg_data(ui_frame_l_tiles, 1, vwf_tile_data);
    //right tile
    get_bkg_data(ui_frame_r_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, width%8, width%8, 0, 7, 0);
    set_bkg_data(ui_frame_r_tiles, 1, vwf_tile_data);
    //bottom-left tile
    get_bkg_data(ui_frame_bl_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, x%8, x%8, 0, height%8, 0);
    vram_buffer_draw_rect(vwf_tile_data, x%8, 7, height%8, height%8, 0);
    set_bkg_data(ui_frame_bl_tiles, 1, vwf_tile_data);
    //bottom tile
    get_bkg_data(ui_frame_b_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, height%8, height%8, 0);
    set_bkg_data(ui_frame_b_tiles, 1, vwf_tile_data);
    //bottom-right tile
    get_bkg_data(ui_frame_br_tiles, 1, vwf_tile_data);
    vram_buffer_draw_rect(vwf_tile_data, 0, 7, 0, 7, 1);
    vram_buffer_draw_rect(vwf_tile_data, width%8, width%8, 0, height%8, 0);
    vram_buffer_draw_rect(vwf_tile_data, 0, width%8, height%8, height%8, 0);
    set_bkg_data(ui_frame_br_tiles, 1, vwf_tile_data);
    
    
}
