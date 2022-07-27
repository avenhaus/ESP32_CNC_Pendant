#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* uiPanelSettings;
UiSetting uiPanelSettingsFeed;
UiSetting uiPanelSettingsSpeed;
UiSetting uiPanelSettingsJogFeed;
UiSetting uiPanelSettingsJogStep;

/* ============================================== *\
 * Event Callbacks
\* ============================================== */

static void _uiJogSettingUpdated(float value, bool isCancelled, void* cbData) {
    if (isCancelled) { return; }
    UiSetting* uia = (UiSetting*) cbData;
    char buffer[64];
    sprintf(buffer, uia->fmt, value);
    lv_label_set_text(uia->text, buffer);
}

static void _uiGetNewJogSetting(lv_event_t* e) {
    lv_event_code_t event = lv_event_get_code(e);
    if(event != LV_EVENT_RELEASED) { return; }
    lv_obj_t* ta = lv_event_get_target(e);
    UiSetting* uis = (UiSetting*) lv_event_get_user_data(e);
    char buffer[64];
    sprintf(buffer, FST("New %s"), lv_label_get_text(uis->label));
    uiShowKeyboard(buffer, nullptr, lv_label_get_text(ta), _uiJogSettingUpdated, uis);
}

/* ============================================== *\
 * Functions
\* ============================================== */

void uiCreatePanelSetting(UiSetting& uiSetting, lv_obj_t* parent, const char* label, const char* fmt, lv_coord_t y, lv_event_cb_t ev) {
    uiSetting.fmt = fmt;

    uiSetting.text = lv_label_create(parent);
    lv_obj_set_size(uiSetting.text, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_pos(uiSetting.text, 0, y);
    lv_obj_set_style_text_align(uiSetting.text, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(uiSetting.text, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(uiSetting.text, lv_color_hex(UI_COLOR_PANEL_SELECT), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(uiSetting.text, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    if (ev) { lv_obj_add_event_cb(uiSetting.text, ev, LV_EVENT_RELEASED, &uiSetting); }
    UI_TE(lv_label_set_text(uiSetting.text, FST("500.0")));

    uiSetting.label = lv_label_create(parent);
    lv_obj_set_size(uiSetting.label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiSetting.label, 0, y);
    char buffer[64];
    snprintf(buffer, sizeof(buffer)-2, FST("%s:"), label);
    buffer[sizeof(buffer)-2] = '\0';
    lv_label_set_text(uiSetting.label, buffer);
}

lv_obj_t* uiCreatePanelSettings(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    uiPanelSettings = uiCreatePanel(parent, &lv_font_montserrat_22, x, y, 150);

    uiCreatePanelSetting(uiPanelSettingsFeed, uiPanelSettings, FST("Feed"), FST("%g"), 0, _uiGetNewJogSetting);
    uiCreatePanelSetting(uiPanelSettingsSpeed, uiPanelSettings, FST("Speed"), FST("%g"), 24, _uiGetNewJogSetting);
    uiCreatePanelSetting(uiPanelSettingsJogFeed, uiPanelSettings, FST("JFeed"), FST("%g"), 48, _uiGetNewJogSetting);
    uiCreatePanelSetting(uiPanelSettingsJogStep, uiPanelSettings, FST("JStep"), FST("%g"), 72, _uiGetNewJogSetting);
    
    return uiPanelSettings;
}