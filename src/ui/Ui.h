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
typedef struct UiAxisCoord {
    lv_obj_t* label;
    lv_obj_t* text;
    const char* fmt;
} UiAxisCoord;

typedef struct UiAxis {
    UiAxisCoord work;
    UiAxisCoord machine;
    lv_obj_t* wZeroButton;
} UiAxis;


lv_obj_t* uiCreatePanelWAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);
lv_obj_t* uiCreatePanelMAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);
void uiUpdateAxisValue(UiAxisCoord& s, float v);
void uiHighlightAxis(int axis);

extern lv_obj_t* uiPanelWAxis;
extern lv_obj_t* uiPanelMAxis;
extern UiAxis uiAxis[6];


// Setting Panel
typedef struct UiSetting {
    lv_obj_t* label;
    lv_obj_t* text;
    const char* fmt;
} UiSetting;

lv_obj_t* uiCreatePanelSettings(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);
void uiUpdateSettingValue(UiSetting& s, float v);

extern lv_obj_t* uiPanelSettings;
extern UiSetting uiPanelSettingsFeed;
extern UiSetting uiPanelSettingsSpeed;
extern UiSetting uiPanelSettingsJogFeed;
extern UiSetting uiPanelSettingsJogStep;


extern lv_obj_t* uiCncStateLabel;


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
#define UI_COLOR_BUTTON_BG 0x085010
#define UI_COLOR_BUTTON_SHADOW 0x11D137
#define UI_COLOR_BUTTON_BG_PRESSED 0xA0D020

extern const char* UI_AXIS_PREC;

#endif // UI_H