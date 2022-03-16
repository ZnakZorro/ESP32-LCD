/*
    Simple Demo with WT32-SC01 + LovyanGFX + LVGL8.x
*/
#define LGFX_AUTODETECT // Autodetect board
#define LGFX_WT32_SC01
#define LGFX_USE_V1     // set to use new version of library
//#define LV_CONF_INCLUDE_SIMPLE

/* Uncomment below line to draw on screen with touch */
#define DRAW_ON_SCREEN

#include <LovyanGFX.hpp> // main library
static LGFX lcd; // declare display variable

#include <lvgl.h>
//#include "lv_conf.h"
/*** Setup screen resolution for LVGL ***/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

// Variables for touch x,y
#ifdef DRAW_ON_SCREEN
static int32_t x, y;
#endif

/*** Function declaration ***/
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void lv_button_demo(void);

void setup(void)
{

  Serial.begin(115200); /* prepare for possible serial debug */

  lcd.init(); // Initialize LovyanGFX
  lv_init();  // Initialize lvgl

  // Setting display to landscape
  if (lcd.width() < lcd.height())
    lcd.setRotation(lcd.getRotation() ^ 1);

  /* LVGL : Setting up buffer to use for display */
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  /*** LVGL : Setup & Initialize the display device driver ***/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = display_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*** LVGL : Setup & Initialize the input device driver ***/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  lv_indev_drv_register(&indev_drv);

  /*** Create simple label and show LVGL version ***/
  String LVGL_Arduino = "WT32-SC01 with LVGL ";
  LVGL_Arduino += String('v') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  lv_obj_t *label = lv_label_create(lv_scr_act()); // full screen as the parent
  lv_label_set_text(label, LVGL_Arduino.c_str());  // set label text
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);      // Center but 20 from the top

  lv_button_demo();
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);

#ifdef DRAW_ON_SCREEN
  /*** Draw on screen with touch ***/
  if (lcd.getTouch(&x, &y))
  {
    lcd.fillRect(x - 2, y - 2, 5, 5, TFT_RED);
    lcd.setCursor(380, 0);
    lcd.printf("Touch:(%03d,%03d)", x, y);
  }
#endif
} // EOF loop

  /*** Display callback to flush the buffer to screen ***/
  void display_flush(lv_disp_drv_t * disp, const lv_area_t *area, lv_color_t *color_p)
  {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.pushColors((uint16_t *)&color_p->full, w * h, true);
    lcd.endWrite();

    lv_disp_flush_ready(disp);
  }

  /*** Touchpad callback to read the touchpad ***/
  void touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
  {
    uint16_t touchX, touchY;
    bool touched = lcd.getTouch(&touchX, &touchY);

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

      // Serial.printf("Touch (x,y): (%03d,%03d)\n",touchX,touchY );
    }
  }

  /* Counter button event handler */
  static void counter_event_handler(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
      static uint8_t cnt = 0;
      cnt++;

      /*Get the first child of the button which is the label and change its text*/
      lv_obj_t *label = lv_obj_get_child(btn, 0);
      lv_label_set_text_fmt(label, "ESC: %d", cnt);
      LV_LOG_USER("LV_Clicked");
      Serial.println("Clicked");

            //qqqqqq
            lcd.setCursor(5, 0);
            //lcd.printf("Click:(%02d)", cnt);
            lcd.printf("Click:(%d)", cnt);
              /* 
              String LVGL_Arduino = "             KLIK           ";
              LVGL_Arduino += String(cnt);
              lv_obj_t *labelx = lv_label_create(lv_scr_act()); // full screen as the parent
              lv_label_set_text(labelx, LVGL_Arduino.c_str());  // set label text
              lv_obj_align(labelx, LV_ALIGN_TOP_MID, 0, 20);      // Center but 20 from the top
              */
            //qqqqqqqqqqqq


    }
  }

  static void toggle_event_handler_restart(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
      LV_LOG_USER("restart");
      Serial.println("restart");
      ESP.restart();
    }
  }  
  


  /* Toggle button event handler */
  static void toggle_event_handler(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
      LV_LOG_USER("LV_Toggled");
      Serial.println("Toggled");
    }
  }
 /* Toggle button3 event handler */
  static void toggle_event_handler3(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    //Serial.println(code);
    //Serial.println(LV_EVENT_VALUE_CHANGED);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
      LV_LOG_USER("LV_Togg3");
      Serial.println("Togg3");
    }
  }

