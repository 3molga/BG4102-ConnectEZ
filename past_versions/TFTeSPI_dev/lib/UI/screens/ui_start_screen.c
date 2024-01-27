/*  This file was originally generated from Squareline Studio
    Then heavily modified by thad to fit it's current function */

#include "../ui.h"

void ui_start_screen_init(void)
{
    ui_start_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_start_screen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_bg = lv_img_create(ui_start_screen);
    lv_img_set_src(ui_bg, &ui_img_1_bg_png);
    lv_obj_set_width(ui_bg, lv_pct(100));
    lv_obj_set_height(ui_bg, lv_pct(100));
    lv_obj_clear_flag(ui_bg, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_scrollbar_mode(ui_bg, LV_SCROLLBAR_MODE_OFF);

    ui_mainlogo = lv_img_create(ui_start_screen);
    lv_img_set_src(ui_mainlogo, &ui_img_logo_png);
    lv_obj_set_width(ui_mainlogo, LV_SIZE_CONTENT);  /// 100
    lv_obj_set_height(ui_mainlogo, LV_SIZE_CONTENT); /// 100
    lv_obj_set_x(ui_mainlogo, 0);
    lv_obj_set_y(ui_mainlogo, lv_pct(-2));
    lv_obj_set_align(ui_mainlogo, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_mainlogo, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags

    ui_development_label = lv_label_create(ui_start_screen);
    lv_obj_set_x(ui_development_label, 4);
    lv_obj_set_y(ui_development_label, 5);
    lv_label_set_text(ui_development_label, "Under Development v0.0.1");
    lv_obj_clear_flag(ui_development_label, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_scrollbar_mode(ui_development_label, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_text_color(ui_development_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_development_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_development_label, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_init_button = lv_btn_create(ui_start_screen);
    lv_obj_set_width(ui_init_button, 100);
    lv_obj_set_height(ui_init_button, 40);
    lv_obj_set_x(ui_init_button, 0);
    lv_obj_set_y(ui_init_button, lv_pct(30));
    lv_obj_set_align(ui_init_button, LV_ALIGN_CENTER);
    lv_obj_add_style(ui_init_button, &ui_btndefstyle, LV_STATE_FOCUS_KEY);

    ui_welcome_text = lv_label_create(ui_init_button);
    lv_obj_set_width(ui_welcome_text, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_welcome_text, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_welcome_text, LV_ALIGN_CENTER);
    lv_label_set_text(ui_welcome_text, "Welcome");
    lv_obj_clear_flag(ui_welcome_text, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN); /// Flags
    lv_obj_set_scrollbar_mode(ui_welcome_text, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_color(ui_welcome_text, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_welcome_text, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(ui_welcome_text, 1, LV_PART_MAIN | LV_STATE_PRESSED);

    // Create btn group
    ui_grp_init_button = lv_group_create();
    lv_group_add_obj(ui_grp_init_button, ui_init_button);

    lv_obj_add_event_cb(ui_init_button, ui_event_init_button, LV_EVENT_ALL, NULL);
}

// Set indevs to whatever group it has to be
void ui_start_screen_setindev(lv_group_t *group)
{
    lv_indev_set_group(indev_joystick, group);
    lv_indev_set_group(indev_button, group);
}

// Callback for init button
void ui_event_init_button(lv_event_t *e)
{
    if (check_inputs_sel(e))
    {
        _ui_screen_change(&ui_main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_main_screen_init);
        _ui_screen_delete(&ui_start_screen);

        // Set indevs
        ui_main_screen_setindev(ui_grp_lp_btnmatrix, ui_leftpanel_btnmatrix);

        // Reset buttons
        lv_btnmatrix_set_selected_btn(ui_mainpanel_btnmatrix, LV_BTNMATRIX_BTN_NONE);
    }
}