/*******************************************************************************
 * Size: 12 px
 * Bpp: 2
 * Opts: 
 ******************************************************************************/

#include "lvgl.h"

#ifndef LV_FONT_ROBOTO_MONO_NUMBERS_12
#define LV_FONT_ROBOTO_MONO_NUMBERS_12 1
#endif

#if LV_FONT_ROBOTO_MONO_NUMBERS_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0024 "$" */
    0x3, 0x0, 0x1d, 0x2, 0xde, 0xc, 0xc, 0x34,
    0x10, 0x78, 0x0, 0x2d, 0x0, 0x1c, 0x70, 0x30,
    0xd6, 0xc0, 0xb8, 0x0, 0xc0,

    /* U+0025 "%" */
    0x2e, 0x0, 0x18, 0xc2, 0x6, 0x32, 0x0, 0xb5,
    0x40, 0x0, 0xc0, 0x0, 0x9b, 0x80, 0x22, 0x20,
    0x20, 0x88, 0x0, 0x2e, 0x0,

    /* U+0028 "(" */
    0x0, 0x6, 0xc, 0x28, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x24, 0x18, 0x8, 0x2,

    /* U+0029 ")" */
    0x0, 0x90, 0x30, 0x18, 0xc, 0xc, 0xd, 0xd,
    0xc, 0xc, 0xc, 0x24, 0x30, 0x80,

    /* U+002A "*" */
    0xc, 0x13, 0x17, 0xf8, 0x78, 0x33, 0x0, 0x0,

    /* U+002B "+" */
    0x2, 0x0, 0xc, 0x0, 0x30, 0x2f, 0xfd, 0x7,
    0x0, 0xc, 0x0, 0x30, 0x0,

    /* U+002C "," */
    0x76, 0x90,

    /* U+002D "-" */
    0xbc,

    /* U+002E "." */
    0x10, 0xc0,

    /* U+002F "/" */
    0x2, 0x40, 0xc0, 0x20, 0x24, 0xc, 0x6, 0x2,
    0x40, 0xc0, 0x60, 0x20, 0x0,

    /* U+0030 "0" */
    0xb, 0xc0, 0x91, 0xc3, 0x3, 0xc, 0x2d, 0x33,
    0x64, 0xf0, 0x93, 0x3, 0x9, 0x1c, 0xb, 0xc0,

    /* U+0031 "1" */
    0x0, 0x1e, 0xaa, 0xa, 0xa, 0xa, 0xa, 0xa,
    0xa, 0xa,

    /* U+0032 "2" */
    0x1f, 0x80, 0xc2, 0x85, 0x7, 0x0, 0x28, 0x0,
    0xc0, 0xd, 0x0, 0xd0, 0x9, 0x0, 0x7f, 0xf0,

    /* U+0033 "3" */
    0x1f, 0x83, 0xa, 0x10, 0x70, 0xa, 0xb, 0xc0,
    0xa, 0x10, 0x33, 0xa, 0x1f, 0x80,

    /* U+0034 "4" */
    0x0, 0xd0, 0xb, 0x40, 0x79, 0x3, 0x24, 0x18,
    0x90, 0xc2, 0x4b, 0xff, 0x80, 0x24, 0x0, 0x90,

    /* U+0035 "5" */
    0x3f, 0xc6, 0x0, 0x60, 0xb, 0xf4, 0x51, 0xc0,
    0x9, 0x40, 0x9a, 0xc, 0x2f, 0x40,

    /* U+0036 "6" */
    0x3, 0xc0, 0x74, 0x3, 0x40, 0xe, 0xf0, 0x34,
    0x70, 0xc0, 0xc3, 0x3, 0xa, 0x1c, 0xb, 0xc0,

    /* U+0037 "7" */
    0x7f, 0xf4, 0x0, 0xc0, 0x6, 0x0, 0x30, 0x1,
    0xc0, 0x9, 0x0, 0x30, 0x2, 0x80, 0xc, 0x0,

    /* U+0038 "8" */
    0x2f, 0x4a, 0xc, 0x90, 0xc6, 0x1c, 0x3f, 0x4a,
    0xc, 0xc0, 0x9a, 0xc, 0x2f, 0x40,

    /* U+0039 "9" */
    0xb, 0x80, 0xd2, 0x83, 0x3, 0xc, 0xc, 0x38,
    0x70, 0x3e, 0xc0, 0x3, 0x0, 0x74, 0xf, 0x40,

    /* U+003A ":" */
    0x30, 0x40, 0x0, 0x0, 0x43, 0x0,

    /* U+003D "=" */
    0x3f, 0xe0, 0x0, 0x0, 0x3, 0xfe, 0x0, 0x0,

    /* U+0078 "x" */
    0x70, 0xd3, 0x58, 0xf, 0x0, 0xe0, 0x1f, 0x3,
    0x5c, 0x70, 0xd0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 108, .box_w = 7, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 21, .adv_w = 141, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 66, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 56, .adv_w = 67, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 70, .adv_w = 83, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 78, .adv_w = 109, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 91, .adv_w = 38, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 93, .adv_w = 53, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 94, .adv_w = 51, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 96, .adv_w = 79, .box_w = 5, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 109, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 115, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 211, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 227, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 47, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 105, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 271, .adv_w = 95, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_2[] = {
    0x0, 0x3b
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 36, .range_length = 2, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 40, .range_length = 19, .glyph_id_start = 3,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 61, .range_length = 60, .glyph_id_start = 22,
        .unicode_list = unicode_list_2, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    10, 10
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -21
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 1,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 3,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t lv_font_roboto_mono_numbers_12 = {
#else
lv_font_t lv_font_roboto_mono_numbers_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if LV_FONT_ROBOTO_MONO_NUMBERS_12*/

