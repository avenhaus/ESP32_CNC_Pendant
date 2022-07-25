#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* uiPanelAxis;
UiAxis uiAxis[4];

/* ============================================== *\
 * Event Callbacks
\* ============================================== */

static void _uiNewAxisPos(float value, bool isCancelled, void* cbData) {
    if (isCancelled) { return; }
    UiAxis* uia = (UiAxis*) cbData;
    char buffer[64];
    sprintf(buffer, uia->fmt, value);
    lv_label_set_text(uia->text, buffer);
}

static void _uiGetNewAxisPos(lv_event_t* e) {
    lv_event_code_t event = lv_event_get_code(e);
    if(event != LV_EVENT_RELEASED) { return; }
    lv_obj_t* ta = lv_event_get_target(e);
    UiAxis* uia = (UiAxis*) lv_event_get_user_data(e);
    char buffer[64];
    sprintf(buffer, FST("New %c Axis Position:"), uia->letter);
    uiShowKeyboard(buffer, nullptr, lv_label_get_text(ta), _uiNewAxisPos, uia);
}

/* ============================================== *\
 * Functions
\* ============================================== */

void uiCreatePanelAxis(UiAxis& uiAxis, lv_obj_t* parent, char letter, const char* fmt, lv_coord_t y, lv_event_cb_t ev) {
    uiAxis.fmt = fmt;
    uiAxis.letter = letter;
 
    uiAxis.text = lv_label_create(parent);
    lv_obj_set_size(uiAxis.text, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_pos(uiAxis.text, 0, y);
    lv_obj_set_style_text_align(uiAxis.text, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(uiAxis.text, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_color(uiAxis.text, lv_color_hex(UI_COLOR_PANEL_SELECT), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(uiAxis.text, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    if (ev) { lv_obj_add_event_cb(uiAxis.text, ev, LV_EVENT_RELEASED, &uiAxis); }
    UI_TE(lv_label_set_text(uiAxis.text, FST("1000.00")));

    uiAxis.label = lv_label_create(parent);
    lv_obj_set_size(uiAxis.label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_pos(uiAxis.label, 0, y);
    char buffer[64];
    buffer[0] = letter;
    buffer[1] = ':';
    buffer[2] = '\0';
    lv_label_set_text(uiAxis.label, buffer);
}

lv_obj_t*  uiCreatePanelAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    uiPanelAxis = uiCreatePanel(parent, &lv_font_montserrat_32, x, y, 190);

    uiCreatePanelAxis(uiAxis[0], uiPanelAxis, 'X', UI_AXIS_PREC, 0, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[1], uiPanelAxis, 'Y', UI_AXIS_PREC, 32, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[2], uiPanelAxis, 'Z', UI_AXIS_PREC, 64, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[3], uiPanelAxis, 'R', UI_AXIS_PREC, 96, _uiGetNewAxisPos);
    
    return uiPanelAxis;
}