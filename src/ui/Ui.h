#ifndef UI_H
#define UI_H

#include "lvgl.h"
#include "VUEF.h"

void uiInit(void);

// Status Bar
lv_obj_t* uiCreateStatusBar(lv_obj_t* parent);

extern lv_obj_t* uiStatusBar;
extern lv_obj_t* uiStatusBarIpAddr;
extern lv_obj_t* uiStatusBarTime;
extern lv_obj_t* uiStatusBarState;


// Axis Panel
typedef struct UiAxis {
    lv_obj_t* label;
    lv_obj_t* text;
    const char* fmt;
    char letter;
} UiAxis;

lv_obj_t* uiCreatePanelAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);

extern lv_obj_t* uiPanelAxis;
extern UiAxis uiAxis[4];


// Jog Panel
typedef struct UiSetting {
    lv_obj_t* label;
    lv_obj_t* text;
    const char* fmt;
} UiSetting;

lv_obj_t* uiCreatePanelJogSettings(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);

extern lv_obj_t* uiPanelJogSettings;
extern UiSetting uiPanelJogDistance;
extern UiSetting uiPanelJogSpeed;


// Keyboard
typedef void (*UiKeyboardDoneCB)(float value, bool isCancelled, void* cbData);
lv_obj_t* uiCreateKeyboard(lv_obj_t* parent);
void uiShowKeyboard(const char* description=nullptr, const char* text=nullptr, const char* placeholder=nullptr, UiKeyboardDoneCB cb=nullptr, void* cbData=nullptr);


/* ============================================== *\
 * Constants
\* ============================================== */

#define UI_COLOR_PANEL_BG 0x303080
#define UI_COLOR_PANEL_BG_GRADIENT 0x101030
#define UI_COLOR_PANEL_BORDER 0x13DD33
#define UI_COLOR_PANEL_SELECT 0x207820
#define UI_COLOR_KEYBOARD_BORDER 0x7E7AE1

extern const char* UI_AXIS_PREC;

#endif // UI_H