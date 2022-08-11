#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* uiPanelSettings;
UiSetting uiPanelSettingsJogFeed;
UiSetting uiPanelSettingsJogStep;
UiSetting uiPanelSettingsSpeedOverride;
UiSetting uiPanelSettingsRapidsOverride;
UiSetting uiPanelSettingsFeedOverride;

lv_obj_t* uiPanelSettings2;
UiSetting uiPanelSettingsFeed;
UiSetting uiPanelSettingsSpeed;

lv_obj_t * uiCncStateLabel;
lv_obj_t * uiCncPinLabel;

const UiSetting* uiSetting [] = {
    &uiPanelSettingsJogStep,
    &uiPanelSettingsJogFeed
};

static int _highlightedSetting = -1;


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

static void _uiGetNewSetting(lv_event_t* e) {
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
    lv_obj_set_style_text_font(uiSetting.text, &lv_font_roboto_mono_numbers_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (ev) { lv_obj_add_event_cb(uiSetting.text, ev, LV_EVENT_RELEASED, &uiSetting); }
    UI_TE(lv_label_set_text(uiSetting.text, FST("500.0")));

    uiSetting.label = lv_label_create(parent);
    lv_obj_set_size(uiSetting.label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiSetting.label, 0, y);
    lv_obj_set_style_text_font(uiSetting.label, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    char buffer[64];
    snprintf(buffer, sizeof(buffer)-2, FST("%s:"), label);
    buffer[sizeof(buffer)-2] = '\0';
    lv_label_set_text(uiSetting.label, buffer);
}

lv_obj_t* uiCreatePanelSettings(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    uiPanelSettings = uiCreatePanel(parent, &lv_font_montserrat_22, x, y, 170);

    uiCreatePanelSetting(uiPanelSettingsFeed, uiPanelSettings, FST("Feed"), FST("%g"), 0, _uiGetNewSetting);
    uiCreatePanelSetting(uiPanelSettingsSpeed, uiPanelSettings, FST("Speed"), FST("%g"), 24, _uiGetNewSetting);
    uiCreatePanelSetting(uiPanelSettingsFeedOverride, uiPanelSettings, FST("FeedOv"), FST("%g"), 48, _uiGetNewSetting);
    uiCreatePanelSetting(uiPanelSettingsRapidsOverride, uiPanelSettings, FST("RapidOv"), FST("%g"), 72, _uiGetNewSetting);
    uiCreatePanelSetting(uiPanelSettingsSpeedOverride, uiPanelSettings, FST("SpeedOv"), FST("%g"), 96, _uiGetNewSetting);
    
    int ps = 144;
    uiPanelSettings2 = uiCreatePanel(parent, &lv_font_montserrat_22, x+180, y, ps);
    uiCreatePanelSetting(uiPanelSettingsJogFeed, uiPanelSettings2, FST("JFeed"), FST("%g"), 0, _uiGetNewSetting);
    uiCreatePanelSetting(uiPanelSettingsJogStep, uiPanelSettings2, FST("JStep"), FST("%g"), 24, _uiGetNewSetting);

    uiCncStateLabel = lv_label_create(uiPanelSettings2);
    lv_obj_set_size(uiCncStateLabel, ps - 10, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiCncStateLabel, 0, 51);
    lv_obj_set_align(uiCncStateLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(uiCncStateLabel, FST("???"));
    lv_obj_set_style_text_align(uiCncStateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiCncStateLabel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiCncStateLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiCncStateLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiCncStateLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(uiCncStateLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_flag(uiCncStateLabel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(uiCncStateLabel, lv_color_hex(0x505050), LV_PART_MAIN | LV_STATE_PRESSED);

    uiCncPinLabel = lv_label_create(uiPanelSettings2);
    lv_obj_set_size(uiCncPinLabel, ps - 10, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiCncPinLabel, 0, 78);
    lv_obj_set_align(uiCncPinLabel, LV_ALIGN_TOP_LEFT);
    lv_label_set_text(uiCncPinLabel, FST("-"));
    lv_obj_set_style_text_align(uiCncPinLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(uiCncPinLabel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(uiCncPinLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiCncPinLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiCncPinLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(uiCncPinLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    return uiPanelSettings;
}

void uiUpdateSettingValue(UiSetting& s, float v) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer)-1, s.fmt, v);   
    lv_label_set_text(s.text, buffer);
}

void uiHighlightCncSetting(int setting) {
    if (setting >= sizeof(uiSetting) / sizeof(UiSetting*) ) { setting = -1; }
    if (setting == _highlightedSetting) { return; }
    if (_highlightedSetting >= 0) {
        lv_obj_set_style_text_color(uiSetting[_highlightedSetting]->text, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(uiSetting[_highlightedSetting]->text, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(uiSetting[_highlightedSetting]->text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);        
    }
    if (setting >= 0) {
        lv_obj_set_style_text_color(uiSetting[setting]->text, lv_color_hex(UI_COLOR_SETTING_HIGHLIGHTED_TEXT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(uiSetting[setting]->text, lv_color_hex(UI_COLOR_SETTING_HIGHLIGHTED_BG), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(uiSetting[setting]->text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    _highlightedSetting = setting;
}