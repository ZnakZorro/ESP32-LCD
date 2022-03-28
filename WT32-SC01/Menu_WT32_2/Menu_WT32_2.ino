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
LGFX_Sprite canvap;

#include <lvgl.h>
//#include "lv_confa.h"

#include "omenu.h"
#include "onet.h"
#include "czas.h"

/*** Setup screen resolution for LVGL ***/
char text[] = "                                                                                                                                 ";  // %160 spacji
char texp[] = "                                                                                                                                 ";  // %160 spacji
size_t textpos = 0;
size_t texppos = 0;
size_t texplen = sizeof(texp) / sizeof(texp[0]);
size_t textlen = sizeof(text) / sizeof(text[0]);

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

void TimersSetup();
String getRealTime();


/*eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee*/
/*
void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);
  
    switch (event) {
      case SYSTEM_EVENT_WIFI_READY: 
        Serial.println("=ev= WiFi interface ready");
        break;
      case SYSTEM_EVENT_SCAN_DONE:
        Serial.println("=ev= Completed scan for access points");
        break;
      case SYSTEM_EVENT_STA_START:
        Serial.println("=ev= WiFi client started");
        break;
      case SYSTEM_EVENT_STA_STOP:
        Serial.println("=ev= WiFi clients stopped");
        break;
      case SYSTEM_EVENT_STA_CONNECTED:
        Serial.println("=ev= Connected to access point");
        break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("=ev= Disconnected from WiFi access point");
        WiFi.begin(ssid, password);
        break;
      case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        Serial.println("=ev= Authentication mode of access point has changed");
        break;
      case SYSTEM_EVENT_STA_GOT_IP:
        Serial.print("=ev= Obtained IP address: ");
        Serial.println(WiFi.localIP());
        //ssssssssssssssssssssssssssssssssssssssssssssssssssssss
        //TimersSetup();
        //getInfo();
        //getYrnoPogoda();
        break;
      case SYSTEM_EVENT_STA_LOST_IP:
        Serial.println("=ev= Lost IP address and IP address is reset to 0");
        break;
      case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        Serial.println("=ev= WiFi Protected Setup (WPS): succeeded in enrollee mode");
        break;
      case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        Serial.println("=ev= WiFi Protected Setup (WPS): failed in enrollee mode");
        break;
      case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        Serial.println("=ev= WiFi Protected Setup (WPS): timeout in enrollee mode");
        break;
      case SYSTEM_EVENT_STA_WPS_ER_PIN:
        Serial.println("=ev= WiFi Protected Setup (WPS): pin code in enrollee mode");
        break;
      case SYSTEM_EVENT_AP_START:
        Serial.println("=ev= WiFi access point started");
        break;
      case SYSTEM_EVENT_AP_STOP:
        Serial.println("=ev= WiFi access point  stopped");
        break;
      case SYSTEM_EVENT_AP_STACONNECTED:
        Serial.println("=ev= Client connected");
        break;
      case SYSTEM_EVENT_AP_STADISCONNECTED:
        Serial.println("=ev= Client disconnected");
        break;
      case SYSTEM_EVENT_AP_STAIPASSIGNED:
        Serial.println("=ev= Assigned IP address to client");
        break;
      case SYSTEM_EVENT_AP_PROBEREQRECVED:
        Serial.println("=ev= Received probe request");
        break;
      case SYSTEM_EVENT_GOT_IP6:
        Serial.println("=ev= IPv6 is preferred");
        break;
      case SYSTEM_EVENT_ETH_START:
        Serial.println("=ev= Ethernet started");
        break;
      case SYSTEM_EVENT_ETH_STOP:
        Serial.println("=ev= Ethernet stopped");
        break;
      case SYSTEM_EVENT_ETH_CONNECTED:
        Serial.println("=ev= Ethernet connected");
        break;
      case SYSTEM_EVENT_ETH_DISCONNECTED:
        Serial.println("=ev= Ethernet disconnected");
        break;
      case SYSTEM_EVENT_ETH_GOT_IP:
        Serial.println("=ev= Obtained IP address");
        break;
      default: break;
    }
}
*/
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("#160 WiFi connected");
  Serial.println("#161 IP address: ");
  Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
        //ssssssssssssssssssssssssssssssssssssssssssssssssssssss
      TimersSetup();
      getInfo();
      getYrnoPogoda();
}
/*eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee*/








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

