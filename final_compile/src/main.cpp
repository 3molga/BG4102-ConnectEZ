/*  MAIN FILE TO RUN CONNECTEZ GUI AND EVERYTHING
    VER 0.0.1
    by thad (and yeah pretty much only me :^)
*/

/* ---------------------------------------------------------------------------------
                              SET UP VARIABLES AND OBJECTS
   ---------------------------------------------------------------------------------*/
#include <vector>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Calibrated.h>
#include <lvgl.h>
#include <ui.h>
#include <joystick.h>
#include <joystickAxis.h>
#include <ezButton.h>

// Define input/output pins and create their objects
#define TS_CS_PIN 10
joystick joystick_dev(4, 5, 1); // X pin, Y pin, buffer use bool
ezButton button_sel(15);
ezButton button_esc(16);

// Define LVGL/TFT_eSPI variables and declare objects/functions
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;
static lv_color_t buf1[screenHeight * 10];
static lv_color_t buf2[screenHeight * 10];
static uint8_t const SCREEN_ROTATION = 3;

TFT_eSPI lcd = TFT_eSPI();
XPT2046_Calibrated ts(TS_CS_PIN);

lv_indev_t *indev_joystick;
lv_indev_t *indev_button;
static lv_disp_draw_buf_t draw_buf;

// Declare LVGL/TFT_eSPI functions
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void joystick_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void button_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void button_read_esc(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void touchscreen_cal(XPT2046_Calibrated &touchscreen);
void lvgl_init_functional_objects();

uint8_t debugCount;

/* ---------------------------------------------------------------------------------
                                      LOOPS
   ---------------------------------------------------------------------------------*/

void setup()
{
  Serial.begin(9600);

  // Init LCD screen
  lcd.begin();
  lcd.setRotation(3);

  // Init touchscreen
  ts.begin();
  ts.setRotation(3);
  touchscreen_cal(ts);

  // Init joystick
  joystick_dev.setup();

  // Init buttons
  button_sel.setDebounceTime(50);
  button_esc.setDebounceTime(50);

  // Init LVGL functional elements in general
  lvgl_init_functional_objects();

  // Init UI elements
  ui_init();
}

void loop()
{
  // Read joystick inputs
  joystick_dev.stateTrigger();

  // Read button inputs
  button_sel.loop();
  button_esc.loop();

  // Debugging
  /*
  if (button_sel.isPressed())
  {
    Serial.println("Button pressed");
  }
  else if (button_sel.isReleased())
  {
    Serial.println("Button released");
  }
  */

  // Call LVGL to do its thing
  lv_timer_handler();
  delay(5); // 5 ms shouldn't lead to missing any inputs right? RIGHT?
}

/* ---------------------------------------------------------------------------------
                                      FUNCTIONS
   ---------------------------------------------------------------------------------*/
/*  Display flush function
    Don't ever, EVER touch this function. It works. */
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.pushColors((uint16_t *)&color_p->full, w * h, true);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

/*  Touchpad callback to read the touchpad
    Slightly re-written to fit the different driver type */
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (!ts.touched())
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Get the coordinates*/
    uint16_t touchX, touchY;
    uint8_t touchZ;
    ts.readData(&touchX, &touchY, &touchZ);

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

/*  Joystick callback to read the joystick buffer
    Based on a LVGL keypad, with 4 keys that map to up/down/left/right. */
void joystick_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  // Read the earliest element of the buffer
  uint8_t bufferElement;
  joystick_dev.returnBufferElement(&bufferElement);

  // Set up
  static uint8_t last_key_joystick = 0;
  uint8_t action = 0;
  data->continue_reading = 1;

  // If joystick is triggered, get earliest user input
  if (bufferElement)
  {
    data->state = LV_INDEV_STATE_PR;

    /*  Debugging:
    Serial.printf("Count %04d: %01d \n", debugCount, bufferElement);
    debugCount++;
    */

    // Use if else instead of switch case cuz the latter messes up some uint8_t logic somewhere
    if (bufferElement == 1)
    {
      action = 20;
    }
    else if (bufferElement == 2)
    {
      action = 19;
    }
    else if (bufferElement == 3)
    {
      action = 17;
    }
    else if (bufferElement == 4)
    {
      action = 18;
    }

    // Clear said buffer element
    joystick_dev.clearBufferElement();
    last_key_joystick = action;

    /* Debugging */
    Serial.printf("Action: %02d \n", action);
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
    data->continue_reading = 0;
  }

  data->key = last_key_joystick;
}

