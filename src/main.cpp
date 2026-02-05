// EEZ#2 data into arduino R2 - Feb 2026 using PlatformIO
// my screen Elecrow HMI ESP32 HMI 3.5"  https://www.elecrow.com/wiki/esp32-display-352727-intelligent-touch-screen-wi-fi26ble-320480-hmi-display.html
// if you you press and button on the tft and it dissappears cheack your LGVL settings around line 35
// as shown in the video #define LV_COLOR_SCREEN_TRANSP 1 //fms
// make sure serial monitor is set to 115200 No Line Ending
//
// lvgl 8.3.5
// TFT_eSPI 2.5.43
// VScode with platformio
//
// edit the User_Setup.h file in the TFT_eSPI libraries folder
// For LVGL copy the lv_cong_template.h file, paste it in to the main libraries DIR. Rename it lv_conf.h and edit it
// Put the huge-app.csv in the ROOT
//
//
//  serach for FMS within the code to find code that I have changed
//
// change line 15 from 0 to 1 to active the file, adjust colour depth if required to match your screen
// Line 49 #define LV_MEM_CUSTOM 1   //fms was 0
// Line 72 #define LV_MEM_BUF_MAX_NUM 64 //fms was 16 change if you get any errors
// Line 81 and 84 I changed both from 30 to 15
// Line 88 #define LV_TICK_CUSTOM 1
// From line 351 use a 1 to use which every font you have used in EEZ Studio - if you miss one you will get a build error telling you which one
//
// check in vars.h what Native global variables we have to use make a vars.cpp file to use them
//
// https://www.envox.eu/eez-studio-docs/widget-basic-button-eez-gui/
//
// So the workflow is:

//- Implement required functionality specific to your target device in C/C++. In your case it is: move motor left, move motor right, change motor speed, stop motor, ...

//- Implement UI in Studio using EEZ Flow for the UI logic. For all the actions that can't be implemented in EEZ Flow, use native user actions implemented in C/C++. If you
//  need to pass some data (like motor speed value) between EEZ Flow and C/C++ then use native user variables.

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"

#define LV_LVGL_H_INCLUDE_SIMPLE

/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;  // fms change your screen
static const uint16_t screenHeight = 320; // fms change your screen

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{ // this FLUSHING worked for my screen Elecrow HMI ESP32 3.5"
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{ // this standard touch worked for my screen - ILI9488 driver set yours as noted above
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY, 600);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    // Serial.print("Data x ");  // no need to keep printing these out  fms
    // Serial.println(touchX);

    // Serial.print("Data y ");
    // Serial.println(touchY);
  }
}
// when using arduion i normally put all my voids at the end but when you do that using Platformio is will not compile
// so i have moved it up here

void clearscreen()
{
  for (int i = 0; i < 10; i++)
  {
    Serial.println("\n\n\n\n");
  }
}

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */
  delay(1000);

  lv_init();

#if LV_USE_LOG != 0                   //
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin();        /* TFT init */
  tft.setRotation(1); /* FMS Landscape orientation, flipped was 3 but was 180 degree incorrect USB cord on right side*/

  /*Set the touchscreen calibration data,
     the actual data for your display can be acquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/

  // uint16_t calData[5] = { 311, 3570, 279, 3466, 7 };  // FMS use the touch calibrate in tft_espi examples to get your own values
  uint16_t calData[5] = {259, 3640, 292, 3484, 7};
  tft.setTouch(calData); // Let the screen use your touch values

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

#if 0
    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#else

#endif

  clearscreen(); // just print some blank line in the serial monitor

  delay(5000); // allows you to turn on the serial monitor if not automatic  comment out if not required
  Serial.println("Setup done, lets have some fun");
  Serial.println("");
  String LVGL_Arduino = "Using PlatformIO & LVGL "; // fms
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("");
  Serial.println("Thank You Andy @FlixMySwitch");
  Serial.println("");

  ui_init(); // Init EEZ-Studio UI
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */

  ui_tick(); // Update EEZ-Studio UI

  // Put your main code here, to run repeatedly:

  // uint32_t time_till_next = 5; // Some value

  // // Timer handler needs to be called periodically to handle the tasks of LVGL
  // // time_till_next = lv_timer_handler(); // lv_task_handler() is LVGL v8 only
  // ui_tick();

  // delay(time_till_next);
}
