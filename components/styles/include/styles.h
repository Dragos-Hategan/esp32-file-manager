#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#define UI_COLOR_BG_DARK             lv_color_hex(0x101214)
#define UI_COLOR_CARD_DARK           lv_color_hex(0x202327)
#define UI_COLOR_BORDER_DARK         lv_color_hex(0x2D3034)
#define UI_COLOR_BUTTON_BORDER_DARK  lv_color_hex(0xBBAAFF)
#define UI_COLOR_INDICATOR_OFF_DARK  lv_color_hex(0x00055F)
#define UI_COLOR_TEXT_DARK           lv_color_hex(0xDCDCDC)
#define UI_COLOR_ACCENT_BLUE_DARK    lv_color_hex(0x7D5FFF)
#define UI_COLOR_ACCENT_BLUE_DARK_2  lv_color_hex(0x347AFF)
#define UI_COLOR_ACCENT_GREEN_DARK   lv_color_hex(0x37B24D)



void styles_build_button(lv_obj_t *button);
void styles_build_switch(lv_obj_t *switch_button);
void styles_build_textarea(lv_obj_t *textarea);
void styles_build_dropdown(lv_obj_t *dropdown);
void styles_build_msgbox(lv_obj_t *mbox);
void styles_build_keyboard(lv_obj_t *kbd);


#ifdef __cplusplus
}
#endif