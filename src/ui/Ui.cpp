#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */
lv_obj_t* uiMainScreen;
lv_obj_t* uiMessageToast;
lv_obj_t* uiErrorToast;
lv_obj_t* uiEmergencyStopToast;
lv_obj_t* uiInputLockToast;

static uint32_t _hideMessageToastTs = 0;
static uint32_t _hideErrorToastTs = 0;

PROGMEM const char* UI_AXIS_PREC = "%0.3f";


// 0xe4e4, 0xf043, 0xf2cc, 0xf192, 0xf740, 0xf5c7, 0xe005, 0xf084, 0xf0eb, 0xf601, 0xf023, 0xf3c1, 0xf303, 0xe55b, 0xf5bd, 0xf2ed, 0xf773, 0xf72e, 0xe4bb, 0xf293, 0xf1eb


/* ============================================== *\
 * Functions
\* ============================================== */

void uiMainScreenInit(void) {

    // uiMainScreen
    uiMainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(uiMainScreen, LV_OBJ_FLAG_SCROLLABLE);
    uiCreateStatusBar(uiMainScreen);
    uiCreatePanelWAxis(uiMainScreen, 5, 29);
    uiCreatePanelMAxis(uiMainScreen, 272, 29);
    uiCreatePanelSettings(uiMainScreen, 5, 183);
    uiCreateButtons(uiMainScreen, 338, 181);
    uiCreateKeyboard(uiMainScreen);

    uiInputLockToast = lv_label_create(uiMainScreen);
    lv_obj_set_size(uiInputLockToast, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiInputLockToast, 0, -6);
    lv_obj_set_align(uiInputLockToast, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(uiInputLockToast, FST("Input Lock"));
    lv_obj_set_style_text_align(uiInputLockToast, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiInputLockToast, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiInputLockToast, lv_color_hex(0xFFD020), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiInputLockToast, lv_color_hex(0x604000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiInputLockToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(uiInputLockToast, lv_color_hex(0xF0D020), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(uiInputLockToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(uiInputLockToast, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(uiInputLockToast, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_ver(uiInputLockToast, 4, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_pad_hor(uiInputLockToast, 10, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_text_font(uiInputLockToast, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(uiInputLockToast, LV_OBJ_FLAG_HIDDEN);

    uiMessageToast = lv_label_create(uiMainScreen);
    lv_obj_set_size(uiMessageToast, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiMessageToast, 0, -6);
    lv_obj_set_align(uiMessageToast, LV_ALIGN_BOTTOM_MID);
    UI_TE(lv_label_set_text(uiMessageToast, FST("More info is best info!")));
    lv_obj_set_style_text_align(uiMessageToast, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiMessageToast, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiMessageToast, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiMessageToast, lv_color_hex(0x606060), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiMessageToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(uiMessageToast, lv_color_hex(0xF0F0F0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(uiMessageToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(uiMessageToast, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(uiMessageToast, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_ver(uiMessageToast, 4, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_pad_hor(uiMessageToast, 10, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_text_font(uiMessageToast, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(uiMessageToast, LV_OBJ_FLAG_HIDDEN);

    uiErrorToast = lv_label_create(uiMainScreen);
    lv_obj_set_size(uiErrorToast, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiErrorToast, 0, 0);
    lv_obj_set_align(uiErrorToast, LV_ALIGN_CENTER);
    UI_TE(lv_label_set_text(uiErrorToast, FST("There was a bad error!")));
    lv_obj_set_style_text_align(uiErrorToast, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiErrorToast, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiErrorToast, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiErrorToast, lv_color_hex(0xA01010), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiErrorToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(uiErrorToast, lv_color_hex(0xF04040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(uiErrorToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(uiErrorToast, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(uiErrorToast, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_ver(uiErrorToast, 4, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_pad_hor(uiErrorToast, 10, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_text_font(uiErrorToast, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(uiErrorToast, LV_OBJ_FLAG_HIDDEN);

    uiEmergencyStopToast = lv_label_create(uiMainScreen);
    lv_obj_set_size(uiEmergencyStopToast, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiEmergencyStopToast, 0, -80);
    lv_obj_set_align(uiEmergencyStopToast, LV_ALIGN_CENTER);
    lv_label_set_text(uiEmergencyStopToast, FST("Emergency Stop!"));
    lv_obj_set_style_text_align(uiEmergencyStopToast, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiEmergencyStopToast, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiEmergencyStopToast, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiEmergencyStopToast, lv_color_hex(0xA01010), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiEmergencyStopToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(uiEmergencyStopToast, lv_color_hex(0xF04040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(uiEmergencyStopToast, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(uiEmergencyStopToast, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(uiEmergencyStopToast, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_ver(uiEmergencyStopToast, 10, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_pad_hor(uiEmergencyStopToast, 20, LV_PART_MAIN | LV_STATE_DEFAULT);    
    lv_obj_set_style_text_font(uiEmergencyStopToast, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(uiEmergencyStopToast, LV_OBJ_FLAG_HIDDEN);
}

void uiInit(void) {
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    uiMainScreenInit();
    lv_disp_load_scr(uiMainScreen);
}

void uiRun(uint32_t now) {
    if (_hideMessageToastTs && _hideMessageToastTs < now) {
        lv_obj_add_flag(uiMessageToast, LV_OBJ_FLAG_HIDDEN);
        _hideMessageToastTs = 0;
    }
    if (_hideErrorToastTs && _hideErrorToastTs < now) {
        lv_obj_add_flag(uiErrorToast, LV_OBJ_FLAG_HIDDEN);
        _hideErrorToastTs = 0;
    }
}

void showMessageToast(const char* text, uint32_t timeMs) {
    lv_label_set_text(uiMessageToast, text);
    lv_obj_clear_flag(uiMessageToast, LV_OBJ_FLAG_HIDDEN);
    if (!timeMs) { _hideMessageToastTs = 0; }
    else { _hideMessageToastTs = millis() + timeMs; }    
}

void hideMessageToast() {
    lv_obj_add_flag(uiMessageToast, LV_OBJ_FLAG_HIDDEN);
    _hideMessageToastTs = 0;
}

void showErrorToast(const char* text, uint32_t timeMs) {
    lv_label_set_text(uiErrorToast, text);
    lv_obj_clear_flag(uiErrorToast, LV_OBJ_FLAG_HIDDEN);
    if (!timeMs) { _hideErrorToastTs = 0; }
    else { _hideErrorToastTs = millis() + timeMs; }    
}

void hideErrorToast() {
    lv_obj_add_flag(uiErrorToast, LV_OBJ_FLAG_HIDDEN);
    _hideErrorToastTs = 0;
}
