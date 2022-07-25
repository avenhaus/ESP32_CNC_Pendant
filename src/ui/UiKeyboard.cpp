#include <stdlib.h>

#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* _uiKeyboardPanel;
lv_obj_t* _uiKeyboardLabel;
lv_obj_t* _uiKeyboardTextArea;
lv_obj_t* _uiKeyboardKeys;

UiKeyboardDoneCB _uiKeyboardDoneCallback = nullptr;
void* _uiKeyboardDoneCallbackData = nullptr;

/* ============================================== *\
 * Keyboard Definitions
\* ============================================== */

PROGMEM static const char* _uiNumberBtnmMap[] = {
    "1", "2", "3", LV_SYMBOL_BACKSPACE, "\n",
    "4", "5", "6", "C", "\n",
    "7", "8", "9", LV_SYMBOL_CLOSE, "\n",
    "+/-", "0", ".", LV_SYMBOL_OK, ""};

PROGMEM static const lv_btnmatrix_ctrl_t _uiNumberBtnmCtlMap[] = {
    1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS, 
    1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS,
    1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS,
    1, 1, 1, LV_KEYBOARD_CTRL_BTN_FLAGS, 1
};                               


/* ============================================== *\
 * Functions
\* ============================================== */

static void _uiKeyboardEventCb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* obj = lv_event_get_target(e);
    lv_keyboard_t* kb = (lv_keyboard_t*)obj;
    lv_obj_t* ta = kb->ta;

    if(code == LV_EVENT_READY) {
        lv_obj_add_flag(_uiKeyboardPanel, LV_OBJ_FLAG_HIDDEN);
        if (_uiKeyboardDoneCallback) {
            float result = atof(lv_textarea_get_text(ta));
            _uiKeyboardDoneCallback(result, false, _uiKeyboardDoneCallbackData); 
        }
        return;
    }
    else if(code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(_uiKeyboardPanel, LV_OBJ_FLAG_HIDDEN);
        if (_uiKeyboardDoneCallback) { _uiKeyboardDoneCallback(0.0, true, _uiKeyboardDoneCallbackData); }
        return;
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));
        if(strcmp(txt, FST("C")) == 0) {
            lv_textarea_set_text(ta, FST(""));
            return;
        }
        else if(strcmp(txt, FST(".")) == 0) {
            if (strchr(lv_textarea_get_text(ta), '.')) { return; }
        }
        else if(strcmp(txt, FST("0")) == 0) {
            if (strcmp(lv_textarea_get_text(ta), FST("0")) == 0 ||
                strcmp(lv_textarea_get_text(ta), FST("-0")) == 0 ||
                strcmp(lv_textarea_get_text(ta), FST("+0")) == 0) { return; }
        }
        lv_keyboard_def_event_cb(e);
    }
    else if(code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(obj, ta);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(obj, NULL);
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

lv_obj_t* uiCreateKeyboard(lv_obj_t* parent) {

    // _uiKeyboardPanel
    _uiKeyboardPanel = lv_obj_create(parent);
    lv_obj_set_size(_uiKeyboardPanel, lv_pct(80), LV_SIZE_CONTENT);
    lv_obj_set_pos(_uiKeyboardPanel, 0, 0);
    lv_obj_set_align(_uiKeyboardPanel, LV_ALIGN_CENTER);
    lv_obj_add_flag(_uiKeyboardPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(_uiKeyboardPanel, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(_uiKeyboardPanel, lv_color_hex(UI_COLOR_PANEL_BG), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_uiKeyboardPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(_uiKeyboardPanel, lv_color_hex(UI_COLOR_PANEL_BG_GRADIENT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(_uiKeyboardPanel, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(_uiKeyboardPanel, lv_color_hex(UI_COLOR_KEYBOARD_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(_uiKeyboardPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(_uiKeyboardPanel, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(_uiKeyboardPanel, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(_uiKeyboardPanel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(_uiKeyboardPanel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(_uiKeyboardPanel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(_uiKeyboardPanel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_uiKeyboardPanel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

  // _uiKeyboardLabel
    _uiKeyboardLabel = lv_label_create(_uiKeyboardPanel);
    lv_obj_set_size(_uiKeyboardLabel, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(_uiKeyboardLabel, 0, -2);
    lv_obj_set_align(_uiKeyboardLabel, LV_ALIGN_TOP_MID);
    lv_obj_set_style_text_color(_uiKeyboardLabel, lv_color_hex(0xFFFF80), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_opa(_uiKeyboardLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(_uiKeyboardLabel, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    UI_TE(lv_label_set_text(_uiKeyboardLabel, FST("Enter a Number")));


    // Create a text area. The keyboard will write here.
    _uiKeyboardTextArea = lv_textarea_create(_uiKeyboardPanel);
    lv_obj_set_size(_uiKeyboardTextArea, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_pos(_uiKeyboardTextArea, 0, 30);
    lv_obj_set_align(_uiKeyboardTextArea, LV_ALIGN_TOP_MID);
    lv_textarea_set_one_line(_uiKeyboardTextArea, true);
    lv_obj_add_state(_uiKeyboardTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

    lv_obj_set_style_text_color(_uiKeyboardTextArea, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_opa(_uiKeyboardTextArea, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(_uiKeyboardTextArea, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(_uiKeyboardTextArea, lv_color_hex(0x64F064), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(_uiKeyboardTextArea, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(_uiKeyboardTextArea, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_opa(_uiKeyboardTextArea, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(_uiKeyboardTextArea, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(_uiKeyboardTextArea, lv_color_hex(0x80F080), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_shadow_opa(_uiKeyboardTextArea, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(_uiKeyboardTextArea, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(_uiKeyboardTextArea, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(_uiKeyboardTextArea, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(_uiKeyboardTextArea, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(_uiKeyboardTextArea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(_uiKeyboardTextArea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_textarea_set_placeholder_text(_uiKeyboardTextArea, FST("0 "));

    _uiKeyboardKeys = lv_keyboard_create(_uiKeyboardPanel);
    lv_obj_set_size(_uiKeyboardKeys, lv_pct(100), 180);
    lv_obj_set_pos(_uiKeyboardKeys, 0, 80);
    lv_obj_set_align(_uiKeyboardKeys, LV_ALIGN_TOP_MID);
    lv_obj_set_style_bg_opa(_uiKeyboardKeys, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_keyboard_set_textarea(_uiKeyboardKeys, _uiKeyboardTextArea);
    lv_keyboard_set_map(_uiKeyboardKeys, LV_KEYBOARD_MODE_NUMBER, _uiNumberBtnmMap, _uiNumberBtnmCtlMap);
    lv_keyboard_set_mode(_uiKeyboardKeys, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_remove_event_cb(_uiKeyboardKeys, lv_keyboard_def_event_cb);
    lv_obj_add_event_cb(_uiKeyboardKeys, _uiKeyboardEventCb, LV_EVENT_ALL, NULL);
    return _uiKeyboardPanel;
}

void uiShowKeyboard(const char* description, const char* text, const char* placeholder, UiKeyboardDoneCB cb, void* cbData) {
    lv_obj_clear_flag(_uiKeyboardPanel, LV_OBJ_FLAG_HIDDEN);
    _uiKeyboardDoneCallback = cb;
    _uiKeyboardDoneCallbackData = cbData;
    if (description) { lv_label_set_text(_uiKeyboardLabel, description); }
    else { lv_label_set_text(_uiKeyboardLabel, EMPTY_STRING); }

    if (text) {
    } else {
        lv_textarea_set_text(_uiKeyboardTextArea, EMPTY_STRING); 
    }

    if (placeholder) {
        // Workaround for lvgl bug: Add space at end of string
        char buffer[64];
        size_t n=0;
        while (placeholder[n] && n<sizeof(buffer)-3) { buffer[n] = placeholder[n]; n++; }
        buffer[n++] = ' ';
        buffer[n++] = '\0';
        lv_textarea_set_placeholder_text(_uiKeyboardTextArea, buffer); 
    }
    else { lv_textarea_set_placeholder_text(_uiKeyboardTextArea, FST("0 ")); }
}