struct buttonsSTRUCT  {
    int8_t x;
    int8_t y;
    const char *labelka;
    lv_event_cb_t eventBTN;
};

buttonsSTRUCT klawisze[16]={
  0, 0, "ESC",       counter_event_handler,
  1, 0, "Salon",     toggle_event_handler,
  2, 0, "Sypialnia", toggle_event_handler3,
  3, 0, "Cartoon",   toggle_event_handler3,

  0, 1, "Vol-",      counter_event_handler,
  1, 1, "Vol+",      toggle_event_handler,
  2, 1, "Sta-",      toggle_event_handler3,
  3, 1, "Sta+",      toggle_event_handler3,

  0, 2, "Info",      counter_event_handler,
  1, 2, "reStart",    toggle_event_handler_restart,
  2, 2, "Stop",      toggle_event_handler3,
  3, 2, "Sleep",      toggle_event_handler3,

  0, 3, "TOK-FM",   counter_event_handler,
  1, 3, "RNS",      toggle_event_handler,
  2, 3, "357",      toggle_event_handler3,
  3, 3, "Classic",  toggle_event_handler3
  

};


#define buttonW  100
#define buttonH   45
#define spaceW    15
#define spaceH    12
#define lineStart 10
#define lineH     50

void createButton(int8_t x, int8_t y, const char *labelka, lv_event_cb_t eventBTN)
  {
    lv_obj_t *label;
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn, eventBTN, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_pos(btn, (spaceW*(x+1)) + (buttonW*x),  lineStart + (spaceH*(y)) + (lineH*(y+1)));   /*Set its position*/
    lv_obj_set_size(btn, buttonW, buttonH);   /*Set its size*/
    label = lv_label_create(btn);
    lv_label_set_text(label, labelka);
    lv_obj_center(label);
  }

void lv_button_demo(void)
  {
    for (int8_t i=0; i<16;i++)  {
        createButton(klawisze[i].x, klawisze[i].y, klawisze[i].labelka,klawisze[i].eventBTN);
    }
    //createButton(0, 0, "ESC",       counter_event_handler);
    //createButton(1, 0, "Salon",     toggle_event_handler);
    //createButton(2, 0, "Sypialnia", toggle_event_handler3);
    //createButton(3, 0, "Cartoon",   toggle_event_handler3);

/*
    lv_obj_t *label;

    // Button 1 with counter
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, counter_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(btn1, spaceB, lineStart + lineH);   
    lv_obj_set_size(btn1, buttonW, buttonH);   
    label = lv_label_create(btn1);
    lv_label_set_text(label, "ESC");
    lv_obj_center(label);

    // Toggle button 2
    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn2, toggle_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_pos(btn2, spaceB*2 + buttonW,  lineStart + lineH);   
    lv_obj_set_size(btn2, buttonW, buttonH);   
    label = lv_label_create(btn2);
    lv_label_set_text(label, "Klawisz2");
    lv_obj_center(label);

    // Toggle button 3
    lv_obj_t *btn3 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn3, toggle_event_handler3, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(btn3, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_pos(btn3, spaceB*3 + buttonW*2,  lineStart + lineH);   
    lv_obj_set_size(btn3, buttonW, buttonH);   
    label = lv_label_create(btn3);
    lv_label_set_text(label, "Klawisz3");
    lv_obj_center(label);

    // Toggle button 4
    lv_obj_t *btn4 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn4, toggle_event_handler3, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(btn4, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_pos(btn4, spaceB*4 + buttonW*3,  lineStart + lineH);   
    lv_obj_set_size(btn4, buttonW, buttonH);   
    label = lv_label_create(btn4);
    lv_label_set_text(label, "Klawisz4");
    lv_obj_center(label);
    //qqqqqqqqqqq
*/
  }
