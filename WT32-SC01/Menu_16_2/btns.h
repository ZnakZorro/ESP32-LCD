#include <LovyanGFX.hpp>
#include <lvgl.h>

/*** Function declaration ***/
void createButton(int8_t x, int8_t y, const char *labelka, lv_event_cb_t eventBTN);
void lv_button_alarmy(void);
void lv_button_demo(bool flaga);
static void connectWIFI();

static void alarm_event_handler(lv_event_t * e);
static void DRAW_buttons_alarmy(lv_event_t * e);

static void RadioInfo_event_handler(lv_event_t * e);
static void ESC_event_handler(lv_event_t * e);
static void counter_event_handler(lv_event_t * e);
static void toggle_event_handler(lv_event_t * e);
static void toggle_event_handler3(lv_event_t * e);
static void toggle_event_handler_restart(lv_event_t * e);
static void RadioVolPlus_event_handler(lv_event_t * e);
static void RadioVolMinus_event_handler(lv_event_t * e);
static void RadioStaPlus_event_handler(lv_event_t * e);
static void RadioStaMinus_event_handler(lv_event_t * e);
static void RadioActual_0_event_handler(lv_event_t * e);
static void RadioActual_1_event_handler(lv_event_t * e);
static void RadioActual_2_event_handler(lv_event_t * e);
static void RadioSTA_0_event_handler(lv_event_t * e);
static void RadioSTA_1_event_handler(lv_event_t * e);
static void RadioSTA_2_event_handler(lv_event_t * e);
static void RadioSTA_3_event_handler(lv_event_t * e);

void sendGET(String param);

/*** Function declaration EOF ***/
int8_t actualRadio=0;
int8_t actualButton=0;

#define buttonW  100
#define buttonH   45
#define spaceW    15
#define spaceH    12
#define lineStart 10
#define lineH     50


struct radiosSTRUCT  {
    String url;
    String name;
};

radiosSTRUCT radia[3]={
  "http://192.168.31.215/","Salon",
  "http://192.168.31.141/","JBL",
  "http://192.168.31.115/","Cartoon",
  
};


struct buttonsSTRUCT  {
    int8_t x;
    int8_t y;
    const char *labelka;
    lv_event_cb_t eventBTN;
};

buttonsSTRUCT klawisze[16]={
  0, 0, "ESC",         ESC_event_handler,
  1, 0, "[Salon]",   RadioActual_0_event_handler,
  2, 0, "[JBL]",     RadioActual_1_event_handler,
  3, 0, "[Cartoon]", RadioActual_2_event_handler,
  

  0, 1, "Vol-",      RadioVolMinus_event_handler,
  1, 1, "Vol+",      RadioVolPlus_event_handler,
  2, 1, "Sta-",      RadioStaMinus_event_handler,
  3, 1, "Sta+",      RadioStaPlus_event_handler,

  0, 2, "Info",      RadioInfo_event_handler,
  1, 2, "reStart",    toggle_event_handler_restart,
  2, 2, "Sleep",      toggle_event_handler3,
  3, 2, "Alarm",      DRAW_buttons_alarmy,

  0, 3, "TOK-FM",   RadioSTA_0_event_handler,
  1, 3, "RNS",      RadioSTA_1_event_handler,
  2, 3, "357",      RadioSTA_2_event_handler,
  3, 3, "Classic",  RadioSTA_3_event_handler,
};

buttonsSTRUCT alarmy[4]={
  0, 2, "1min",         alarm_event_handler,
  1, 2, "5min",         alarm_event_handler,
  2, 2, "15min",         alarm_event_handler,
  3, 2, "60min",         alarm_event_handler,
};

void printHeap(){
            lcd.setCursor(5, 0);
            int hp = ESP.getFreeHeap();
            Serial.print("HEAP=");Serial.println(hp);
            lcd.printf("Heap:(%d)", hp);
}

static void opiszRadio(){
  Serial.print("actualRadio=");Serial.println(actualRadio);
}

static void RadioActual_0_event_handler(lv_event_t * e) {actualRadio=0;opiszRadio();}
static void RadioActual_1_event_handler(lv_event_t * e) {actualRadio=1;opiszRadio();}
static void RadioActual_2_event_handler(lv_event_t * e) {actualRadio=2;opiszRadio();}

