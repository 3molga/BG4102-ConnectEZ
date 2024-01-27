/*  This file was originally generated from Squareline Studio
    Then heavily modified by thad to fit it's current function */

#include "ui.h"
#include "ui_helpers.h"

/* ---------------------------------------------------------------------------------
                                    VARIABLES
   ---------------------------------------------------------------------------------*/
//  Input devices
lv_indev_t *indev_joystick;
lv_indev_t *indev_button;

bool check_inputs_sel(lv_event_t *event);
bool check_inputs_del(lv_event_t *event);

//  SCREEN: ui_start_screen
void ui_start_screen_init(void);
void ui_start_screen_setindev(lv_group_t *group);
void ui_event_init_button(lv_event_t *e);

lv_obj_t *ui_start_screen;
lv_obj_t *ui_bg;
lv_obj_t *ui_mainlogo;
lv_obj_t *ui_development_label;
lv_obj_t *ui_init_button;
lv_obj_t *ui_welcome_text;
lv_group_t *ui_grp_init_button;

lv_style_t ui_btndefstyle;
lv_style_t ui_btnselstyle;

// SCREEN: ui_main_screen
void ui_main_screen_init(void);
void ui_main_screen_setindev(lv_group_t *group, lv_obj_t *btnmatrix);
void ui_event_returntostart(lv_event_t *e);
void ui_event_mainpanel_esc(lv_event_t *e);
void ui_event_leftpanel_sel(lv_event_t *e);
void ui_event_leftpanel_esc(lv_event_t *e);

lv_obj_t *ui_main_screen;
lv_obj_t *ui_returntostart;
lv_obj_t *ui_mainpanel;
lv_obj_t *ui_leftpanel;

lv_obj_t *ui_mainpanel_btnmatrix;
lv_obj_t *ui_leftpanel_btnmatrix;
lv_group_t *ui_grp_mp_btnmatrix; // Assign this dynamically
lv_group_t *ui_grp_lp_btnmatrix;

lv_style_t ui_mainpanel_btnmatrix_mainstyle;
lv_style_t ui_mainpanel_btnmatrix_btndefstyle;
lv_style_t ui_mainpanel_btnmatrix_btnselstyle;
lv_style_t ui_mainpanel_btnmatrix_btnprestyle;

/* ---------------------------------------------------------------------------------
                                    FUNCTIONS
   ---------------------------------------------------------------------------------*/
void ui_init(void)
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    init_styles();

    // For whatever reason, init-ing the screens in this order seems to work
    ui_start_screen_init();
    ui_main_screen_init();
    ui_start_screen_setindev(ui_grp_init_button);
    lv_disp_load_scr(ui_start_screen);
}

void init_styles()
{
    // Initiate styles
    // Default button style (wrt background and borders) (it'll always be focused by the keypad objects, so that's the default)
    lv_style_init(&ui_btndefstyle);
    lv_style_set_bg_color(&ui_btndefstyle, lv_color_hex(0x3044FA));
    lv_style_set_border_width(&ui_btndefstyle, 2);
    lv_style_set_border_color(&ui_btndefstyle, lv_color_hex(0x000000));
    lv_style_set_outline_width(&ui_btndefstyle, 0);
    lv_style_set_shadow_width(&ui_btndefstyle, 0);

    // Default btnmatrix background style - shouldn't ever change
    lv_style_init(&ui_mainpanel_btnmatrix_mainstyle);
    lv_style_set_bg_color(&ui_mainpanel_btnmatrix_mainstyle, lv_color_make(110, 185, 240));
    lv_style_set_border_width(&ui_mainpanel_btnmatrix_mainstyle, 2);     // Make border 2 px wide
    lv_style_set_border_color(&ui_mainpanel_btnmatrix_mainstyle, lv_color_make(0, 0, 0));
    lv_style_set_outline_width(&ui_mainpanel_btnmatrix_mainstyle, 0);    // Make outline transparent
    lv_style_set_pad_top(&ui_mainpanel_btnmatrix_mainstyle, 8);          // Set paddings 
    lv_style_set_pad_bottom(&ui_mainpanel_btnmatrix_mainstyle, 4);       // (top and left should be 4px wider than bot/right, for some reason)
    lv_style_set_pad_right(&ui_mainpanel_btnmatrix_mainstyle, 4);
    lv_style_set_pad_left(&ui_mainpanel_btnmatrix_mainstyle, 8);

    // Default btnmatrix btn style
    lv_style_init(&ui_mainpanel_btnmatrix_btndefstyle);
    lv_style_set_shadow_width(&ui_mainpanel_btnmatrix_btndefstyle, 0);                     // Remove shadow
    lv_style_set_border_width(&ui_mainpanel_btnmatrix_btndefstyle, 0);                     // Remove border
    lv_style_set_outline_width(&ui_mainpanel_btnmatrix_btndefstyle, 0);                    // Remove outline
    lv_style_set_bg_color(&ui_mainpanel_btnmatrix_btndefstyle, lv_color_make(33, 40, 63)); // Internal button color
    lv_style_set_text_color(&ui_mainpanel_btnmatrix_btndefstyle, lv_color_make(255, 255, 255));
    lv_style_set_pad_top(&ui_mainpanel_btnmatrix_btndefstyle, 0);                          // Add padding of 0 px on all sides
    lv_style_set_pad_bottom(&ui_mainpanel_btnmatrix_btndefstyle, 0);
    lv_style_set_pad_right(&ui_mainpanel_btnmatrix_btndefstyle, 0);
    lv_style_set_pad_left(&ui_mainpanel_btnmatrix_btndefstyle, 0);

    // btnmatrix btn style when "hovered"/selected by joystick
    lv_style_init(&ui_mainpanel_btnmatrix_btnselstyle);
    lv_style_set_border_width(&ui_mainpanel_btnmatrix_btnselstyle, 3);                       // Add 2px-wide border
    lv_style_set_border_color(&ui_mainpanel_btnmatrix_btnselstyle, lv_color_make(10, 0, 0)); // Add black outline
    lv_style_set_bg_color(&ui_mainpanel_btnmatrix_btnselstyle, lv_color_make(56, 69, 255));  // Make bg color lighter
    lv_style_set_outline_width(&ui_mainpanel_btnmatrix_btnselstyle, 0);
}

//  Function to combine and simplify checking for touchpad click and button_sel press
//  Nevermind, turns out the original implementation was buggy - just use CLICKED
bool check_inputs_sel(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_CLICKED)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Similar function to check_inputs_sel but for the esc button instead
bool check_inputs_del(lv_event_t *event)
{
    if (lv_event_get_code(event) == LV_EVENT_KEY && (lv_event_get_key(event) == LV_KEY_ESC))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
