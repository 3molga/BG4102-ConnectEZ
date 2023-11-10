/*  MAIN FILE TO RUN CONNECTEZ GUI AND EVERYTHING
    VER 0.0.1
    by thad
*/

/* ---------------------------------------------------------------------------------
                              SET UP VARIABLES AND OBJECTS
   ---------------------------------------------------------------------------------*/
#include <vector>
#include <string>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Calibrated.h>
#include <lvgl.h>
#include <ui.h>
#include <joystick.h>
#include <joystickAxis.h>
#include <ezButton.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <telebot.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <googlefirebase.h>

// Define input/output pins and create their objects
#define TS_CS_PIN 10
#define ONBOARD_LED 38
#define ACCEL_SDA 1
#define ACCEL_SCL 2
joystick joystick_dev(4, 5, 1); // X pin, Y pin, buffer use bool
ezButton button_sel(15);
ezButton button_esc(16);
ezButton button_tele(17);

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

// WiFi, Telegram and Firebase Setup
const char *ssid = "tmt";                                         // Wi-Fi ID
const char *password = "yeah8913";                                // Wi-Fi password
#define BOTtoken "6606670486:AAGP24iLfP047ysZ7yaAUHyv3cOf-iLKMqE" // Telegram bot API token
#define CHAT_ID "-4016407865"                                     // "Chat with esp32" telegram group ID
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
telebot botController(bot, CHAT_ID);
std::string format_sentence();
std::string get_raw_input();

#define API_KEY "AIzaSyC9IZqH7onhkDCxWYMoz4Hb_ZWK1eQJ7YM" // Insert Firebase project API Key
#define USER_EMAIL "zaneyong00@gmail.com"                 // Insert Authorized Email and Corresponding Password
#define USER_PASSWORD "zane1234"
#define DATABASE_URL "https://connectez-87c05-default-rtdb.asia-southeast1.firebasedatabase.app/" // Insert RTDB URLefine the RTDB URL
googlefirebase fb;

// Declare functions for multitasking loops
void TeleHandler(void *pvParameters);
TaskHandle_t TeleTask;

uint8_t debugCount;

/* ---------------------------------------------------------------------------------
                                      LOOPS
   ---------------------------------------------------------------------------------*/

void setup()
{
  Serial.begin(115200);

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
  button_tele.setDebounceTime(50);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (millis() < 5000)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WiFi connected");
      botController.queueMessage(std::string("Hello everyone, I am connected!\nUse /start for commands to control me!"));
      break;
    }
    else
    {
      delay(500);
      Serial.print(".");
    }
  }

  /*
    fb.setup(API_KEY, USER_EMAIL, USER_PASSWORD, DATABASE_URL);
    Serial.printf("Connected to Google Firebase at %s", DATABASE_URL);
    fb.begin(ACCEL_SDA, ACCEL_SCL);
    Serial.printf("Accelerometer Initialized");

  */

  // Init LVGL functional elements in general
  lvgl_init_functional_objects();
  ui_init();

  // Set up parallel processing task to handle Telegram reading
  xTaskCreatePinnedToCore(
      TeleHandler,            // Task function.
      "Telegram Bot Handler", // name of task.
      100000,                 // Stack size of task
      NULL,                   // parameter of the task
      0,                      // priority of the task
      &TeleTask,              // Task handle to keep track of created task
      0);                     // pin task to core 0
}

// Main loop to handle LVGL stuff
void loop()
{
  static uint16_t telepresscount;

  // Read joystick inputs
  joystick_dev.stateTrigger();

  // Read button inputs
  button_sel.loop();
  button_esc.loop();
  button_tele.loop();

  if (button_tele.getCount() > telepresscount)
  {
    if (user_input_struct.num_words > 0)
    {
      std::string message = format_sentence();
      std::string raw = get_raw_input();
      botController.queueNewUserInput(message, raw);
    }
  }

  telepresscount = button_tele.getCount();

  // Call LVGL to do its thing
  lv_timer_handler();
  delay(5); // 5 ms shouldn't lead to missing any inputs right? RIGHT?
}

// 2nd loop to handle Telegram operations
void TeleHandler(void *pvParameters)
{
  while (1)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
    }
    else
    {
      // Wi-Fi is connected
      botController.handleActiveUpdates();
      botController.handlePassiveUpdates();
    }
  }
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

/*  Function to format words into sentences
    Based on 2 things - left panel ID (aka the context of the word), and main panel word (the word itself)
    Assembles the formatted sentence itself */
