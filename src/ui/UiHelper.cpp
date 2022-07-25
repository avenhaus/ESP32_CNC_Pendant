
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