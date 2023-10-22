// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: UI_test

#include "../ui.h"

void ui_main_screen_screen_init(void)
{
ui_main_screen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_main_screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_main_screen, lv_color_hex(0x0D0269), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_main_screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_returntostart = lv_btn_create(ui_main_screen);
lv_obj_set_width( ui_returntostart, 20);
lv_obj_set_height( ui_returntostart, 20);
lv_obj_set_x( ui_returntostart, lv_pct(1) );
lv_obj_set_y( ui_returntostart, lv_pct(-1) );
lv_obj_set_align( ui_returntostart, LV_ALIGN_BOTTOM_LEFT );
lv_obj_add_flag( ui_returntostart, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_returntostart, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_returntostart, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_returntostart, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_returntostart, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_returntostart, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_returntostart, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_returntostart, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_returntostart, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_x(ui_returntostart, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_ofs_y(ui_returntostart, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_mainpanel = lv_obj_create(ui_main_screen);
lv_obj_set_width( ui_mainpanel, lv_pct(75));
lv_obj_set_height( ui_mainpanel, lv_pct(90));
lv_obj_set_x( ui_mainpanel, lv_pct(-2) );
lv_obj_set_y( ui_mainpanel, lv_pct(2) );
lv_obj_set_align( ui_mainpanel, LV_ALIGN_BOTTOM_RIGHT );
lv_obj_clear_flag( ui_mainpanel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN );    /// Flags
lv_obj_set_style_radius(ui_mainpanel, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_mainpanel, lv_color_hex(0xB0E0FF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_mainpanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_mainpanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_width(ui_mainpanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_outline_pad(ui_mainpanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

// Custom buttonmatrix code
static const char * btnm_map[] = {"1", "2", "\n",
                                  "3", "4", "\n",
                                  "5", "6", 
                                   ""
                                 };

buttonmatrixtest = lv_btnmatrix_create(ui_mainpanel);
lv_btnmatrix_set_map(buttonmatrixtest, btnm_map);
lv_btnmatrix_set_btn_ctrl_all(buttonmatrixtest, LV_BTNMATRIX_CTRL_NO_REPEAT);
lv_btnmatrix_set_btn_ctrl_all(buttonmatrixtest, LV_BTNMATRIX_CTRL_CHECKABLE);
lv_btnmatrix_set_one_checked(buttonmatrixtest, true);

// Set buttonmatrix styles
const lv_style_const_prop_t btnmatrix_mainstyle_props[] = {
    LV_STYLE_CONST_WIDTH(lv_pct(100)), // Set width to 100%
    LV_STYLE_CONST_BG_OPA(0), // Set background opacity to 0
    LV_STYLE_CONST_BORDER_OPA(0) // Set border opacity to 0
};

const lv_style_const_prop_t btnmatrix_btnstyle_props[] = {
    LV_STYLE_CONST_HEIGHT(60)
};

LV_STYLE_CONST_INIT(btnmatrix_mainstyle, btnmatrix_mainstyle_props);
LV_STYLE_CONST_INIT(btnmatrix_btnstyle, btnmatrix_btnstyle_props);

// Apply buttonmatrix styles
lv_obj_add_style(buttonmatrixtest, &btnmatrix_mainstyle, LV_PART_MAIN);
lv_obj_add_style(buttonmatrixtest, &btnmatrix_btnstyle, LV_PART_ITEMS);

// Add buttonmatrixtest to joystick input
btnmatrixgrp = lv_group_create();
lv_group_add_obj(btnmatrixgrp, buttonmatrixtest);
lv_indev_set_group(indev_joystick, btnmatrixgrp);

lv_obj_add_event_cb(ui_returntostart, ui_event_returntostart, LV_EVENT_ALL, NULL);

}
