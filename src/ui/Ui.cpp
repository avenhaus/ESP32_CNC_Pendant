#include "Ui.h"

/* ============================================== *\
 * Variables
\* ============================================== */
lv_obj_t * uiMainScreen;
lv_obj_t * uiCncStateLabel;
lv_obj_t * uiCncPinLabel;

PROGMEM const char* UI_AXIS_PREC = "%0.3f";

/* ============================================== *\
 * Functions
\* ============================================== */

void uiMainScreenInit(void) {

    // uiMainScreen
    uiMainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(uiMainScreen, LV_OBJ_FLAG_SCROLLABLE);
    uiCreateStatusBar(uiMainScreen);
    uiCreatePanelWAxis(uiMainScreen, 5, 29);
    uiCreatePanelMAxis(uiMainScreen, 260, 29);
    uiCreatePanelSettings(uiMainScreen, 5, 183);
    uiCreateKeyboard(uiMainScreen);

    uiCncStateLabel = lv_label_create(uiMainScreen);
    lv_obj_set_size(uiCncStateLabel, 140, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiCncStateLabel, 0, 0);
    lv_obj_set_align(uiCncStateLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(uiCncStateLabel, FST("???"));
    lv_obj_set_style_text_align(uiCncStateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiCncStateLabel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiCncStateLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiCncStateLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiCncStateLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(uiCncStateLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_flag(uiCncStateLabel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(uiCncStateLabel, lv_color_hex(0x505050), LV_PART_MAIN | LV_STATE_PRESSED);

    uiCncPinLabel = lv_label_create(uiMainScreen);
    lv_obj_set_size(uiCncPinLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiCncPinLabel, 0, 0);
    lv_obj_set_align(uiCncPinLabel, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(uiCncPinLabel, FST("-"));
    lv_obj_set_style_text_align(uiCncPinLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiCncPinLabel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiCncPinLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiCncPinLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiCncPinLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(uiCncPinLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void uiInit(void) {
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    uiMainScreenInit();
    lv_disp_load_scr(uiMainScreen);
}