static void RadioSTA_0_event_handler(lv_event_t * e) {sendGET(radia[actualRadio].url+"radio?s=0");}  
static void RadioSTA_1_event_handler(lv_event_t * e) {sendGET(radia[actualRadio].url+"radio?s=1");}  
static void RadioSTA_2_event_handler(lv_event_t * e) {sendGET(radia[actualRadio].url+"radio?s=2");}  
static void RadioSTA_3_event_handler(lv_event_t * e) {sendGET(radia[actualRadio].url+"radio?s=3");}  


static void RadioInfo_event_handler(lv_event_t * e) {
      opiszRadio();
      String param = radia[actualRadio].url+"radio?n=0";
      sendGET(param);  
}  

static void RadioVolPlus_event_handler(lv_event_t * e) {
      sendGET(radia[actualRadio].url+"radio?v=p");  
}  
static void RadioVolMinus_event_handler(lv_event_t * e) {
      Serial.println("RadioVolPlus click");
      String param = radia[actualRadio].url+"radio?v=m";
      sendGET(param);  
}  
static void RadioStaPlus_event_handler(lv_event_t * e) {
      Serial.println("RadioVolPlus click");
      String param = radia[actualRadio].url+"radio?s=p";
      sendGET(param);  
}  
static void RadioStaMinus_event_handler(lv_event_t * e) {
      Serial.println("RadioVolPlus click");
      String param = radia[actualRadio].url+"radio?s=m";
      sendGET(param);  
}  

static void ESC_event_handler(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
      Serial.println("toggle ESC");
      void printHeap();
       lv_button_demo(0);    
    }
  }  


static void DRAW_buttons_alarmy(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    Serial.print("code=");Serial.println(code);
    /*Serial.print("LV_EVENT_CLICKED       =");Serial.println(code == LV_EVENT_CLICKED);
    Serial.print("LV_EVENT_VALUE_CHANGED =");Serial.println(code == LV_EVENT_VALUE_CHANGED);
    Serial.print("LV_EVENT_PRESSED       =");Serial.println(code == LV_EVENT_PRESSED);
    Serial.print("LV_EVENT_ALL           =");Serial.println(code == LV_EVENT_ALL);*/
    
    
    
    //if (code == LV_EVENT_VALUE_CHANGED)
    if (code == LV_EVENT_CLICKED)
    {
      Serial.println("toggle ALARM --------------------");
      Serial.println(ESP.getFreeHeap());
      lv_button_alarmy();      
      Serial.println(ESP.getFreeHeap());
      void printHeap();
    }
  }  

static void alarm_event_handler(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
      Serial.println("ALARM");
      lv_button_demo(0);
      
    }
  }  
  

  /* Counter button event handler */
  static void counter_event_handler(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    //Serial.print("[7]LV_EVENT_CLICKED=");Serial.print(LV_EVENT_CLICKED);Serial.print(" c code=");Serial.println(code);
    if (code == LV_EVENT_CLICKED)
    {
      static uint8_t cnt = 0;
      cnt++;

      /*Get the first child of the button which is the label and change its text*/
      lv_obj_t *label = lv_obj_get_child(btn, 0);
      lv_label_set_text_fmt(label, "ESC: %d", cnt);
      LV_LOG_USER("LV_Clicked");
      Serial.println("1Clicked");

            //qqqqqq
            Serial.println(ESP.getFreeHeap());
            lcd.setCursor(5, 0);
            //lcd.printf("Click:(%02d)", cnt);
            //lcd.printf("Click:(%d)", cnt);
            int hp = ESP.getFreeHeap();
            lcd.printf("Heap:(%d)", hp);
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






void createButton(int8_t x, int8_t y, const char *labelka, lv_event_cb_t eventBTN)
  {
    lv_obj_t *label;
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    //lv_obj_add_event_cb(btn, eventBTN, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(btn, eventBTN, LV_EVENT_CLICKED, NULL);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_pos(btn, (spaceW*(x+1)) + (buttonW*x),  lineStart + (spaceH*(y)) + (lineH*(y+1)));   /*Set its position*/
    lv_obj_set_size(btn, buttonW, buttonH);   /*Set its size*/
    label = lv_label_create(btn);
    lv_label_set_text(label, labelka);
    lv_obj_center(label);
  }

void lv_button_alarmy(void)
{
    for (int8_t i=0; i<4;i++)  {
        createButton(alarmy[i].x, alarmy[i].y, alarmy[i].labelka, alarmy[i].eventBTN);        
    }
}




void lv_button_demo(bool wifi)
{
    for (int8_t i=0; i<16;i++)  {
        createButton(klawisze[i].x, klawisze[i].y, klawisze[i].labelka,klawisze[i].eventBTN);
    }
    if (wifi) connectWIFI();
}
