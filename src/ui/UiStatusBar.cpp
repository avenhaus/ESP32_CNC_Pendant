
#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* uiStatusBar;
lv_obj_t* uiStatusBarIpAddr;
lv_obj_t* uiStatusBarTime;
lv_obj_t* uiStatusBarState;


/* ============================================== *\
 * Functions
\* ============================================== */

lv_obj_t* uiCreateStatusBar(lv_obj_t* parent) {

    uiStatusBar = lv_obj_create(parent);

    lv_obj_set_width(uiStatusBar, lv_pct(100));
    lv_obj_set_height(uiStatusBar, LV_SIZE_CONTENT);
    lv_obj_set_align(uiStatusBar, LV_ALIGN_TOP_MID);

    lv_obj_clear_flag(uiStatusBar, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(uiStatusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(uiStatusBar, lv_color_hex(0x22039C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(uiStatusBar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(uiStatusBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(uiStatusBar, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(uiStatusBar, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(uiStatusBar, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(uiStatusBar, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(uiStatusBar, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // uiStatusBarIpAddr
    uiStatusBarIpAddr = lv_label_create(uiStatusBar);
    lv_obj_set_size(uiStatusBarIpAddr, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(uiStatusBarIpAddr, LV_ALIGN_LEFT_MID);
    UI_TE(lv_label_set_text(uiStatusBarIpAddr, FST("192.168.100.22")));

   // uiStatusBarTime
    uiStatusBarTime = lv_label_create(uiStatusBar);
    lv_obj_set_size(uiStatusBarTime, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(uiStatusBarTime, LV_ALIGN_RIGHT_MID);
    UI_TE(lv_label_set_text(uiStatusBarTime, FST("20:00:11 12.5.1971")));

    // uiStatusBarState
    uiStatusBarState = lv_label_create(uiStatusBar);
    lv_obj_set_size(uiStatusBarState, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(uiStatusBarState, LV_ALIGN_CENTER);
    UI_TE(lv_label_set_text(uiStatusBarState, FST("Not Connected")));

    return uiStatusBar;
}