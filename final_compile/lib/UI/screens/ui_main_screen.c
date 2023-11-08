/*  This file was originally generated from Squareline Studio
    Then heavily modified by thad to fit it's current function
    Contains functions that set up objects and does stuff in the main screen */

#include "../ui.h"

// Define symbols
#define PRONOUNS_EMOJI "\xEF\x80\x87"
#define QUESTIONS_EMOJI "\xEF\x84\xA8"
#define VERBS_EMOJI "\xEF\x95\x94"
#define TIME_EMOJI "\xEF\x80\x97"
#define PLACES_EMOJI "\xEF\x88\x9D"
#define FOOD_EMOJI "\xEF\x8B\xA7"
#define FEELINGS_EMOJI "\xEF\x98\xB0"
#define ADJECTIVES_EMOJI "\xEF\x83\x90"

// Define colours
#define DARK_BLUE lv_color_make(0, 0, 120)
#define LIGHT_BLUE lv_color_make(108, 161, 255)
#define DARK_GREEN lv_color_make(51, 105, 30)
#define LIGHT_GREEN lv_color_make(174, 213, 129)
#define DARK_RED lv_color_make(183, 28, 28)
#define LIGHT_RED lv_color_make(255, 170, 170)
#define DARK_CYAN lv_color_make(0, 108, 125)
#define LIGHT_CYAN lv_color_make(38, 198, 218)
#define DARK_YELLOW lv_color_make(145, 121, 0)
#define LIGHT_YELLOW lv_color_make(255, 235, 59)
#define DARK_PINK lv_color_make(215, 0, 150)
#define LIGHT_PINK lv_color_make(255, 145, 231)
#define DARK_BROWN lv_color_make(219, 71, 0)
#define LIGHT_BROWN lv_color_make(227, 141, 99)
#define DARK_GREY lv_color_make(110, 110, 110)
#define LIGHT_GREY lv_color_make(210, 210, 210)

// Define button height
#define MAINPANELBTNHEIGHT 80
#define LEFTPANELBTNHEIGHT 55

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
    // Create wrapper
    ui_leftpanel_wrapper = lv_obj_create(ui_main_screen);
    lv_obj_set_size(ui_leftpanel_wrapper, lv_pct(19), lv_pct(75));
    lv_obj_set_x(ui_leftpanel_wrapper, lv_pct(2));
    lv_obj_set_y(ui_leftpanel_wrapper, lv_pct(-2));
    lv_obj_set_align(ui_leftpanel_wrapper, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_add_style(ui_leftpanel_wrapper, &ui_mainpanel_btnmatrix_wrapperstyle, 0);
    lv_obj_set_style_bg_color(ui_leftpanel_wrapper, lv_color_make(165, 165, 165), 0); // Set bg to gray

    static const char *lp_btnm_map[] = {PRONOUNS_EMOJI, "\n",
                                        QUESTIONS_EMOJI, "\n",
                                        VERBS_EMOJI, "\n",
                                        TIME_EMOJI, "\n",
                                        PLACES_EMOJI, "\n",
                                        FOOD_EMOJI, "\n",
                                        FEELINGS_EMOJI, "\n",
                                        ADJECTIVES_EMOJI, ""};

    ui_leftpanel_btnmatrix = lv_btnmatrix_create(ui_leftpanel_wrapper);

    //  Apply buttonmatrix styles
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, 0);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_leftpanel_btnmatrix_btndefstyle, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_leftpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_leftpanel_btnmatrix, &ui_leftpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_PRESSED);

    //  Apply controls
    lv_btnmatrix_set_map(ui_leftpanel_btnmatrix, lp_btnm_map);
    lv_btnmatrix_set_btn_ctrl_all(ui_leftpanel_btnmatrix, LV_BTNMATRIX_CTRL_NO_REPEAT);
    lv_btnmatrix_set_one_checked(ui_leftpanel_btnmatrix, true);
    lv_btnmatrix_set_selected_btn(ui_leftpanel_btnmatrix, 0); // Start with first button selected

    // Set position
    lv_obj_set_width(ui_leftpanel_btnmatrix, lv_pct(100));
    lv_obj_set_height(ui_leftpanel_btnmatrix, calc_btnmatrix_height(ui_leftpanel_btnmatrix, LEFTPANELBTNHEIGHT));

    // Set scrollbar settings
    lv_obj_set_scrollbar_mode(ui_leftpanel_wrapper, LV_SCROLLBAR_MODE_OFF); // Disable scrollbar showing up on screen if not scrolling
    lv_obj_clear_flag(ui_leftpanel_wrapper, LV_OBJ_FLAG_SCROLL_MOMENTUM);   // Disable scrollbar momentum
    lv_obj_clear_flag(ui_leftpanel_wrapper, LV_OBJ_FLAG_SCROLL_ELASTIC);    // Disable scrollbar elasticity

    //  Create btnmatrix group
    ui_grp_lp_btnmatrix = lv_group_create();
    lv_group_add_obj(ui_grp_lp_btnmatrix, ui_leftpanel_btnmatrix);

    // Create and add btnmatrix grp
    ui_grp_mp_btnmatrix = lv_group_create();

    // Init a mainpanel
    ui_initmainpanel(0);

    // Add callbacks
    lv_obj_add_event_cb(ui_returntostart, ui_event_returntostart, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_leftpanel_btnmatrix, ui_event_leftpanel_sel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_leftpanel_btnmatrix, ui_event_leftpanel_esc, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_leftpanel_btnmatrix, ui_event_scroll_leftpanel, LV_EVENT_ALL, NULL);
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
    Prevent triggering when it's being scrolled
    Removes indevs from left panel, adds them to main panel, and sets left panel selected button ID to 0
    Also resets main panel scroll to the top
    Eventually, add functionality to filter panel that is loaded based on ID of button pressed in left panel? */
