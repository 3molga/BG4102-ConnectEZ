/* Test file to try running the UI
TESTING TFT_eSPI LIBRARY*/
#define LV_CONF_SKIP

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Calibrated.h>
#include <lvgl.h>
#include <ui.h>

#define TS_CS_PIN 5

static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[screenHeight * 10];
static lv_color_t buf2[screenHeight * 10];

void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

enum class PointID { NONE = -1, A, B, C, COUNT };

static TS_Point _touchPoint[] = {
  TS_Point(3800, 3677), // point A
  TS_Point( 350, 2169), // point B 300
  TS_Point(1970,  646), // point C
};

static TS_Point _screenPoint[] = {
  TS_Point( 13,  11), // point A
  TS_Point(312, 113), // point B
  TS_Point(167, 214)  // point C
};

static TS_Calibration cal(
    _screenPoint[(int)PointID::A], _touchPoint[(int)PointID::A],
    _screenPoint[(int)PointID::B], _touchPoint[(int)PointID::B],
    _screenPoint[(int)PointID::C], _touchPoint[(int)PointID::C],
    screenWidth,
    screenHeight
);
TFT_eSPI lcd = TFT_eSPI();
XPT2046_Calibrated ts(TS_CS_PIN);
static uint8_t  const SCREEN_ROTATION = 3;

void setup()
{
  Serial.begin(9600);

  // Init LCD screen
  lcd.begin();
  lcd.setRotation(3);
  //lcd.fillScreen(TFT_BLACK); // debugging uwu

  // Init touchscreen
  ts.begin();
  ts.setRotation(3);
  ts.calibrate(cal);

  // Init all LVGL stuff
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenHeight * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = display_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();
  
}

void loop()
{
  lv_timer_handler(); 
  delay(5); 
}

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

/*** Touchpad callback to read the touchpad
 * Slightly re-written to fit the different driver type ***/
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

    Serial.printf("Touch (x,y): (%03d,%03d)\n",touchX,touchY );
  }
}