void showClock(){
      String czas = getRealTime();
      lv_obj_t *label = lv_obj_get_child(klawisze[0].btn, 0);
      lv_label_set_text_fmt(label, czas.c_str());
      long timeStamp = getTimeStamp();
      //Serial.printf("\ntimeStamp=(%d)",timeStamp);  1648362916
}

void SetDefaultLabels(int start){
  //Serial.println("\n***");
    for (int8_t i=0+start; i<4+start; i++){
      lv_obj_t *label = lv_obj_get_child(klawisze[i].btn, 0);
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
void setActiveRadio(String l0){
      int8_t radioNR = l0.toInt()+8;
      //Serial.print("radioNR=");Serial.println(radioNR);
      lv_obj_set_style_bg_color(klawisze[radioNR].btn, lv_palette_main(LV_PALETTE_DEEP_PURPLE), 0);
      //lv_obj_set_style_text_color(klawisze[radioNR].btn, lv_palette_main(LV_PALETTE_NONE), 0);  
}
void getInfo(){
  sendGET(radia[actualRadio].url+"radio?n=0");
}

void sendGET(String serverPath){
   //Serial.printf("\nsendGET...%d :: %s\n",licznik, serverPath);
   //printHeap();
   if(WiFi.status()== WL_CONNECTED){
      String jsonBuffer = httpGETRequest(serverPath.c_str());      
      //Serial.print("======\njsonBuffer=");Serial.println(jsonBuffer);
      String l0 = splitValue(jsonBuffer,'!',0); // station nr
      String l1 = splitValue(jsonBuffer,'!',1); // volume
      String l2 = splitValue(jsonBuffer,'!',2); // RSSI
      String l3 = splitValue(jsonBuffer,'!',3); // 
      String l4 = splitValue(jsonBuffer,'!',4);

      SetDefaultLabels(0);
      SetDefaultLabels(8);
      if (l0 != " ") {
        setKeyInfo("  $"+l0+"  %"+l1);  // #station %volume
        setESCInfo(l2);    // RSSI
        setActiveRadio(l0); 
      }
      jsonBuffer = " * "+radia[actualRadio].name+" * "+l2+"% * #"+l0+" * "+l1+"dB * "+l3+" * "+l4;
      jsonBuffer = radia[actualRadio].name+" * "+l3+" * "+l4;
 
            //lcd.setCursor(0, 2);
            //lcd.printf(jsonBuffer.c_str());
            int strl = jsonBuffer.length();
            for(int i=0; i<strl; i++){
              text[i] = jsonBuffer[i];
            }
            
            //String l00 = splitValue(text,"!",0);
            textlen = strl;//sizeof(text) / sizeof(text[0]);
            textpos = 0; 
            //canvas.setCursor(0, 240);
            //canvas.setCursor(0, 0);
            
   }
   else {
      Serial.println("WiFi Disconnected");
   }
}

void getYrnoPogoda(){
  Serial.println("Pogoda...");
  //clio.drukLCD("Pogoda...");
     if(WiFi.status()== WL_CONNECTED){
      //String serverPath = proxyHost;  // from credentials.h
      //Serial.println(serverPath);
      String pogodaBuffer = httpGETRequest(line1Host);

      //clio.payload2LCD(jsonBuffer); 
            int strl = pogodaBuffer.length();
            Serial.println(pogodaBuffer); 
            Serial.println(strl); 
            for(int i=0; i<strl; i++){
              texp[i] = pogodaBuffer[i];
            }
            texplen = strl;
            texppos = 0;         
    }
    else {
      Serial.println("getYrnoPogoda WiFi Disconnected");
    }
  
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
        canvas.createSprite(lcd.width() + 32, 32);  // było 36
  /*sssssssssssssss*/
  /*ppppppppppppppp*/
        canvap.setColorDepth(8);
        //canvap.setFont(&fonts::lgfxJapanMinchoP_40);  //28 32 36 40 //https://m5stack.lang-ship.com/howto/m5gfx/font/
        canvap.setFont(&fonts::Font4);
        //canvap.setTextFont(8);
        
        canvap.setTextWrap(false); 
        canvap.createSprite(lcd.width() + 32, 32);
  /*pppppppppppppp*/
  

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
  //lv_obj_t *label = lv_label_create(lv_scr_act()); // full screen as the parent
  //lv_label_set_text(label, LVGL_Arduino.c_str());  // set label text
  //lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 18);      // Center but 20 from the top

  //lv_button_demo_start();
  lv_button_demo(1);
  lv_timer_handler();


  /*eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee*/
  //WiFi.disconnect(true);
  //delay(1000);

  // Examples of different ways to register wifi events
  //WiFi.onEvent(WiFiEvent);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
  WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
    Serial.print("#412 WiFi lost connection. Reason: ");
    Serial.println(info.disconnected.reason);
  }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);

  // Remove WiFi event
  Serial.print("#417 WiFi Event ID: ");
  Serial.println(eventID);
  // WiFi.removeEvent(eventID);
  /*eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee*/
  Serial.println("......");
  
  lasTsecundDelay = millis();
  lastMillis = millis();
  licznik=0;
  connectWIFI();  
  //TimersSetup();
  //getYrnoPogoda();
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
          Serial.print("\n$$$$$$$$$$$$$$$type/flagaMenu = ");Serial.print(typeMenu);Serial.print("/");Serial.println(flagaMenu);
          if (typeMenu == TYPE_MENU_MAIN) {Serial.println("MAIN Menu");}
          if (typeMenu == TYPE_MENU_ALARM) {Serial.println("aaaaaaaaaaaALARM Menu");}
          lastMenu = flagaMenu;
        }
    
    lastMillis = millis();
  }


  // test every second
  if ((millisy - lasTsecundDelay) > secundDelay) {
    //Serial.print(", ");Serial.print(licznik);
    //if (licznik % 30 == 0) Serial.println("");
      //if (licznik==1) connectWIFI();
      //if (licznik==20) getInfo();
      //if (licznik==25) getYrnoPogoda();
      //if (licznik==60) getYrnoPogoda();
      if (licznik % 30 == 0) getInfo(); 
      if (licznik % 30 > 5) showClock();
      if (licznik % 1800 == 100) getYrnoPogoda();
      
      //esp_reBootSleep("2");
      
      lasTsecundDelay = millis();
      licznik++;


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
          int32_t cursor_p = canvap.getCursorX() - 1; 
          if (cursor_x <= 0){
            textpos = 0;    
            cursor_x = lcd.width(); 
          }       
          canvas.setCursor(cursor_x, 0); 
          canvas.scroll(-1, 0);          
          while (textpos < textlen && cursor_x <= lcd.width()){
            canvas.print(text[textpos++]);   
            cursor_x = canvas.getCursorX();  
          }       
          canvas.pushSprite(&lcd, 0, 0);
  /*lllllllllllllllllll*/
/*pppppppppppppppppppppppppppppppppppppppppp*/
          
          if (cursor_p <= 0){
            texppos = 0;    
            cursor_p = lcd.width(); 
          }       
          canvap.setCursor(cursor_p, 0); 
          canvap.scroll(-1, 0);          
          while (texppos < texplen && cursor_p <= lcd.width()){
            canvap.print(texp[texppos++]);   
            cursor_p = canvap.getCursorX();  
          }       
          canvap.pushSprite(&lcd, 0, 32);
  /*pppppppppppppppppp*/

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
