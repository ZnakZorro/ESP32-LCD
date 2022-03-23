#define MENU_MAIN         0
#define MENU_ALARMY       1

#define TYPE_MENU_MAIN    0 
#define TYPE_MENU_ALARM   1

int8_t typeMenu  = TYPE_MENU_MAIN;
int8_t flagaMenu = 0;
int8_t lastMenu  = 0;

#include <LovyanGFX.hpp>
#include <lvgl.h>

/*** Function declaration ***/
void createButton(int8_t x, int8_t y, const char *labelka, lv_event_cb_t eventBTN, int8_t nr);
void lv_button_alarmy(void);
void lv_button_demo(bool flaga);
static void connectWIFI();
static void savePreferences();
void getInfo();

static void DRAW_buttons_alarmy(lv_event_t * e);
static void alarm_event_handler1(lv_event_t * e);
static void alarm_event_handler2(lv_event_t * e);
static void alarm_event_handler3(lv_event_t * e);
static void alarm_event_handler4(lv_event_t * e);

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

static void updateButtons(int8_t menu, int8_t startRow, int8_t ctn);


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
    lv_obj_t *btn;
};

buttonsSTRUCT klawisze[16]={
  0, 0, " ESC",     ESC_event_handler, NULL,
  1, 0, "Salon",   RadioActual_0_event_handler, NULL,
  2, 0, "JBL",     RadioActual_1_event_handler, NULL,
  3, 0, "Cartoon", RadioActual_2_event_handler, NULL,
  

  0, 1, "Vol-",      RadioVolMinus_event_handler, NULL,
  1, 1, "Vol+",      RadioVolPlus_event_handler, NULL,
  2, 1, "Sta-",      RadioStaMinus_event_handler, NULL,
  3, 1, "Sta+",      RadioStaPlus_event_handler, NULL,

  0, 2, "#ffffa0 TOK-FM",   RadioSTA_0_event_handler, NULL,
  1, 2, "#ffffa0 RNS",      RadioSTA_1_event_handler, NULL,
  2, 2, "#ffffa0 357",      RadioSTA_2_event_handler, NULL,
  3, 2, "#ffffa0 Classic",  RadioSTA_3_event_handler, NULL,

  0, 3, "Info",      RadioInfo_event_handler, NULL,
  1, 3, "reStart",    toggle_event_handler_restart, NULL,
  2, 3, "Sleep",      toggle_event_handler3, NULL,
  3, 3, "Alarm",      DRAW_buttons_alarmy, NULL,
  
};

buttonsSTRUCT alarmy[4]={
  0, 2, "1min",         alarm_event_handler1, NULL,
  1, 2, "5min",         alarm_event_handler2, NULL,
  2, 2, "15min",        alarm_event_handler3, NULL,
  3, 2, "60min",        alarm_event_handler4, NULL,
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
static void RadioInfo_event_handler(lv_event_t * e) {
      opiszRadio();
      getInfo();
      //String param = radia[actualRadio].url+"radio?n=0";
      //sendGET(param);  
}  

static void RadioActual_0_event_handler(lv_event_t * e) {actualRadio=0;savePreferences();opiszRadio();getInfo();}
static void RadioActual_1_event_handler(lv_event_t * e) {actualRadio=1;savePreferences();opiszRadio();getInfo();}
static void RadioActual_2_event_handler(lv_event_t * e) {actualRadio=2;savePreferences();opiszRadio();getInfo();}

/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/
static void activeMENU(int8_t nr){
    flagaMenu=nr; 
    if (typeMenu == TYPE_MENU_MAIN)  {
      sendGET(radia[actualRadio].url+"radio?s="+nr);
    }
    if (typeMenu == TYPE_MENU_ALARM) {
      Serial.print("AAAlarm = "+String(nr));
    }

}
static void RadioSTA_0_event_handler(lv_event_t * e) {  activeMENU(0);}  
static void RadioSTA_1_event_handler(lv_event_t * e) {  activeMENU(1);}  
static void RadioSTA_2_event_handler(lv_event_t * e) {  activeMENU(2);}  
static void RadioSTA_3_event_handler(lv_event_t * e) {  activeMENU(3);}  


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
    updateButtons(MENU_MAIN, 0, 16);
    typeMenu  = TYPE_MENU_MAIN;
    getInfo();
    /*
    if (code == LV_EVENT_VALUE_CHANGED)
    {
      Serial.println("toggle ESC");
      void printHeap();
       lv_button_demo(0);    
    }*/
  }  


static void DRAW_buttons_alarmy(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    Serial.print("code=");Serial.println(code);
    //updateButtons(MENU_MAIN, 0, 16);
    updateButtons(MENU_ALARMY, 2, 4);
    typeMenu  = TYPE_MENU_ALARM;
    /*Serial.print("LV_EVENT_CLICKED       =");Serial.println(code == LV_EVENT_CLICKED);
    Serial.print("LV_EVENT_VALUE_CHANGED =");Serial.println(code == LV_EVENT_VALUE_CHANGED);
    Serial.print("LV_EVENT_PRESSED       =");Serial.println(code == LV_EVENT_PRESSED);
    Serial.print("LV_EVENT_ALL           =");Serial.println(code == LV_EVENT_ALL);*/
    
    
    /*
    //if (code == LV_EVENT_VALUE_CHANGED)
    if (code == LV_EVENT_CLICKED)
    {
      Serial.println("toggle ALARM --------------------");
      Serial.println(ESP.getFreeHeap());
      lv_button_alarmy();      
      Serial.println(ESP.getFreeHeap());
      void printHeap();
    }*/
  }  

