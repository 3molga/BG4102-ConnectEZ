#ifndef _UI_TEST_UI_H
#define _UI_TEST_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#define MAX_USER_INPUTS 5

#include <Arduino.h>
#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"

  // Global-ish UI objects and functions
  extern lv_indev_t *indev_joystick;
  extern lv_indev_t *indev_button;

  void ui_init(void);
  void init_styles();
  bool check_inputs_sel(lv_event_t *event);
  bool check_inputs_del(lv_event_t *event);

  // SCREEN: ui_start_screen
  void ui_start_screen_init(void);
  void ui_start_screen_setindev(lv_group_t *group);
  void ui_event_init_button(lv_event_t *e);

  extern lv_obj_t *ui_start_screen;
  extern lv_obj_t *ui_bg;
  extern lv_obj_t *ui_mainlogo;
  extern lv_obj_t *ui_development_label;
  extern lv_obj_t *ui_init_button;
  extern lv_obj_t *ui_welcome_text;
  extern lv_group_t *ui_grp_init_button;

  extern lv_style_t ui_btndefstyle;
  extern lv_style_t ui_btnselstyle;

  // SCREEN: ui_main_screen
  void ui_main_screen_init(void);
  void ui_main_screen_setindev(lv_group_t *group, lv_obj_t *btnmatrix);
  void ui_initmainpanel(uint16_t leftpanel_btnid);

  void ui_event_returntostart(lv_event_t *e);
  void ui_event_mainpanel_sel(lv_event_t *e);
  void ui_event_mainpanel_esc(lv_event_t *e);
  void ui_event_leftpanel_sel(lv_event_t *e);
  void ui_event_leftpanel_esc(lv_event_t *e);
  void ui_event_scroll_mainpanel(lv_event_t *e);
  void ui_event_scroll_leftpanel(lv_event_t *e);

  extern lv_obj_t *ui_main_screen;
  extern lv_obj_t *ui_returntostart;
  extern lv_obj_t *ui_mainpanel;
  extern lv_obj_t *ui_leftpanel;
  extern lv_obj_t *ui_worddisplay;

  // Button matrix stuff
  extern lv_obj_t *ui_mainpanel_wrapper;
  extern lv_obj_t *ui_mainpanel_btnmatrix;

  extern lv_obj_t *ui_leftpanel_wrapper;
  extern lv_obj_t *ui_leftpanel_btnmatrix;

  extern lv_group_t *ui_grp_mp_btnmatrix;
  extern lv_group_t *ui_grp_lp_btnmatrix;

  extern lv_style_t ui_mainpanel_btnmatrix_wrapperstyle;
  extern lv_style_t ui_mainpanel_btnmatrix_mainstyle;
  extern lv_style_t ui_mainpanel_btnmatrix_btndefstyle;
  extern lv_style_t ui_mainpanel_btnmatrix_btnselstyle;
  extern lv_style_t ui_leftpanel_btnmatrix_btndefstyle;
  extern lv_style_t ui_leftpanel_btnmatrix_btnselstyle;

  extern lv_style_t ui_worddisplay_style;

  // Utility functions
  extern lv_coord_t calc_btnmatrix_height(lv_obj_t *btnmatrixobj, lv_coord_t rowheight);
  extern void assign_inputs(uint16_t mp_ID, uint16_t lp_ID, char *word);
  extern char* remove_newlines(char *word);

  // Struct to store words
  // Assume a maximum of 5 words that the user can input for now?
  struct words_input
  {
    uint16_t lp_array_ID[MAX_USER_INPUTS];
    uint16_t mp_array_ID[MAX_USER_INPUTS];
    uint16_t lp_array_last_ID;
    uint16_t num_words;

    char *mp_array_words[MAX_USER_INPUTS];
  };
  struct words_input user_input_struct;
  extern char* generate_worddisplay();

  LV_IMG_DECLARE(ui_img_1_bg_png); // assets\1_bg.png
  LV_IMG_DECLARE(ui_img_logo_png); // assets\logo.png

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
