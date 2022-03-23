//WT32-SC01 with LVGL v8.2.0    https://docs.lvgl.io/8.2/examples.html?highlight=lv_obj_add_event_cb
#include "WiFiMulti.h"
#include "AsyncTCP.h"             //https://github.com/me-no-dev/ESPAsyncTCP 
#include "ESPAsyncWebServer.h"    //https://github.com/me-no-dev/ESPAsyncWebServer
#include "HTTPClient.h"           //https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/src/HTTPClient.h
#include "credentials.h"
WiFiMulti wifiMulti;
AsyncWebServer server(80);
#include <Preferences.h>
Preferences preferences;

unsigned long licznik = 0;

#define coIleSekund  30
unsigned long secundDelay      = 1000;//*coIleSekund; 
unsigned long lasTsecundDelay  = 0; 
unsigned long lastMillis  = 0; 
unsigned long millisy = 0;

/*
    Simple Demo with WT32-SC01 + LovyanGFX + LVGL8.x
*/
#define LGFX_AUTODETECT // Autodetect board
#define LGFX_WT32_SC01
#define LGFX_USE_V1     // set to use new version of library
//#define LV_CONF_INCLUDE_SIMPLE "lv_confa.h"

/* Uncomment below line to draw on screen with touch */
#define DRAW_ON_SCREEN


#include <LovyanGFX.hpp> // main library
static LGFX lcd; // declare display variable
LGFX_Sprite canvas;

#include <lvgl.h>
//#include "lv_confa.h"

#include "btns.h"

/*** Setup screen resolution for LVGL ***/
//static constexpr char text[] = "Pilot radiowy *** Pilot radiowy *** Pilot radiowy *** Pilot radiowy *** Pilot radiowy *** Pilot radiowy *** Pilot radiowy *** Pilot radiowy";
//static constexpr size_t textlen = sizeof(text) / sizeof(text[0]);
//size_t textpos = 0;

char text[] = " . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";
size_t textlen = sizeof(text) / sizeof(text[0]);
size_t textpos = 0;

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

static void savePreferences(){
     preferences.begin("menu-wt32", false);
     preferences.putUInt("actualRadio", actualRadio);
     preferences.end();
     Serial.print("#save actualRadio="); Serial.println(actualRadio);
} 
static void readPreferences(){
     preferences.begin("menu-wt32", false);
     actualRadio   = preferences.getUInt("actualRadio", 1);
     preferences.putUInt("actualRadio", actualRadio);
     preferences.end();
     Serial.print("#75 read actualRadio="); Serial.println(actualRadio);
} 

String splitValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : " ";
}




String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  //esp_http_client_set_timeout_ms(esp_http_client_handle_tclient, int timeout_ms)
  //esp_http_client_set_timeout_ms(http, 5000);
  http.begin(client, serverName);
  http.setTimeout(3000);
  http.setConnectTimeout(3000);
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  return payload;
}

void SetDefaultLabels(){
  //Serial.println("\n***");
    for (int8_t i=0; i<4; i++){
      lv_obj_t *label = lv_obj_get_child(klawisze[i].btn, 0);
      //Serial.print(i);Serial.println(klawisze[i].labelka);
      lv_label_set_text_fmt(label, klawisze[i].labelka);
      lv_obj_set_style_bg_color(klawisze[i].btn, lv_palette_main(LV_PALETTE_BLUE), 0);
      lv_obj_set_style_text_color(klawisze[i].btn, lv_palette_main(LV_PALETTE_YELLOW), 0);

    }
}

void setESCInfo(String str){
      lv_obj_t *label = lv_obj_get_child(klawisze[0].btn, 0);
      lv_label_set_text_fmt(label, +"%s%s\n%s%s", "#000000 ","WiFi",str,"dB");
      lv_obj_set_style_bg_color(klawisze[0].btn, lv_palette_main(LV_PALETTE_RED), 0);
      lv_obj_set_style_text_color(klawisze[0].btn, lv_palette_main(LV_PALETTE_NONE), 0);
      
}

