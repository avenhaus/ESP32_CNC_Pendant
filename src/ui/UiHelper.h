#ifndef UI_HELPER_H
#define UI_HELPER_H

#include "lvgl.h"

lv_obj_t* uiCreatePanel(lv_obj_t* parent, const lv_font_t* font, lv_coord_t x, lv_coord_t y, lv_coord_t w);

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