std::string format_sentence()
{
  // Init temp variables
  std::string sentence;
  bool isQuestion = 0;
  bool hasPronouns = 0;
  bool hasNouns = 0;
  bool hasVerb = 0;
  bool hasTimes = 0;
  bool hasAdj = 0;
  bool hasFeel = 0;

  // Warning: messy algo ahead
  // Start by finding words of 7 types: pronouns, nouns, verbs, feelings, adjectives, questions, time
  // Init vectors for - pronouns, nouns, verbs, adjectives, questions, time
  std::vector<std::string> pronouns;
  std::vector<std::string> nouns;
  std::vector<std::string> verbs;
  std::vector<std::string> feelings;
  std::vector<std::string> adjectives;
  std::vector<std::string> questions;
  std::vector<std::string> times;

  // Fill the aforementioned vectors with the words with corresponding "type"
  for (uint16_t i = 0; i < user_input_struct.num_words; i++)
  {
    std::string temp_string;
    temp_string = user_input_struct.mp_array_words[i];

    if (user_input_struct.lp_array_ID[i] == 0) // Pronouns
    {
      pronouns.push_back(temp_string);
      hasPronouns = 1;
    }
    else if (user_input_struct.lp_array_ID[i] == 4 || user_input_struct.lp_array_ID[i] == 5) // Nouns
    {
      nouns.push_back(temp_string);
      hasNouns = 1;
    }
    else if (user_input_struct.lp_array_ID[i] == 7) // Adjectives
    {
      adjectives.push_back(temp_string);
      hasAdj = 1;
    }
    else if (user_input_struct.lp_array_ID[i] == 6) // Feelings
    {
      feelings.push_back(temp_string);
      hasFeel = 1;
    }
    else if (user_input_struct.lp_array_ID[i] == 2) // Verbs
    {
      verbs.push_back(temp_string);
      hasVerb = 1;
    }
    else if (user_input_struct.lp_array_ID[i] == 1) // Questions
    {
      questions.push_back(temp_string);
      isQuestion = 1;
    }
    else if (user_input_struct.lp_array_ID[i] == 3) // Times
    {
      times.push_back(temp_string);
      hasTimes = 1;
    }
  }

  // Assemble the sentence
  // If there's a question included, it will always be at the start
  if (isQuestion)
  {
    sentence.append(questions.front());
    sentence.append(" ");
  }

  // TO DO: define behaviour
  // Weird if-else to set subject of sentence
  if (hasPronouns && !hasNouns) // If there are only pronouns
  {
    sentence.append(pronouns.front());
    sentence.append(" ");
  }
  else if (hasNouns && !hasPronouns) // If there are only nouns
  {
    sentence.append(nouns.front());
    sentence.append(" ");
  }
  else if (hasPronouns && hasNouns) // If there are both, go with pronoun then noun (modify pronoun to show possessive)
  {
    // This is going to be ugly
    std::string tempPronoun = pronouns.front();
    if (tempPronoun == "I")
    {
      sentence.append("My");
    }
    else if (tempPronoun == "You")
    {
      sentence.append("Your");
    }
    else if (tempPronoun == "He")
    {
      sentence.append("His");
    }
    else if (tempPronoun == "She")
    {
      sentence.append("Her");
    }
    else if (tempPronoun == "We")
    {
      sentence.append("Our");
    }
    else if (tempPronoun == "They")
    {
      sentence.append("Their");
    }
    else if (tempPronoun == "It")
    {
      sentence.append("Its");
    }
    else if (tempPronoun == "One")
    {
      sentence.append("One");
    }
    sentence.append(" ");
    sentence.append(nouns.front());
    sentence.append(" ");
  }

  // Add verbs
  if (hasVerb)
  {
    sentence.append(verbs.front());
    sentence.append (" ");
  }

  // Add feelings
  if (hasFeel)
  {
    sentence.append("feels");
    sentence.append(feelings.front());
    sentence.append(" ");
  }

  // Add adjectives
  if (hasAdj)
  {
    sentence.append(adjectives.front());
    sentence.append("-ly "); // I would like to apologize to all my English teachers
  }

  // If there's a time included, it will always be at the end
  if (hasTimes)
  {
    sentence.append(" in the ");
    sentence.append(times.front());
    sentence.append(" ");
  }

  // Add punctuation
  if (isQuestion)
  {
    sentence.pop_back();
    sentence = sentence + "?";
  }
  else
  {
    sentence.pop_back();
    sentence = sentence + ".";
  }

  return sentence;
}

//  Function to get raw user input in the form of an std::string
std::string get_raw_input()
{
  std::string raw;
  raw = user_input_struct.mp_array_sentence;
  return raw;
}