void ui_event_leftpanel_sel(lv_event_t *e)
{
    if (check_inputs_sel(e))
    {
        // Check btnid
        lv_obj_t *btnmatobj = lv_event_get_target(e);
        LV_ASSERT_OBJ(btnmat, lv_btnmatrix_t);
        lv_btnmatrix_t *btnm = (lv_btnmatrix_t *)btnmatobj;
        uint16_t btnid = btnm->btn_id_sel;

        // Based on that, create required mainpanel btnmat
        // First delete previous buttonmatrix
        lv_obj_del(ui_mainpanel_btnmatrix);

        // Then create new one
        ui_initmainpanel(btnid);

        // Set indevs
        ui_main_screen_setindev(ui_grp_mp_btnmatrix, ui_mainpanel_btnmatrix);

        // Reset buttons
        lv_btnmatrix_set_selected_btn(ui_leftpanel_btnmatrix, LV_BTNMATRIX_BTN_NONE);

        // Reset scroll to top
        lv_obj_scroll_to_y(ui_mainpanel_wrapper, 0, LV_ANIM_OFF);
    }
}

/*  Callback for pressing the red button
    Returns user to main screen */
void ui_event_returntostart(lv_event_t *e)
{
    if (check_inputs_sel(e))
    {
        _ui_screen_change(&ui_start_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_start_screen_init);
        ui_start_screen_setindev(ui_grp_init_button);
        _ui_screen_delete(&ui_main_screen);
    }
}

/*  Callback for main panel + esc button
    There's a POSSIBILITY of a bug here but fuck it we ball */
void ui_event_mainpanel_esc(lv_event_t *e)
{
    if (check_inputs_del(e))
    {
        // Reset indev and button
        ui_main_screen_setindev(ui_grp_lp_btnmatrix, ui_leftpanel_btnmatrix);
        lv_btnmatrix_set_selected_btn(ui_mainpanel_btnmatrix, LV_BTNMATRIX_BTN_NONE);

        // Reset scroll to top
        lv_obj_scroll_to_y(ui_leftpanel_wrapper, 0, LV_ANIM_ON);

    }
}