void setKeyInfo(String str){
      lv_obj_t *label = lv_obj_get_child(klawisze[actualRadio+1].btn, 0);
      lv_label_set_text_fmt(label, +"%s%s\n%s", "#000000 ",klawisze[actualRadio+1].labelka,str);
      lv_obj_set_style_bg_color(klawisze[actualRadio+1].btn, lv_palette_main(LV_PALETTE_AMBER), 0);
      lv_obj_set_style_text_color(klawisze[actualRadio+1].btn, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
      
}

void getInfo(){
  sendGET(radia[actualRadio].url+"radio?n=0");
}

void sendGET(String serverPath){
   //Serial.printf("\nsendGET...%d :: %s\n",licznik, serverPath);
   printHeap();
   if(WiFi.status()== WL_CONNECTED){
      String jsonBuffer = httpGETRequest(serverPath.c_str());      
      Serial.print("======\njsonBuffer=");Serial.println(jsonBuffer);
      String l0 = splitValue(jsonBuffer,'!',0); // station nr
      String l1 = splitValue(jsonBuffer,'!',1); // volume
      String l2 = splitValue(jsonBuffer,'!',2); // RSSI
      String l3 = splitValue(jsonBuffer,'!',3); // 
      String l4 = splitValue(jsonBuffer,'!',4);

      SetDefaultLabels();
      setKeyInfo("  $"+l0+"  %"+l1);  // #station %volume
      setESCInfo(l2);    // RSSI
      //Serial.println(l0);
      //Serial.println(l1);
      //Serial.println(l2);
      //Serial.println(l3);
      //Serial.println(l4);
      jsonBuffer = " * "+radia[actualRadio].name+" * "+l2+"% * #"+l0+" * "+l1+"dB * "+l3+" * "+l4;
      jsonBuffer = radia[actualRadio].name+" * "+l3+" * "+l4;
      
      Serial.println(jsonBuffer);
      
            //lllllllllllllllllll
                  //canvas.setCursor(0, 20); 
                  //canvas.print("#"+l0+" V"+l1); 

            lcd.setCursor(0, 2);
            //lcd.printf(jsonBuffer.c_str());
            int strl = jsonBuffer.length();
            for(int i=0; i<strl; i++){
              text[i] = jsonBuffer[i];
            }
            
            //String l00 = splitValue(text,"!",0);
            textlen = strl;//sizeof(text) / sizeof(text[0]);
            textpos = 0; 
            //canvas.setCursor(0, 240);
            canvas.setCursor(0, 0);
            
   }
   else {
      Serial.println("WiFi Disconnected");
   }
}

static void connectWIFI(){
Serial.println(ESP.getFreeHeap());
Serial.printf_P(PSTR("Free mem=%d\n"), ESP.getFreeHeap());

    //WiFi.mode(WIFI_OFF);
    //WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
      for (int i = 0; i < 8; i++) {
         wifiMulti.addAP(credential[i].ssid, credential[i].pass);
      } 
    lcd.setCursor(0, 0);
    lcd.printf("WiFi Start ");      
    while(wifiMulti.run() != WL_CONNECTED) {
      Serial.print("*");
      licznik++;
      Serial.println("WiFi::"+String(licznik));
      delay(333);
      if (licznik>50) ESP.restart();
      lcd.print("*");
      //lcd.printf("*:(%03d)", licznik);
      } 
    Serial.printf_P(PSTR("Connected\r\nRSSI: "));
    Serial.print("WiFi.status="); Serial.println(WiFi.status());
    Serial.print("WiFi.RSSI=");   Serial.println(WiFi.RSSI());
    Serial.print("IP: ");         Serial.println(WiFi.localIP());
    Serial.print("SSID: ");       Serial.println(WiFi.SSID());      

    lcd.setCursor(0, 0);
    //lcd.printf("id:%s",String(WiFi.SSID()));
    lcd.print(" IP:"); lcd.print((WiFi.localIP()));
    lcd.print(" ID:"); lcd.print((WiFi.SSID()));
    lcd.print(" ----------");
    //lcd.printf("IP:%s",String(WiFi.localIP()));
    
    
    //lcd.print(String(WiFi.localIP()));
    delay(4500);
    getInfo();
  
}

void setup(void)
{

  Serial.begin(115200); /* prepare for possible serial debug */
  Serial.println(ESP.getFreeHeap());
  readPreferences();
  lcd.init(); // Initialize LovyanGFX
  lv_init();  // Initialize lvgl

  // Setting display to landscape
  if (lcd.width() < lcd.height()) lcd.setRotation(lcd.getRotation() ^ 1);
  
  /*sssssssssssssss*/

        canvas.setColorDepth(8);
        //canvas.setFont(&fonts::lgfxJapanMinchoP_40);  //28 32 36 40 //https://m5stack.lang-ship.com/howto/m5gfx/font/
        canvas.setFont(&fonts::Font4);
        //canvas.setTextFont(8);
        
        canvas.setTextWrap(false); 
        canvas.createSprite(lcd.width() + 36, 36);
  /*sssssssssssssss*/
  

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
  Serial.println(LVGL_Arduino);
  lv_obj_t *label = lv_label_create(lv_scr_act()); // full screen as the parent
  lv_label_set_text(label, LVGL_Arduino.c_str());  // set label text
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 18);      // Center but 20 from the top

  //lv_button_demo_start();
  lv_button_demo(1);
  lv_timer_handler();
  
Serial.println("......");

lasTsecundDelay = millis();
lastMillis = millis();

sendGET(radia[actualRadio].url+"radio?n=0");  
licznik=0;

} // EOF setup

