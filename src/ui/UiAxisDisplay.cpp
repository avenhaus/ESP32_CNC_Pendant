#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* uiPanelWAxis;
lv_obj_t* uiPanelMAxis;
UiAxis uiAxis[6];

/* ============================================== *\
 * Event Callbacks
\* ============================================== */

static void _uiNewAxisPos(float value, bool isCancelled, void* cbData) {
    if (isCancelled) { return; }
    UiAxisCoord* uia = (UiAxisCoord*) cbData;
    char buffer[64];
    sprintf(buffer, uia->fmt, value);
    lv_label_set_text(uia->text, buffer);
}

static void _uiGetNewAxisPos(lv_event_t* e) {
    lv_event_code_t event = lv_event_get_code(e);
    if(event != LV_EVENT_RELEASED) { return; }
    lv_obj_t* ta = lv_event_get_target(e);
    UiAxisCoord* uia = (UiAxisCoord*) lv_event_get_user_data(e);
    char buffer[64];
    sprintf(buffer, FST("New Axis Position %s"), lv_label_get_text(uia->label));
    uiShowKeyboard(buffer, nullptr, lv_label_get_text(ta), _uiNewAxisPos, uia);
}

/* ============================================== *\
 * Functions
\* ============================================== */

void uiCreatePanelAxis(UiAxisCoord& uiAxis, lv_obj_t* parent, char letter, char tl, const char* fmt, lv_coord_t x, lv_coord_t y, lv_event_cb_t ev) {
    uiAxis.fmt = fmt;
 
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
    uiPanelWAxis = uiCreatePanel(parent, &lv_font_montserrat_32, x, y, 244);

    uiCreatePanelAxis(uiAxis[0].world, uiPanelWAxis, 'X', 'w', UI_AXIS_PREC, -32, 0, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[1].world, uiPanelWAxis, 'Y', 'w', UI_AXIS_PREC, -32, 32, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[2].world, uiPanelWAxis, 'Z', 'w', UI_AXIS_PREC, -32, 64, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[3].world, uiPanelWAxis, 'A', 'w', UI_AXIS_PREC, -32, 96, _uiGetNewAxisPos);

    const int xpad = -2;
    const int ypad = 5;
    uiAxis[0].wZeroButton = uiCreateButton(uiPanelWAxis, xpad, ypad +  0, 25, 25, FST("0"), &lv_font_montserrat_22);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[0].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    uiAxis[1].wZeroButton = uiCreateButton(uiPanelWAxis, xpad, ypad + 32, 25, 25, FST("0"), &lv_font_montserrat_22);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[1].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    uiAxis[2].wZeroButton = uiCreateButton(uiPanelWAxis, xpad, ypad + 64, 25, 25, FST("0"), &lv_font_montserrat_22);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[2].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    uiAxis[3].wZeroButton = uiCreateButton(uiPanelWAxis, xpad, ypad + 96, 25, 25, FST("0"), &lv_font_montserrat_22);
    lv_obj_set_style_text_decor(lv_obj_get_child(uiAxis[3].wZeroButton, 0), LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);

    return uiPanelWAxis;
}

lv_obj_t* uiCreatePanelMAxis(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    uiPanelMAxis = uiCreatePanel(parent, &lv_font_montserrat_32, x, y, 215);

    uiCreatePanelAxis(uiAxis[0].machine, uiPanelMAxis, 'X', 'm', UI_AXIS_PREC, 0, 0, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[1].machine, uiPanelMAxis, 'Y', 'm', UI_AXIS_PREC, 0, 32, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[2].machine, uiPanelMAxis, 'Z', 'm', UI_AXIS_PREC, 0, 64, _uiGetNewAxisPos);
    uiCreatePanelAxis(uiAxis[3].machine, uiPanelMAxis, 'A', 'm', UI_AXIS_PREC, 0, 96, _uiGetNewAxisPos);

    return uiPanelMAxis;
}

