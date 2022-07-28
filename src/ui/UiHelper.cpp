
#include "Ui.h"
#include "UiHelper.h"

lv_obj_t* uiCreatePanel(lv_obj_t* parent, const lv_font_t* font, lv_coord_t x, lv_coord_t y, lv_coord_t w) {
    lv_obj_t* uiPanel = lv_obj_create(parent);
    lv_obj_set_size(uiPanel, w, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiPanel, x, y);
    lv_obj_clear_flag(uiPanel, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(uiPanel, lv_color_hex(UI_COLOR_PANEL_BG), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(uiPanel, lv_color_hex(UI_COLOR_PANEL_BG_GRADIENT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(uiPanel, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(uiPanel, lv_color_hex(UI_COLOR_PANEL_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(uiPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(uiPanel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(uiPanel, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_left(uiPanel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(uiPanel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(uiPanel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(uiPanel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(uiPanel, font, LV_PART_MAIN | LV_STATE_DEFAULT);

    return uiPanel;
}

lv_obj_t* uiCreateButton(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* text, const lv_font_t* font, lv_event_cb_t eventCb, void *userData, lv_event_code_t filter) {

    lv_obj_t* uiButton = lv_btn_create(parent);

    lv_obj_set_size(uiButton, w, h);
    lv_obj_set_pos(uiButton, x, y);
    lv_obj_set_align(uiButton, LV_ALIGN_TOP_RIGHT);

    lv_obj_set_style_bg_color(uiButton, lv_color_hex(UI_COLOR_BUTTON_BG), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(uiButton, lv_color_hex(UI_COLOR_BUTTON_SHADOW), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiButton, lv_color_hex(UI_COLOR_BUTTON_BG_PRESSED), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(uiButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(uiButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(uiButton, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(uiButton, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (eventCb) {
        lv_obj_add_event_cb(uiButton, eventCb, filter, userData);
    }

    // uiButtonLabel
    lv_obj_t* uiButtonLabel = lv_label_create(uiButton);

    lv_obj_set_size(uiButtonLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiButtonLabel, 0, 0);
    lv_obj_set_align(uiButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(uiButtonLabel, text);
    
    //lv_obj_set_style_text_decor(uiButtonLabel, LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(uiButtonLabel, font, LV_PART_MAIN | LV_STATE_DEFAULT);

    return uiButton;
}

void uiUpdateLabel(lv_obj_t* l, const char* text, int32_t color, int32_t bgColor) {
    lv_label_set_text(l, text);
    if (color != -1) { lv_obj_set_style_text_color(l, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT); }
    if (bgColor != -1) { lv_obj_set_style_bg_color(l, lv_color_hex(bgColor), LV_PART_MAIN | LV_STATE_DEFAULT); }

}