/*void lv_button_demo_start(){
  lv_button_demo(1);
  
}*/


void loop()
{
  millisy = millis();

  if ((millisy - lastMillis) > 50) {
    lv_timer_handler();   // update GUI
        if (flagaMenu != lastMenu){
          Serial.print("\n$$$$$type/flagaMenu = ");Serial.print(typeMenu);Serial.print("/");Serial.println(flagaMenu);
          if (typeMenu == TYPE_MENU_MAIN) {Serial.println("MAIN Menu");}
          if (typeMenu == TYPE_MENU_ALARM) {Serial.println("ALARM Menu");}
          lastMenu = flagaMenu;
        }
    
    lastMillis = millis();
  }

  // test every second
  if ((millisy - lasTsecundDelay) > secundDelay) {
      if (licznik==1) connectWIFI();
      if (licznik==5) getInfo();//sendGET(radia[actualRadio].url+"radio?n=0"); 
      if (licznik % 30 == 29) getInfo();//sendGET(radia[actualRadio].url+"radio?n=0"); 
      //esp_reBootSleep("2");
      
      lasTsecundDelay = millis();
      licznik++;

      //lv_obj_t *label0 = lv_obj_get_child(klawisze[0].btn, 0);
      //lv_label_set_text_fmt(label0, "ESC\n#%02d", licznik);
      
      //lv_obj_t *label = lv_obj_get_child(klawisze[actualRadio+1].btn, 0);
      //lv_label_set_text_fmt(label, +"%s%s:\n%02d", "#ff0000 ",klawisze[actualRadio+1].labelka,licznik % 100);
      //lv_label_set_text_fmt(label, +"%s%s\n%02d", "#ff0000 ",klawisze[actualRadio+1].labelka,licznik % 100);
  }
  
  
  
  
  //lv_timer_handler(); /* let the GUI do its work */
  //delay(5);

#ifdef DRAW_ON_SCREEN
  /*** Draw on screen with touch ***/
  if (lcd.getTouch(&x, &y))
  {
    lcd.fillRect(x - 2, y - 2, 5, 5, TFT_RED);
    lcd.setCursor(380, 0);
    lcd.printf("Touch:(%03d,%03d)", x, y);
  }
#endif

/*lllllllllllllllllllllllllllllllllllll*/
          int32_t cursor_x = canvas.getCursorX() - 1; 
          //Serial.print(cursor_x);Serial.print(",");
          //if (cursor_x <= 0) Serial.println();
          if (cursor_x <= 0)
          {
            textpos = 0;    
            cursor_x = lcd.width(); 
          }
        
          canvas.setCursor(cursor_x, 0); 
          canvas.scroll(-1, 0);          
          while (textpos < textlen && cursor_x <= lcd.width()) 
          {
            canvas.print(text[textpos++]);   
            cursor_x = canvas.getCursorX();  
          }
        
          canvas.pushSprite(&lcd, 0, 0);

  /*lllllllllllllllllll*/

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
