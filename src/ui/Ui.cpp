#include "Ui.h"

/* ============================================== *\
 * Variables
\* ============================================== */
lv_obj_t * uiMainScreen;

PROGMEM const char* UI_AXIS_PREC = "%0.2f";

/* ============================================== *\
 * Functions
\* ============================================== */

void uiMainScreenInit(void) {

    // uiMainScreen
    uiMainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(uiMainScreen, LV_OBJ_FLAG_SCROLLABLE);
    uiCreateStatusBar(uiMainScreen);
    uiCreatePanelAxis(uiMainScreen, 5, 25);
    uiCreatePanelJogSettings(uiMainScreen, 5, 180);
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