static void alarm_event_handler1(lv_event_t * e)  {
    lv_event_code_t code = lv_event_get_code(e);
    flagaMenu=1;
    //updateButtons(MENU_MAIN, 0, 16);
}  
static void alarm_event_handler2(lv_event_t * e)  {
    lv_event_code_t code = lv_event_get_code(e);
    flagaMenu=2;
    //updateButtons(MENU_MAIN, 0, 16);
}  
static void alarm_event_handler3(lv_event_t * e)  {
    lv_event_code_t code = lv_event_get_code(e);
    flagaMenu=3;
    //updateButtons(MENU_MAIN, 0, 16);
}  
static void alarm_event_handler4(lv_event_t * e)  {
    lv_event_code_t code = lv_event_get_code(e);
    flagaMenu=4;
    //updateButtons(MENU_MAIN, 0, 16);
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
    ESP.restart();
    /*lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
      LV_LOG_USER("restart");
      Serial.println("restart");
      ESP.restart();
    }*/
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


void lv_button_alarmy(void)
{
    updateButtons(MENU_ALARMY, 2, 4);
    /*for (int8_t i=0; i<4;i++)  {
        createButton(alarmy[i].x, alarmy[i].y, alarmy[i].labelka, alarmy[i].eventBTN,i);        
    }*/
}




void lv_button_demo(bool wifi)
{
    for (int8_t i=0; i<16;i++)  {
        createButton(klawisze[i].x, klawisze[i].y, klawisze[i].labelka,klawisze[i].eventBTN,i);
    }
    //if (wifi) connectWIFI();
}





void createButton(int8_t x, int8_t y, const char *labelka, lv_event_cb_t eventBTN, int8_t nr)
  {
    lv_obj_t *label;
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    //lv_obj_add_event_cb(btn, eventBTN, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(btn, eventBTN, LV_EVENT_CLICKED, NULL);
    //lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_pos(btn, (spaceW*(x+1)) + (buttonW*x),  lineStart + (spaceH*(y)) + (lineH*(y+1)));   /*Set its position*/
    lv_obj_set_size(btn, buttonW, buttonH);   /*Set its size*/
    label = lv_label_create(btn);
    lv_label_set_text(label, labelka);
            //lv_label_set_long_mode(label, LV_LABEL_LONG_BREAK);     //Break the long lines
            lv_label_set_recolor(label, true);
            //lv_style_set_text_color(label, LV_STATE_FOCUSED, LV_STATE_FOCUSED);
            //lv_style_set_text_color(label, LV_STATE_DEFAULT, LV_COLOR_GREEN); //LV_COLOR_RED
/*Add a shadow*/
    static lv_style_t style_shadow;
    lv_style_init(&style_shadow);
    lv_style_set_radius(&style_shadow, 10);
    lv_style_set_shadow_width(&style_shadow, 10);
    lv_style_set_shadow_spread(&style_shadow, 5);
    lv_style_set_shadow_color(&style_shadow, lv_palette_main(LV_PALETTE_BLUE));    
               
    lv_obj_center(label);
    klawisze[nr].btn =  btn;
    
  }


/*bbbbbbbbbbbbbbbbb*/
static void updateButtons(int8_t menu, int8_t startRow, int8_t ctn){
  Serial.println("updateButtons=");
  //Serial.println(btns);
    for (int8_t i=0; i<ctn;i++)  {
      int8_t nr = (startRow * 4) + i;
      //Serial.print(nr);Serial.print(" = ");Serial.println(klawisze[nr].labelka);
      lv_obj_t *label = lv_obj_get_child(klawisze[nr].btn, 0);
      if (menu == MENU_ALARMY){
          lv_label_set_text_fmt(label,  alarmy[i].labelka);
          //lv_obj_remove_event_dsc(klawisze[nr].btn,NULL);
          const char *num1 = "a"; // ??????????????????
          //lv_obj_add_event_cb(klawisze[nr].btn, alarmy[i].eventBTN, LV_EVENT_CLICKED, &num1);
          //Serial.println(alarmy[nr].labelka);
          lv_obj_set_style_bg_color(klawisze[nr].btn, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
          lv_obj_set_style_text_color(klawisze[nr].btn, lv_palette_main(LV_PALETTE_YELLOW), 0);
      }
      if (menu == MENU_MAIN){
          lv_label_set_text_fmt(label,  klawisze[i].labelka);
          //lv_obj_remove_event_dsc(klawisze[nr].btn,NULL);
          //lv_obj_add_event_cb(klawisze[nr].btn, klawisze[i].eventBTN, LV_EVENT_CLICKED, NULL);
          lv_obj_set_style_bg_color(klawisze[nr].btn, lv_palette_main(LV_PALETTE_BLUE), 0);
          lv_obj_set_style_text_color(klawisze[nr].btn, lv_palette_main(LV_PALETTE_YELLOW), 0);
          
      }
          //lv_label_set_text_fmt(label,  +"%s%s: %02d", "*",alarmy[nr].labelka,nr);
      

        //createButton(alarmy[i].x, alarmy[i].y, alarmy[i].labelka, alarmy[i].eventBTN,i);        
    }  
    getInfo();
}