/*  Callback for left panel + esc button
    Pretty much a copy of the red button, might delete the red button in the future? Before release */
void ui_event_leftpanel_esc(lv_event_t *e)
{
    if (check_inputs_del(e))
    {
        _ui_screen_change(&ui_start_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_start_screen_init);
        ui_start_screen_setindev(ui_grp_init_button);
        _ui_screen_delete(&ui_main_screen);
    }
}

/*  Callback for scrolling btnmatrix panels
    Kinda experimental because I'm not too sure what I'm doing at this point in time */
void ui_event_scroll_mainpanel(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_KEY)
    {
        return;
    }

    int16_t scrollby; // -1 to scroll down, 1 to scroll up
    uint32_t key;
    key = lv_event_get_key(e);

    if (key != LV_KEY_UP && key != LV_KEY_DOWN && key != LV_KEY_RIGHT && key != LV_KEY_LEFT) // Not the keys we are interested in
    {
        return;
    }

    // Get buttonmatrix that was triggered
    lv_obj_t *btnmatobj = lv_event_get_target(e);
    LV_ASSERT_OBJ(btnmat, lv_btnmatrix_t);
    lv_btnmatrix_t *btnm = (lv_btnmatrix_t *)btnmatobj;

    // Get its parent wrapper
    lv_obj_t *btnwrapper = lv_obj_get_parent(btnmatobj);

    // Get the new button that's selected
    uint16_t btnid = btnm->btn_id_sel;

    // Do some maths
    // Since the scroll is bounded, the most important thing is to ensure that left/right works
    // Else it's all handled by bounded scroll huhu
    uint16_t col_count = btnm->btn_cnt / btnm->row_cnt; // m
    uint16_t col_cur = btnid % col_count;

    // Do general checks
    if (key == LV_KEY_DOWN) // Down
    {
        scrollby = -1;
    }
    else if (key == LV_KEY_UP) // Up
    {
        scrollby = 1;
    }
    else if (key == LV_KEY_RIGHT) // Right
    {
        if (col_cur == 0) // Scroll down if on rightmost col
        {
            scrollby = -1;
        }
        else
        {
            return;
        }
    }
    else if (key == LV_KEY_LEFT) // Left
    {
        if (col_cur == 1) // Scroll up if on leftmost col
        {
            scrollby = 1;
        }
        else
        {
            return;
        }
    }

    // Another check
    if (scrollby == 0)
    {
        return;
    }

    // Get number of pixels to scroll by
    // For main panel, I guess I'll just hardcode it
    lv_obj_scroll_by_bounded(btnwrapper, 0, MAINPANELBTNHEIGHT * scrollby, LV_ANIM_ON);
}

/*  Callback for scrolling btnmatrix left panel
    Simplified from ui_event_scroll_mainpanel because there's only ever one column */
void ui_event_scroll_leftpanel(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_KEY)
    {
        return;
    }

    int16_t scrollby; // -1 to scroll down, 1 to scroll up
    uint32_t key;
    key = lv_event_get_key(e);

    if (key != LV_KEY_UP && key != LV_KEY_DOWN && key != LV_KEY_RIGHT && key != LV_KEY_LEFT) // Not the keys we are interested in
    {
        return;
    }

    // Get buttonmatrix that was triggered
    lv_obj_t *btnmatobj = lv_event_get_target(e);
    LV_ASSERT_OBJ(btnmat, lv_btnmatrix_t);
    lv_btnmatrix_t *btnm = (lv_btnmatrix_t *)btnmatobj;

    // Get its parent wrapper
    lv_obj_t *btnwrapper = lv_obj_get_parent(btnmatobj);

    // Get current button that's selected
    uint16_t btnid = btnm->btn_id_sel;

    // Do general checks
    // Since there's only one column, down or right will go down, up or left will go up
    if (key == LV_KEY_DOWN || key == LV_KEY_RIGHT) // Down
    {
        scrollby = -1;
    }
    else if (key == LV_KEY_UP || key == LV_KEY_LEFT) // Up
    {
        scrollby = 1;
    }

    // Another check
    if (scrollby == 0)
    {
        return;
    }

    // Get number of pixels to scroll by
    // For main panel, I guess I'll just hardcode it
    lv_obj_scroll_by_bounded(btnwrapper, 0, LEFTPANELBTNHEIGHT * scrollby, LV_ANIM_ON);
}

