#ifndef UI_H
#define UI_H

#include "lvgl.h"
#include "VUEF.h"
//#include "Config.h"

void uiInit(void);
void uiRun(uint32_t now);

// Status Bar
lv_obj_t* uiCreateStatusBar(lv_obj_t* parent);

extern lv_obj_t* uiStatusBar;
extern lv_obj_t* uiStatusBarLocalIpAddr;
extern lv_obj_t* uiStatusBarTime;
extern lv_obj_t* uiStatusBarState;
extern lv_obj_t* uiStatusBarBattery;


// Axis Panel
typedef struct UiAxisCoord {
    lv_obj_t* label;
    lv_obj_t* text;
    const char* fmt;
    int axis;
} UiAxisCoord;

typedef struct UiAxis {
    UiAxisCoord work;
    UiAxisCoord machine;
    lv_obj_t* wZeroButton;
    lv_obj_t* wHomeButton;
} UiAxis;

typedef void (*UiMoveAxisCB)(float value, bool isRelative, UiAxisCoord* uiAxisCoord);

lv_obj_t* uiCreatePanelWAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);
lv_obj_t* uiCreatePanelMAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);
void uiUpdateAxisValue(UiAxisCoord& s, float v);
void uiHighlightAxis(int axis);

extern lv_obj_t* uiPanelWAxis;
extern lv_obj_t* uiPanelMAxis;
extern UiAxis uiAxis[6];
extern UiMoveAxisCB uiMoveAxisCB;


// Setting Panel
typedef void (*UiKeyboardCB)(float value, void* cbData);
typedef struct UiSetting {
    lv_obj_t* label;
    lv_obj_t* text;
    const char* fmt;
    UiKeyboardCB cb;
    void* cbData;
} UiSetting;

lv_obj_t* uiCreatePanelSettings(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);
void uiUpdateSettingValue(UiSetting& s, float v);
void uiHighlightCncSetting(int setting);

extern lv_obj_t* uiPanelSettings;
extern UiSetting uiPanelSettingsFeed;
extern UiSetting uiPanelSettingsSpeed;
extern UiSetting uiPanelSettingsJogFeed;
extern UiSetting uiPanelSettingsJogStep;

extern UiSetting uiPanelSettingsSpeedOverride;
extern UiSetting uiPanelSettingsRapidsOverride;
extern UiSetting uiPanelSettingsFeedOverride;

extern lv_obj_t* uiCncStateLabel;
extern lv_obj_t* uiCncPinLabel;

extern lv_obj_t * uiMessageToast;
extern lv_obj_t * uiErrorToast;

extern const lv_font_t lv_font_icon_22;
extern const lv_font_t lv_font_roboto_mono_numbers_18;
extern const lv_font_t lv_font_roboto_mono_numbers_22;
extern const lv_font_t lv_font_roboto_mono_numbers_32;

// Buttons
lv_obj_t* uiCreateButtons(lv_obj_t* parent, lv_coord_t x, lv_coord_t y);
typedef enum UI_BUTTON_ENUM { UIB_HOME, UIB_ZERO, UIB_FILES, UIB_SETTINGS, UIB_HELP, UIB_PLAY, UIB_PAUSE, UIB_STOP, UIB_SPINDLE, UIB_FLOOD, UIB_MIST, UIB_PROBE, UIB_MACRO, UIB_RESTART, UIB_MAX } UI_BUTTON_ENUM;
extern lv_obj_t* uiButton[UIB_MAX];

// Keyboard
typedef void (*UiKeyboardDoneCB)(float value, bool isCancelled, void* cbData);
lv_obj_t* uiCreateKeyboard(lv_obj_t* parent);
void uiShowKeyboard(const char* description=nullptr, const char* text=nullptr, const char* placeholder=nullptr, bool showAbsRel=false ,UiKeyboardDoneCB cb=nullptr, void* cbData=nullptr);

// Toast
void showMessageToast(const char* text, uint32_t timeMs=0);
void hideMessageToast();
void showErrorToast(const char* text, uint32_t timeMs=0);
void hideErrorToast();
extern lv_obj_t* uiEmergencyStopToast;
extern lv_obj_t* uiInputLockToast;


/* ============================================== *\
 * Constants
\* ============================================== */

#if USE_IPS_DISPLAY
#define UI_COLOR_PANEL_BG 0x3030A0
#define UI_COLOR_PANEL_BG_GRADIENT 0x101060
#else
#define UI_COLOR_PANEL_BG 0x303080
#define UI_COLOR_PANEL_BG_GRADIENT 0x101030
#endif
#define UI_COLOR_PANEL_BORDER 0x13DD33
#define UI_COLOR_PANEL_SELECT 0x207820
#define UI_COLOR_KEYBOARD_BORDER 0x7E7AE1
#define UI_COLOR_BUTTON_BG 0x085010
#define UI_COLOR_BUTTON_SHADOW 0x11D137
#define UI_COLOR_BUTTON_BG_PRESSED 0xA0D020
#define UI_COLOR_AXIS_HIGHLIGHTED_TEXT 0xFFFF20
#define UI_COLOR_AXIS_HIGHLIGHTED_BG 0x20B070
#define UI_COLOR_SETTING_HIGHLIGHTED_TEXT 0x20FFFF
#define UI_COLOR_SETTING_HIGHLIGHTED_BG 0x2070B0

extern const char* UI_AXIS_PREC;

#endif // UI_H