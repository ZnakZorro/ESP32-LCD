
    bool zagraj = false;
    int zagrajNR = 0;


#include "time.h"
const char* ntpServer = "3.pl.pool.ntp.org";
const long  gmtOffset_sec      = 3600;   //12600;
const int   daylightOffset_sec = 3600;

      const char powitanie[] PROGMEM = {"Radio blacha"};
      
      const char dzien_0[] PROGMEM = "Niedziela";
      const char dzien_1[] PROGMEM = "Poniedziałek";
      const char dzien_2[] PROGMEM = "Wtorek";
      const char dzien_3[] PROGMEM = "środa";
      const char dzien_4[] PROGMEM = "Czwartek";
      const char dzien_5[] PROGMEM = "Piątek";
      const char dzien_6[] PROGMEM = "Sobota";
      
      const char *const daysOFweek[] PROGMEM = {dzien_0,dzien_1,dzien_2,dzien_3,dzien_4,dzien_5,dzien_6};
      char buffer[16];



 long getTimeStamp() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      return(0);
    }
    time(&now);
    return now;
}
  



bool compareTimes(int nr, long timeStart,  long timeEnd){
     long timeNow = getTimeStamp();
     long deltaStart = timeNow - timeStart;
     long deltaEnd   = timeNow - timeEnd;
    Serial.print(nr); Serial.print(" %%%%%%%%%%%%%%%% ");Serial.println(nr);
    
    Serial.print(" timeNow: ");Serial.print(timeNow);
    Serial.print("; dStart: ");Serial.print(deltaStart);
    Serial.print("; dEnd: ");Serial.print(deltaEnd);
    bool ret = (deltaStart>0 && deltaEnd<0); 
    Serial.print(", ret=");Serial.println(ret);
    return ret;
}






String pad(int liczba){
  String p="";
  if (liczba<10) p="0";
  return p+String(liczba);
}

String getTime(){
       struct tm timeinfo;
       if(!getLocalTime(&timeinfo)){
          Serial.println("Failed to obtain time");
          return "...!";
      }
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      int weekDay = timeinfo.tm_wday;
      int dzien   = timeinfo.tm_mday;
      int miesiac = timeinfo.tm_mon + 1;
      int rok     = timeinfo.tm_year + 1900;
      int godzina = timeinfo.tm_hour;
      int minuta  = timeinfo.tm_min;
      int sekunda = timeinfo.tm_sec;
      int dzienrok= timeinfo.tm_yday;
      snprintf(buffer, 16, daysOFweek[weekDay]);
      return pad(rok)+"-"+pad(miesiac)+"-"+pad(dzien)+", "+pad(godzina)+":"+pad(minuta)+":"+pad(sekunda)+", "+ String(buffer);
}

void showTime(){
      Serial.println("#84---showTime()---");
      Serial.println(getTime());
      //return;
      //tm (int8_t wday, int16_t year, int8_t mon, int8_t mday, int8_t hour, int8_t min, int8_t sec)
       struct tm timeinfo;
       if(!getLocalTime(&timeinfo)){
          Serial.println("Failed to obtain time");
          return;
      }
      //Serial.println("@@@@@@@@@@@@@@@@");
      //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      //Serial.print(timeinfo.tm_wday);Serial.print("/");Serial.print(timeinfo.tm_hour);Serial.print("/");Serial.println(timeinfo.tm_min);
      int weekDay = timeinfo.tm_wday;
      int dzien   = timeinfo.tm_mday;
      int godzina = timeinfo.tm_hour;
      int minuta  = timeinfo.tm_min;
      Serial.print(godzina);   Serial.print(":");  Serial.print(minuta); Serial.print("  ");

      Serial.print(", %w=");  Serial.print(weekDay);
      Serial.print(", %d=");  Serial.print(dzien);
      Serial.print(", %H=");  Serial.print(godzina);
      Serial.print(", %M=");  Serial.print(minuta);
      Serial.print(", po=");  Serial.print(powitanie);
      Serial.print(" ");  
      snprintf(buffer, 16, daysOFweek[weekDay]);
      Serial.println(buffer);
}

long countTimeStamp(long godz, long minu){     
       struct tm timeinfo;
       if(!getLocalTime(&timeinfo)){
          Serial.println("Failed to obtain time");
          return 0;
      }
      long ts = getTimeStamp();      
      Serial.print("#134 ts=");Serial.println(ts);
      showTime();
      long godzina = timeinfo.tm_hour;
      long minuta  = timeinfo.tm_min;
      /*Serial.print("godz=");Serial.println(godz);
      Serial.print("godzina=");Serial.println(godzina);
      Serial.print("minu=");Serial.println(minu);
      Serial.print("minuta=");Serial.println(minuta);
      Serial.print("godz-godzina=");Serial.println(godz-godzina);
      Serial.print("minu-minuta=");Serial.println(minu-minuta);*/
      ts = ts + ((godz-godzina)*60*60);
      ts = ts + ((minu-minuta)*60);
      Serial.print("#140 ts=");Serial.println(ts);
    return ts;
}

