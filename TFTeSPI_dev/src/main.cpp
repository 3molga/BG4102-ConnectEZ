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
#define TS_CS_PIN 5
joystick joystick_dev(13, 12, 1); // X pin, Y pin, buffer use bool
ezButton button_sel(27);
ezButton button_esc(28);

// Define LVGL/TFT_eSPI variables and objects
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screenHeight * 10];
static lv_color_t buf2[screenHeight * 10];
static uint8_t const SCREEN_ROTATION = 3;

TFT_eSPI lcd = TFT_eSPI();
XPT2046_Calibrated ts(TS_CS_PIN);

lv_indev_t *indev_joystick;
lv_indev_t *indev_buttonsel;
lv_indev_t *indev_buttonret;

// Define LVGL/TFT_eSPI functions
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void joystick_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void button_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void button_read_esc(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void touchscreen_cal(XPT2046_Calibrated &touchscreen);

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

  // Init all LVGL stuff
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
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
    data->continue_reading = 0;
  }

  data->key = action;

  /*  Debugging:
  if (action)
  {
    Serial.printf("Action: %02d \n", action);
  }
  */
}

/*  Button callback to read ezButton objects
    Similar to joystick_read, but with only one key
    Assume this one callback is for both sel and esc buttons 
    Also assume that if both buttons are pressed at the same time, nothing happens? */
void button_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint8_t action = 0;

  if (button_sel.isPressed() && button_esc.isReleased())
  {
    data->state = LV_INDEV_STATE_PR;
    action = LV_KEY_ENTER;
    data->key = action;
  }
  else if (button_esc.isPressed() && button_sel.isReleased())
  {
    data->state = LV_INDEV_STATE_PR;
    action = LV_KEY_ESC;
    data->key = action;
  }
  else 
  {
    data->state = LV_INDEV_STATE_REL;
  }
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