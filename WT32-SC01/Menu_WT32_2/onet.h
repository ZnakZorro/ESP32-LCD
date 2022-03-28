static void connectWIFI();

static void connectWIFI(){
    Serial.println(ESP.getFreeHeap());
    Serial.printf_P(PSTR("Free mem=%d\n"), ESP.getFreeHeap());

    //WiFi.mode(WIFI_OFF);
    //WiFi.disconnect();
    WiFi.mode(WIFI_STA);
          WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
          WiFi.setHostname("WT32menu1");
    WiFi.begin(ssid, pass);
      for (int i = 0; i < 8; i++) {
         wifiMulti.addAP(credential[i].ssid, credential[i].pass);
      } 
    lcd.setCursor(0, 0);
    lcd.printf("WiFi Start ");      
    while(wifiMulti.run() != WL_CONNECTED) {
      Serial.print("*");
      Serial.println("WiFi::"+String(licznik));
      delay(333);
      if (licznik%50==49) ESP.restart();
      lcd.print("*");
      licznik++;
      if (licznik>200){
        break;
      }
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
    lcd.print("  ");
    //lcd.printf("IP:%s",String(WiFi.localIP()));
    
    
    //lcd.print(String(WiFi.localIP()));
    delay(4500);
    getInfo();
  
}