/*  Function to create mainpanels */
void ui_initmainpanel(uint16_t ID)
{
    if (ID > 7){return;};

    // Create btnmat wrapper
    ui_mainpanel_wrapper = lv_obj_create(ui_main_screen);
    lv_obj_set_size(ui_mainpanel_wrapper, lv_pct(75), lv_pct(75));
    lv_obj_set_x(ui_mainpanel_wrapper, lv_pct(-2));
    lv_obj_set_y(ui_mainpanel_wrapper, lv_pct(-2));
    lv_obj_set_align(ui_mainpanel_wrapper, LV_ALIGN_BOTTOM_RIGHT);
    lv_obj_add_style(ui_mainpanel_wrapper, &ui_mainpanel_btnmatrix_wrapperstyle, 0);

    // Create btnmat
    ui_mainpanel_btnmatrix = lv_btnmatrix_create(ui_mainpanel_wrapper);

    // Get and set char map, and set relevant bg color styles
    if (ID == 0) // Pronouns
    {
        static const char *btnmap[] = {"I", "You", "\n",
                                       "He", "She", "\n",
                                       "We", "They", "\n",
                                       "It", "One", 
                                       ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_BLUE, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_BLUE, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_BLUE, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_BLUE, LV_PART_ITEMS | LV_STATE_PRESSED);
    }
    else if (ID == 1) // Questions
    {
        static const char *btnmap[] = {"How", "What", "\n",
                                       "When", "Where", "\n",
                                       "Why", "Who",
                                       ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_GREEN, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_GREEN, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_GREEN, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_GREEN, LV_PART_ITEMS | LV_STATE_PRESSED);
    }
    else if (ID == 2) // Verbs
    {
        static const char *btnmap[] = {"Hear", "See", "\n",
                                       "Smell", "Taste", "\n",
                                       "Touch", "Feel", "\n",
                                       "Drink", "Eat", "\n",
                                       "Shower", "Sleep", "\n",
                                       "Stand", "Sit", "\n",
                                       "Open", "Close", 
                                       ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_RED, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_RED, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_RED, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_RED, LV_PART_ITEMS | LV_STATE_PRESSED);
    }
    else if (ID == 3) // Time
    {
        static const char *btnmap[] = {"Now", "Just Now", "\n",
                                       "Earlier", "Later", "\n",
                                       "Morning", "Afternoon", "\n",
                                       "Evening", "Night", "\n",
                                       "Today", "Yesterday", "\n",
                                       "Tomorrow", "Next Time",
                                       ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_CYAN, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_CYAN, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_CYAN, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_CYAN, LV_PART_ITEMS | LV_STATE_PRESSED);
    }
    else if (ID == 4) // Places
    {
        static const char *btnmap[] = {"Bathroom", "Bedroom", "\n",
                                       "Dining\nRoom", "Living\nRoom", "\n",
                                       "Kitchen", "Toilet", "\n",
                                       "Hospital", "Market", "\n",
                                       "Restaurant", "School",
                                       ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_YELLOW, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_YELLOW, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_YELLOW, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_YELLOW, LV_PART_ITEMS | LV_STATE_PRESSED);
    }
    else if (ID == 5) // Food
    {
        static const char *btnmap[] = {"Rice", "Noodles", "\n",
                                       "Kuey\nTeow", "Beehoon", "\n",
                                       "Porridge", "Soup", "\n",
                                       "Chicken", "Beef", "\n",
                                       "Duck", "Pork", "\n", 
                                       "Fish", "Prawns", "\n",
                                       "Cabbage", "Brocolli", "\n",
                                       "Kailan", "Lettuce", "\n",
                                       "Mushrooms", "Seaweed", "\n",
                                       "Apple", "Orange", "\n",
                                       "Banana", "Watermelon",
                                       ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_PINK, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_PINK, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_PINK, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_PINK, LV_PART_ITEMS | LV_STATE_PRESSED);
    }
    else if (ID == 6) // Feelings
    {
        static const char *btnmap[] = {"Happy", "Excited", "\n",
                                       "Loved", "Angry", "\n",
                                       "Sad", "Confused", "\n",
                                       "Shocked", "Scared", "\n",
                                       "Shy", "Disgusted", "\n",
                                       "Hungry", "Full", "\n",
                                       "Tired", "Energetic",
                                       ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_BROWN, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_BROWN, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_BROWN, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_BROWN, LV_PART_ITEMS | LV_STATE_PRESSED);
    } 
    else if (ID == 7) // Adjectives
    {
        static const char *btnmap[] = {"Hot", "Cold", "\n",
                                        "Big", "Small", "\n",
                                        "Sweet", "Salty", "\n",
                                        "Sour", "Bitter", "\n",
                                        "High", "Low", "\n",
                                        "Bright", "Dark", "\n",
                                        "Heavy", "Light", "\n",
                                        "Difficult", "Easy", "\n",
                                        "Near", "Far",
                                        ""};
        lv_btnmatrix_set_map(ui_mainpanel_btnmatrix, btnmap);

        // Wrapper bg properties
        lv_obj_set_style_bg_color(ui_mainpanel_wrapper, LIGHT_GREY, 0);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, DARK_GREY, LV_PART_ITEMS | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_GREY, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        lv_obj_set_style_bg_color(ui_mainpanel_btnmatrix, LIGHT_GREY, LV_PART_ITEMS | LV_STATE_PRESSED);
    }

    // Apply controls
    lv_btnmatrix_set_btn_ctrl_all(ui_mainpanel_btnmatrix, LV_BTNMATRIX_CTRL_NO_REPEAT);
    lv_btnmatrix_set_one_checked(ui_mainpanel_btnmatrix, true);

    // Apply buttonmatrix styles
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, 0);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_mainstyle, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_btndefstyle, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(ui_mainpanel_btnmatrix, &ui_mainpanel_btnmatrix_btnselstyle, LV_PART_ITEMS | LV_STATE_PRESSED);

    // Set position
    lv_obj_set_width(ui_mainpanel_btnmatrix, lv_pct(100));
    lv_obj_set_height(ui_mainpanel_btnmatrix, calc_btnmatrix_height(ui_mainpanel_btnmatrix, MAINPANELBTNHEIGHT));

    // Set scrollbar settings
    lv_obj_set_scrollbar_mode(ui_mainpanel_wrapper, LV_SCROLLBAR_MODE_ACTIVE); // Disable scrollbar showing up on screen
    lv_obj_clear_flag(ui_mainpanel_wrapper, LV_OBJ_FLAG_SCROLL_MOMENTUM);      // Disable scrollbar momentum
    lv_obj_clear_flag(ui_mainpanel_wrapper, LV_OBJ_FLAG_SCROLL_ELASTIC);       // Disable scrollbar elasticity
    lv_obj_add_flag(ui_mainpanel_wrapper, LV_OBJ_FLAG_SCROLL_ON_FOCUS);        // Autoscroll

    // Add callbacks
    lv_obj_add_event_cb(ui_mainpanel_btnmatrix, ui_event_mainpanel_esc, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_mainpanel_btnmatrix, ui_event_scroll_mainpanel, LV_EVENT_ALL, NULL);

    // Add to group
    lv_group_add_obj(ui_grp_mp_btnmatrix, ui_mainpanel_btnmatrix);
}
