
#include "lvgl.h"
#include "Ui.h"
#include "UiHelper.h"

/* ============================================== *\
 * Variables
\* ============================================== */

lv_obj_t* uiButton[UIB_MAX] = { nullptr };
	 
// https://docs.lvgl.io/latest/en/html/overview/font.html#add-new-symbols
// https://fontawesome.com/

// TODO: 0xe4e4 (solid Cloud-showers-water), 0xe005, 0xf601, 0xe55b, 0xe4bb, 0xf192 (regular circle-dot)

// Solid
// 0xf043, 0xf2cc, 0xf740, 0xf5c7, 0xf084, 0xf0eb, 0xf023, 0xf3c1, 0xf5bd, 0xf773, 0xf72e, 0Xf05b
const char* UI_SYMBOL_LOCK = FST("\xEF\x80\xA3"); /*U+F023 */
const char* UI_SYMBOL_DROP = FST("\xEF\x81\x83"); /*U+F043 */
const char* UI_SYMBOL_CROSSHAIRS = FST("\xEF\x81\x9B"); /*U+F05B */
const char* UI_SYMBOL_KEY = FST("\xEF\x82\x84"); /*U+F084 */
const char* UI_SYMBOL_BULB = FST("\xEF\x83\xAB"); /*U+F0EB */
// const char* UI_SYMBOL_WIFI = FST("\xEF\x87\xAB"); /*U+F1EB */
// const char* UI_SYMBOL_BT = FST("\xEF\x8A\x93"); /*U+F293 */
const char* UI_SYMBOL_SHOWER = FST("\xEF\x8B\x8C"); /*U+F2CC */
// const char* UI_SYMBOL_TRASH = FST("\xEF\x8B\xAD"); /*U+F2ED */
// const char* UI_SYMBOL_PEN = FST("\xEF\x8C\x83"); /*U+F303 */
const char* UI_SYMBOL_LOCK_OPEN = FST("\xEF\x8F\x81"); /*U+F3C1 */
const char* UI_SYMBOL_SPRAY = FST("\xEF\x96\xBD"); /*U+F5BD */
const char* UI_SYMBOL_DROP_OFF = FST("\xEF\x97\x87"); /*U+F5C7 */
const char* UI_SYMBOL_WIND = FST("\xEF\x9C\xAE"); /*U+F72E */
const char* UI_SYMBOL_RAIN = FST("\xEF\x9D\x80"); /*U+F740 */
const char* UI_SYMBOL_WATER = FST("\xEF\x9D\xB3"); /*U+F773 */

// Regular
// 0xf192
const char* UI_SYMBOL_DISC = FST("\xEF\x86\x92"); /*U+F192 */


/* ============================================== *\
 * Functions
\* ============================================== */

lv_obj_t* uiCreateButtons(lv_obj_t* parent, lv_coord_t x, lv_coord_t y) {
    int bs=30;
    uiButton[UIB_HOME] = uiCreateButton(parent, x,    y, bs, bs, LV_SYMBOL_HOME, &lv_font_montserrat_22);
    uiButton[UIB_ZERO] = uiCreateButton(parent, x+35, y, bs, bs, FST("0"), &lv_font_montserrat_22);
    lv_obj_set_style_text_decor(uiButton[UIB_ZERO], LV_TEXT_DECOR_STRIKETHROUGH, LV_PART_MAIN | LV_STATE_DEFAULT);
    uiButton[UIB_FILES] = uiCreateButton(parent, x+70, y, bs, bs, LV_SYMBOL_DIRECTORY, &lv_font_montserrat_22);

    uiButton[UIB_PLAY] =  uiCreateButton(parent, x,    y+35, bs, bs, LV_SYMBOL_PLAY, &lv_font_montserrat_22);
    uiButton[UIB_PAUSE] = uiCreateButton(parent, x+35, y+35, bs, bs, LV_SYMBOL_PAUSE, &lv_font_montserrat_22);
    uiButton[UIB_STOP] =  uiCreateButton(parent, x+70, y+35, bs, bs, LV_SYMBOL_STOP, &lv_font_montserrat_22);

    uiButton[UIB_SPINDLE] =     uiCreateButton(parent, x+0 , y+70, bs, bs, UI_SYMBOL_DISC, &lv_font_icon_22);
    uiButton[UIB_FLOOD] =     uiCreateButton(parent, x+35, y+70, bs, bs, UI_SYMBOL_WATER, &lv_font_icon_22);
    uiButton[UIB_MIST] =     uiCreateButton(parent, x+70, y+70, bs, bs, UI_SYMBOL_WIND, &lv_font_icon_22);

    uiButton[UIB_PROBE] =     uiCreateButton(parent, x, y+105, bs, bs, UI_SYMBOL_CROSSHAIRS, &lv_font_icon_22);
    uiButton[UIB_MACRO] =     uiCreateButton(parent, x+35, y+105, bs, bs, LV_SYMBOL_LIST, &lv_font_montserrat_22);
    uiButton[UIB_SETTINGS] = uiCreateButton(parent, x+70, y+105, bs, bs, LV_SYMBOL_SETTINGS, &lv_font_montserrat_22);

    // uiButton[UIB_RESET] = uiCreateButton(parent, x   , y+70, bs, bs, LV_SYMBOL_POWER, &lv_font_montserrat_22);
    // uiButton[UIB_HELP] =     uiCreateButton(parent, x+70, y+70, bs, bs, FST("?"), &lv_font_montserrat_22);
	
    return nullptr;
}
