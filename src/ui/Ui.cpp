#include "Ui.h"

/* ============================================== *\
 * Variables
\* ============================================== */
lv_obj_t * uiMainScreen;

PROGMEM const char* UI_AXIS_PREC = "%0.3f";

/* ============================================== *\
 * Functions
\* ============================================== */

void uiMainScreenInit(void) {

    // uiMainScreen
    uiMainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(uiMainScreen, LV_OBJ_FLAG_SCROLLABLE);
    uiCreateStatusBar(uiMainScreen);
    uiCreatePanelWAxis(uiMainScreen, 5, 30);
    uiCreatePanelMAxis(uiMainScreen, 260, 30);
    uiCreatePanelSettings(uiMainScreen, 5, 185);
    uiCreateKeyboard(uiMainScreen);
}

void uiInit(void) {
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    uiMainScreenInit();
    lv_disp_load_scr(uiMainScreen);
}

