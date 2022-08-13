#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* uiPanelWAxis;
lv_obj_t* uiPanelMAxis;
UiAxis uiAxis[6];
static int _highlightedAxis = -1;
UiMoveAxisCB uiMoveAxisCB = nullptr;

/* ============================================== *\
 * Event Callbacks
\* ============================================== */

static void _uiNewAxisPos(float value, bool isCancelled, void* cbData) {
    if (isCancelled) { return; }
    UiAxisCoord* uia = (UiAxisCoord*) cbData;
    extern bool uiKeyboardInputTypeRel;
    if (uiMoveAxisCB) { uiMoveAxisCB(value, uiKeyboardInputTypeRel, uia); }
}

static void _uiGetNewAxisPos(lv_event_t* e) {
    lv_event_code_t event = lv_event_get_code(e);
    if(event != LV_EVENT_RELEASED) { return; }
    lv_obj_t* ta = lv_event_get_target(e);
    UiAxisCoord* uia = (UiAxisCoord*) lv_event_get_user_data(e);
    char buffer[64];
    sprintf(buffer, FST("Axis %s"), lv_label_get_text(uia->label));
    uiShowKeyboard(buffer, nullptr, lv_label_get_text(ta), true, _uiNewAxisPos, uia);
}

/* ============================================== *\
 * Functions
\* ============================================== */

void uiCreatePanelAxis(UiAxisCoord& uiAxis, lv_obj_t* parent, int axis, char letter, char tl, const char* fmt, lv_coord_t x, lv_coord_t y, lv_event_cb_t ev) {
    uiAxis.fmt = fmt;
    uiAxis.axis = axis;
 
    uiAxis.text = lv_label_create(parent);
    lv_obj_set_size(uiAxis.text, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_pos(uiAxis.text, x, y);
    lv_obj_set_style_text_align(uiAxis.text, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(uiAxis.text, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(uiAxis.text, lv_color_hex(UI_COLOR_PANEL_SELECT), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(uiAxis.text, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    if (ev) { lv_obj_add_event_cb(uiAxis.text, ev, LV_EVENT_RELEASED, &uiAxis); }
    UI_TE(lv_label_set_text(uiAxis.text, FST("-5000.000")));

    uiAxis.label = lv_label_create(parent);
    lv_obj_set_size(uiAxis.label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiAxis.label, 0, y+6);
    char buffer[64];
    buffer[0] = tl;
    buffer[1] = letter;
    buffer[2] = ':';
    buffer[3] = '\0';
    lv_label_set_text(uiAxis.label, buffer);
    lv_obj_set_style_text_font(uiAxis.label, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);
}

lv_obj_t* uiCreatePanelWAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    uiPanelWAxis = uiCreatePanel(parent, &lv_font_roboto_mono_numbers_32, x, y, 260);

    uiCreatePanelAxis(uiAxis[0].work, uiPanelWAxis, 0, 'X', 'w', UI_AXIS_PREC, -58, 0, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[1].work, uiPanelWAxis, 1, 'Y', 'w', UI_AXIS_PREC, -58, 32, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[2].work, uiPanelWAxis, 2, 'Z', 'w', UI_AXIS_PREC, -58, 64, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[3].work, uiPanelWAxis, 3, 'A', 'w', UI_AXIS_PREC, -58, 96, _uiGetNewAxisPos);

    int xpad = -29;
    const int ypad = 5;
    uiAxis[0].wZeroButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad +  0, 25, 25, FST("0"), &lv_font_montserrat_22, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[0].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    uiAxis[1].wZeroButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad + 32, 25, 25, FST("0"), &lv_font_montserrat_22, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[1].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    uiAxis[2].wZeroButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad + 64, 25, 25, FST("0"), &lv_font_montserrat_22, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[2].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    uiAxis[3].wZeroButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad + 96, 25, 25, FST("0"), &lv_font_montserrat_22, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[3].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);

    xpad = 0;
    uiAxis[0].wHomeButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad +  0, 25, 25, LV_SYMBOL_HOME, &lv_font_montserrat_18, LV_ALIGN_TOP_RIGHT);
    uiAxis[1].wHomeButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad + 32, 25, 25, LV_SYMBOL_HOME, &lv_font_montserrat_18, LV_ALIGN_TOP_RIGHT);
    uiAxis[2].wHomeButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad + 64, 25, 25, LV_SYMBOL_HOME, &lv_font_montserrat_18, LV_ALIGN_TOP_RIGHT);
    uiAxis[3].wHomeButton = uiCreateButtonSmall(uiPanelWAxis, xpad, ypad + 96, 25, 25, LV_SYMBOL_HOME, &lv_font_montserrat_18, LV_ALIGN_TOP_RIGHT);

    return uiPanelWAxis;
}

lv_obj_t* uiCreatePanelMAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    uiPanelMAxis = uiCreatePanel(parent, &lv_font_roboto_mono_numbers_32, x, y, 204);

    uiCreatePanelAxis(uiAxis[0].machine, uiPanelMAxis, 0, 'X', 'm', UI_AXIS_PREC, 1, 0, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[1].machine, uiPanelMAxis, 1, 'Y', 'm', UI_AXIS_PREC, 1, 32, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[2].machine, uiPanelMAxis, 2, 'Z', 'm', UI_AXIS_PREC, 1, 64, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[3].machine, uiPanelMAxis, 3, 'A', 'm', UI_AXIS_PREC, 1, 96, _uiGetNewAxisPos);

    return uiPanelMAxis;
}

void uiUpdateAxisValue(UiAxisCoord& s, float v) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer)-1, s.fmt, v);   
    lv_label_set_text(s.text, buffer);
}


void uiHighlightAxis(int axis) {
    if (axis == _highlightedAxis) { return; }
    if (_highlightedAxis >= 0) {
        lv_obj_set_style_text_color(uiAxis[_highlightedAxis].work.text, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(uiAxis[_highlightedAxis].work.text, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(uiAxis[_highlightedAxis].work.text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);        
        lv_obj_set_style_text_color(uiAxis[_highlightedAxis].machine.text, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(uiAxis[_highlightedAxis].machine.text, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(uiAxis[_highlightedAxis].machine.text, 0, LV_PART_MAIN | LV_STATE_DEFAULT);        
    }
    if (axis >= 0) {
        lv_obj_set_style_text_color(uiAxis[axis].work.text, lv_color_hex(UI_COLOR_AXIS_HIGHLIGHTED_TEXT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(uiAxis[axis].work.text, lv_color_hex(UI_COLOR_AXIS_HIGHLIGHTED_BG), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(uiAxis[axis].work.text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(uiAxis[axis].machine.text, lv_color_hex(UI_COLOR_AXIS_HIGHLIGHTED_TEXT), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(uiAxis[axis].machine.text, lv_color_hex(UI_COLOR_AXIS_HIGHLIGHTED_BG), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(uiAxis[axis].machine.text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    _highlightedAxis = axis;
}