/*  Button callback to read ezButton objects
    Similar to joystick_read, but with only one key
    Assume this one callback is for both sel and esc buttons
    Based on counts instead of isPressed() due to consistency issues */
void button_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  // Static variables
  static uint8_t last_key_btn = 0; 
  static unsigned long last_count_sel = 0;
  static unsigned long last_count_esc = 0;

  if (button_sel.getCount() > last_count_sel)
  {
    data->state = LV_INDEV_STATE_PR;
    last_key_btn = LV_KEY_ENTER;
    last_count_sel = button_sel.getCount();

    // Debugging
    Serial.printf("Action registered: select \n");
  }
  else if (button_esc.getCount() > last_count_esc)
  {
    data->state = LV_INDEV_STATE_PR;
    last_key_btn = LV_KEY_ESC;
    last_count_esc = button_esc.getCount();

    // Debugging
    Serial.printf("Action registered: escape \n");
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }

  // Assign key
  data->key = last_key_btn;
}

/*  Touchscreen calibration function
    Just calibrates the touchpad based on values I got from manual calibration */
void touchscreen_cal(XPT2046_Calibrated &touchscreen)
{
  enum class PointID
  {
    NONE = -1,
    A,
    B,
    C,
    COUNT
  };

  static TS_Point _touchPoint[] = {
      TS_Point(3800, 3677), // point A
      TS_Point(350, 2169),  // point B 300
      TS_Point(1970, 646),  // point C
  };

  static TS_Point _screenPoint[] = {
      TS_Point(13, 11),   // point A
      TS_Point(312, 113), // point B
      TS_Point(167, 214)  // point C
  };

  static TS_Calibration cal(
      _screenPoint[(int)PointID::A], _touchPoint[(int)PointID::A],
      _screenPoint[(int)PointID::B], _touchPoint[(int)PointID::B],
      _screenPoint[(int)PointID::C], _touchPoint[(int)PointID::C],
      screenWidth,
      screenHeight);

  touchscreen.calibrate(cal);
}

/*  Initializing function for all LVGL functional objects
    ie. displays and input devices
    Pretty much just a wrapper for all the init functions
    Add them here instead of in the main loop for A E S T H E T I C S */
void lvgl_init_functional_objects()
{
  // Init general LVGL stuff
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenHeight * 10);

  // Init LV display
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = display_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Touchpad device
  static lv_indev_drv_t indev_touchpad;
  lv_indev_drv_init(&indev_touchpad);
  indev_touchpad.type = LV_INDEV_TYPE_POINTER;
  indev_touchpad.read_cb = touchpad_read;
  lv_indev_drv_register(&indev_touchpad);

  // Joystick device
  static lv_indev_drv_t indev_joystick_drv;
  lv_indev_drv_init(&indev_joystick_drv);
  indev_joystick_drv.type = LV_INDEV_TYPE_KEYPAD;
  indev_joystick_drv.read_cb = joystick_read;
  indev_joystick = lv_indev_drv_register(&indev_joystick_drv);

  // Button device (for both sel and esc buttons)
  static lv_indev_drv_t indev_button_drv;
  lv_indev_drv_init(&indev_button_drv);
  indev_button_drv.type = LV_INDEV_TYPE_KEYPAD;
  indev_button_drv.read_cb = button_read;
  indev_button = lv_indev_drv_register(&indev_button_drv);
}
