#include "styles.h"

static inline lv_style_selector_t style_sel(lv_part_t part, lv_state_t state)
{
    return (lv_style_selector_t)((lv_style_selector_t)part | (lv_style_selector_t)state);
}

void styles_build_button(lv_obj_t *button)
{
    if (!button) {
        return;
    }
    lv_obj_set_style_bg_color(button, UI_COLOR_ACCENT_BLUE_DARK, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(button, UI_COLOR_BUTTON_BORDER_DARK, LV_PART_MAIN);
    lv_obj_set_style_border_width(button, 1, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(button, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(button, UI_COLOR_TEXT_DARK, LV_PART_MAIN);
    lv_obj_set_style_text_color(button, UI_COLOR_TEXT_DARK, LV_PART_MAIN);
}

void styles_build_dropdown(lv_obj_t *dropdown)
{
    if (dropdown) {
        lv_obj_set_style_bg_color(dropdown, UI_COLOR_CARD_DARK, LV_PART_MAIN);
        lv_obj_set_style_bg_color(dropdown, UI_COLOR_BORDER_DARK, LV_PART_SCROLLBAR);

        lv_obj_set_style_bg_opa(dropdown, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_text_color(dropdown, UI_COLOR_TEXT_DARK, 0);

        /* Subtle selection: keep base bg, just a 1px accent border on selected item */
        lv_obj_set_style_bg_opa(dropdown, LV_OPA_TRANSP, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED));
        lv_obj_set_style_bg_opa(dropdown, LV_OPA_TRANSP, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED | LV_STATE_PRESSED));
        lv_obj_set_style_border_color(dropdown, UI_COLOR_BUTTON_BORDER_DARK, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED));
        lv_obj_set_style_border_color(dropdown, UI_COLOR_BUTTON_BORDER_DARK, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED | LV_STATE_PRESSED));
        lv_obj_set_style_border_width(dropdown, 1, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED));
        lv_obj_set_style_border_width(dropdown, 1, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED | LV_STATE_PRESSED));
        lv_obj_set_style_text_color(dropdown, UI_COLOR_TEXT_DARK, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED));
        lv_obj_set_style_text_color(dropdown, UI_COLOR_TEXT_DARK, style_sel(LV_PART_SELECTED, LV_STATE_CHECKED | LV_STATE_PRESSED));

        lv_obj_set_style_border_color(dropdown, UI_COLOR_BORDER_DARK, LV_PART_SCROLLBAR);
        lv_obj_set_style_border_color(dropdown, UI_COLOR_BORDER_DARK, LV_PART_MAIN);

        lv_obj_set_style_border_width(dropdown, 1, LV_PART_MAIN);
    }
}

void styles_build_switch(lv_obj_t *switch_button)
{
    if (!switch_button) {
        return;
    }
    lv_obj_set_style_bg_color(switch_button, UI_COLOR_CARD_DARK, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(switch_button, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(switch_button, UI_COLOR_ACCENT_BLUE_DARK, LV_PART_KNOB);
    lv_obj_set_style_border_color(switch_button, UI_COLOR_BUTTON_BORDER_DARK, LV_PART_KNOB);
    lv_obj_set_style_bg_color(switch_button, UI_COLOR_INDICATOR_OFF_DARK, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(switch_button, 2, LV_PART_KNOB);  
}

void styles_build_textarea(lv_obj_t *textarea)
{
    if (!textarea) {
        return;
    }    
    lv_obj_set_style_bg_color(textarea, lv_color_lighten(UI_COLOR_CARD_DARK, 50), 0);
    lv_obj_set_style_bg_opa(textarea, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(textarea, UI_COLOR_BORDER_DARK, 0);
    lv_obj_set_style_border_width(textarea, 1, 0);
    lv_obj_set_style_text_color(textarea, UI_COLOR_TEXT_DARK, 0);
}

void styles_build_msgbox(lv_obj_t *mbox)
{
    if (!mbox) {
        return;
    }
    lv_obj_set_style_bg_color(mbox, UI_COLOR_CARD_DARK, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(mbox, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(mbox, UI_COLOR_BORDER_DARK, LV_PART_MAIN);
    lv_obj_set_style_border_width(mbox, 1, LV_PART_MAIN);
    lv_obj_set_style_text_color(mbox, UI_COLOR_TEXT_DARK, LV_PART_MAIN);
    lv_obj_set_style_text_color(mbox, UI_COLOR_TEXT_DARK, LV_PART_ITEMS);
}

void styles_build_keyboard(lv_obj_t *kbd)
{
    if (!kbd) {
        return;
    }
    lv_obj_set_style_bg_color(kbd, UI_COLOR_CARD_DARK, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(kbd, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(kbd, UI_COLOR_BORDER_DARK, LV_PART_MAIN);
    lv_obj_set_style_border_width(kbd, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(kbd, 6, LV_PART_MAIN);
    lv_obj_set_style_text_color(kbd, UI_COLOR_TEXT_DARK, LV_PART_MAIN);

    /* Keys */
    lv_obj_set_style_bg_color(kbd, UI_COLOR_CARD_DARK, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(kbd, LV_OPA_COVER, LV_PART_ITEMS);
    lv_obj_set_style_border_color(kbd, UI_COLOR_BORDER_DARK, LV_PART_ITEMS);
    lv_obj_set_style_border_width(kbd, 1, LV_PART_ITEMS);
    lv_obj_set_style_text_color(kbd, UI_COLOR_TEXT_DARK, LV_PART_ITEMS);

    /* Pressed/checked/focused keys: subtle dark instead of accent */
    lv_obj_set_style_bg_color(kbd, UI_COLOR_BORDER_DARK, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(kbd, UI_COLOR_BORDER_DARK, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(kbd, UI_COLOR_BORDER_DARK, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(kbd, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(kbd, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(kbd, LV_OPA_COVER, LV_PART_ITEMS | LV_STATE_FOCUSED);
    lv_obj_set_style_text_color(kbd, UI_COLOR_TEXT_DARK, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(kbd, UI_COLOR_TEXT_DARK, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(kbd, UI_COLOR_TEXT_DARK, LV_PART_ITEMS | LV_STATE_FOCUSED);
}
