#ifndef UI_HELPER_H
#define UI_HELPER_H

#include "lvgl.h"

lv_obj_t* uiCreatePanel(lv_obj_t* parent, const lv_font_t* font, lv_coord_t x, lv_coord_t y, lv_coord_t w);

lv_obj_t* uiCreateButton(lv_obj_t* parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char* text, const lv_font_t* font, lv_event_cb_t eventCb=nullptr, void *userData=nullptr, lv_event_code_t filter=LV_EVENT_CLICKED);

void uiUpdateLabel(lv_obj_t* l, const char* text, int32_t color=-1, int32_t bgColor=-1);

/* ============================================== *\
 * Macros
\* ============================================== */

#ifndef SHOW_UI_TEXT_EXAMPLES
#define SHOW_UI_TEXT_EXAMPLES 1
#endif 

#if SHOW_UI_TEXT_EXAMPLES
#define UI_TE(...) __VA_ARGS__
#else
#define UI_TE(...)
#endif

#endif // UI_HELPER_H