struct czasySTRUCT  {
  long start;  
  long end;  
};

struct czasySTRUCT czasy [5]={
  {0,0},
  {0,0},
  {0,0},
  {0,0},
  {0,0}
};


//unsigned long licznik=0;
 long epochTime; 
 long TcurrentMillis  = 0;
 long TpreviousMillis = 0;
 long TintervalMillis = 1000*60;

void TimersSetup(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  epochTime = getTimeStamp();
  //czasy[0].start = epochTime+100;
  //czasy[0].end   = epochTime+200;
  //czasy[1].start = epochTime+300;
  //czasy[1].end   = epochTime+400;
  //czasy[2].start = epochTime+500;
  //czasy[2].end   = epochTime+600;
}


void timersLoop(){
  return;
    TcurrentMillis = millis();
    if(TcurrentMillis - TpreviousMillis > TintervalMillis){
      TpreviousMillis = millis();
              showTime();              
              byte bajt=0;
              //bool czy=compareTimes(0,epochTime-20, epochTime+10);
              bool czy0 = compareTimes(0,czasy[0].start,czasy[0].end);
              Serial.print("czy0= ");Serial.println(czy0);
              bool czy1 = compareTimes(1,czasy[1].start,czasy[1].end);
              Serial.print("czy1= ");Serial.println(czy1);
              bool czy2 = compareTimes(2,czasy[2].start,czasy[2].end);
              Serial.print("czy2= ");Serial.println(czy2);
              bool czy3 = compareTimes(3,czasy[3].start,czasy[3].end);
              Serial.print("czy3= ");Serial.println(czy3);
              bool czy4 = compareTimes(4,czasy[4].start,czasy[4].end);
              Serial.print("czy4= ");Serial.println(czy4);

              if (czy0){
                    zagrajNR=0;
                    zagraj = true;
                    Serial.println("czy0=1 zagraj 00000000000000000");
              }
              if (czy1){
                    zagrajNR=0;
                    zagraj = true;
                    Serial.println("czy1=1 zagraj 111111111111111111");
              }
              if (czy2){
                    zagrajNR=0;
                    zagraj = true;
                    Serial.println("czy2=1 zagraj 2222222222222222222");
              }
              if (czy3){
                    zagrajNR=0;
                    zagraj = true;
                    Serial.println("czy2=1 zagraj 3333333333333333333333");
              }
              if (czy4){
                    zagrajNR=0;
                    zagraj = true;
                    Serial.println("czy2=1 zagraj 44444444444444444444444444");
              }
              if (czy0) Serial.println("czy0=1");
              if (czy1) Serial.println("czy0=2");
              if (czy2) Serial.println("czy0=3");
              if (czy3) Serial.println("czy0=3");
              if (czy4) Serial.println("czy0=3");
              if (czy0) bajt = bajt || 0b00000001;
              if (czy1) bajt = bajt || 0b00000010;
              if (czy2) bajt = bajt || 0b00000100;
              if (czy3) bajt = bajt || 0b00001000;
              if (czy4) bajt = bajt || 0b00010000;
              Serial.print("byte= ");Serial.println(bajt);
    }
}


/*
            void printLocalTime() {
              showTime();
              epochTime = getTimeStamp();
              bool czy=compareTimes(0,epochTime-20, epochTime+10);            
            }
*/







/*
           unsigned long licznik=0;
            #include "time.h"
            const char* ntpServer = "3.pl.pool.ntp.org";
            const long  gmtOffset_sec      = 3600;   //12600;
            const int   daylightOffset_sec = 3600;


            
            void printLocalTime() {
              showTime();
              struct tm timeinfo;

              if(!getLocalTime(&timeinfo)){
                Serial.println("Failed to obtain time");
                return;
              }
             
              Serial.println("***...................");
              Serial.println(licznik);
              Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
                  char miesiac[3];  strftime(miesiac,3, "%m", &timeinfo);
                  char wd[3];       strftime(wd,3,      "%w", &timeinfo);
                  char dzien[3];    strftime(dzien,3,   "%d", &timeinfo);
                  char godzina[3];  strftime(godzina,3, "%H", &timeinfo);
                  char minuta[3];   strftime(minuta,3,  "%M", &timeinfo);
                  int d = String(wd).toInt();
                  int h = String(godzina).toInt();
                  int m = String(minuta).toInt();
                    //tt.hour  = h; 
                    //tt.min   = m;
                    //tt.mday  = d;
                  Serial.print(d);Serial.print("/");Serial.print(h);Serial.print("/");Serial.println(m);
                  epochTime = getTimeStamp();
                  bool czy=compareTimes(0,epochTime-20, epochTime+10);
                  Serial.print("Epoch Time: ");
                  Serial.println(epochTime);        
                  Serial.println("***--------------------");   

                  char timeHour[3];
                  strftime(timeHour,3, "%H", &timeinfo);
                  Serial.println(timeHour);
                  char timeWeekDay[10];
                  strftime(timeWeekDay,10, "%A", &timeinfo);
                  Serial.println(timeWeekDay);
                  Serial.println();
                  
              
            }
*/            
            /*TIME*/     
