/*  This file was originally generated from Squareline Studio
    Then heavily modified by thad to fit it's current function 
    Contains functions that set up objects and does stuff in the main screen */

#include "../ui.h"

// Init function - sets up all objects to their intended initial state
void ui_main_screen_init(void)
{
    ui_main_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_main_screen, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_main_screen, lv_color_hex(0x0D0269), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_main_screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_returntostart = lv_btn_create(ui_main_screen);
    lv_obj_set_width(ui_returntostart, 20);
    lv_obj_set_height(ui_returntostart, 20);
    lv_obj_set_x(ui_returntostart, lv_pct(-2));
    lv_obj_set_y(ui_returntostart, lv_pct(2));
    lv_obj_set_align(ui_returntostart, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(ui_returntostart, LV_OBJ_FLAG_SCROLL_ON_FOCUS);                                                                                                          /// Flags
    lv_obj_clear_flag(ui_returntostart, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(ui_returntostart, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_returntostart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_returntostart, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_returntostart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_returntostart, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_returntostart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_returntostart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui_returntostart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui_returntostart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Left panel btnmatrix
    static const char *lp_btnm_map[] = {"A", "\n",
                                        "B", "\n",
                                        "C", "\n", 
                                        "D", "" };
    ui_leftpanel_btnmatrix = lv_btnmatrix_create(ui_main_screen);
    lv_obj_set_width(ui_leftpanel_btnmatrix, lv_pct(19));
    lv_obj_set_height(ui_leftpanel_btnmatrix, lv_pct(85));
    lv_obj_set_align(ui_leftpanel_btnmatrix, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_x(ui_leftpanel_btnmatrix, lv_pct(2));
    lv_obj_set_y(ui_leftpanel_btnmatrix, lv_pct(-2));

    // Apply controls
    lv_btnmatrix_set_map(ui_leftpanel_btnmatrix, lp_btnm_map);
    lv_btnmatrix_set_btn_ctrl_all(ui_leftpanel_btnmatrix, LV_BTNMATRIX_CTRL_NO_REPEAT);
    lv_btnmatrix_set_one_checked(ui_leftpanel_btnmatrix, true);
    lv_btnmatrix_set_selected_btn(ui_leftpanel_btnmatrix, 0); // Start with first button selected

    //  Apply buttonmatrix styles
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, 0);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_mainpanel_btnmatrix_btndefstyle, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_mainpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_mainpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_PRESSED);

    //  Create btnmatrix group
    ui_grp_lp_btnmatrix = lv_group_create();
    lv_group_add_obj(ui_grp_lp_btnmatrix, ui_leftpanel_btnmatrix);

    // Main panel btnmatrix
    static const char *mp_btnm_map[] = {"1", "2", "\n",
                                     "3", "4", "\n",
                                     "5", "6",
                                     ""};

    ui_mainpanel_btnmatrix = lv_btnmatrix_create(ui_main_screen);
    lv_obj_set_width(ui_mainpanel_btnmatrix, lv_pct(75));
    lv_obj_set_height(ui_mainpanel_btnmatrix, lv_pct(85));
    lv_obj_set_x(ui_mainpanel_btnmatrix, lv_pct(-2));
    lv_obj_set_y(ui_mainpanel_btnmatrix, lv_pct(-2));
    lv_obj_set_align(ui_mainpanel_btnmatrix, LV_ALIGN_BOTTOM_RIGHT);

    // Apply controls
    lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, mp_btnm_map);
    lv_btnmatrix_set_btn_ctrl_all(ui_mainpanel_btnmatrix, LV_BTNMATRIX_CTRL_NO_REPEAT);
    lv_btnmatrix_set_one_checked(ui_mainpanel_btnmatrix, true);

    // Apply buttonmatrix styles
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, 0);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_btndefstyle, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_PRESSED);
    
    // Create btnmatrix grp
    ui_grp_mp_btnmatrix = lv_group_create();
    lv_group_add_obj(ui_grp_mp_btnmatrix, ui_mainpanel_btnmatrix);

    // Add callbacks
    lv_obj_add_event_cb(ui_returntostart, ui_event_returntostart, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_leftpanel_btnmatrix, ui_event_leftpanel, LV_EVENT_ALL, NULL);
}

// Set indev to whatever group it has to be + reset button matrix
void ui_main_screen_setindev(lv_group_t *group, lv_obj_t *btnmatrix)
{
    // Add indevs to group
    lv_indev_set_group(indev_joystick, group);
    lv_indev_set_group(indev_button, group);    

    // Reset selected button
    lv_btnmatrix_set_selected_btn(btnmatrix, 0);
}

/*  Callback for pressing left panel buttons
    Removes indevs from left panel, adds them to main panel, and sets selected button ID to 0 
    Eventually, add functionality to filter panel that is loaded based on ID of button pressed in left panel? */ 
void ui_event_leftpanel(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (check_inputs_sel(event_code))
    {
        // Set indevs
        ui_main_screen_setindev(ui_grp_mp_btnmatrix, ui_mainpanel_btnmatrix);

        // Reset buttons
        lv_btnmatrix_set_selected_btn(ui_leftpanel_btnmatrix, LV_BTNMATRIX_BTN_NONE);
    }
}

/*  Callback for pressing the red button
    Returns user to main screen */
void ui_event_returntostart(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (check_inputs_sel(event_code))
    {
        _ui_screen_change(&ui_start_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_start_screen_init);
        ui_start_screen_setindev(ui_grp_init_button);
        _ui_screen_delete(&ui_main_screen);
    }
}
