#include "time.h"

//const char* ntpServer = "3.pl.pool.ntp.org";
const long  gmtOffset_sec      = 3600;   //12600;
const int   daylightOffset_sec = 3600;
/*
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
*/

/*** Function declaration ***/
//String getRealTime();



void TimersSetup(){
  configTime(gmtOffset_sec, daylightOffset_sec, "0.pl.pool.ntp.org","1.pl.pool.ntp.org","3.pl.pool.ntp.org");
  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //epochTime = getTimeStamp();
}

long getTimeStamp() { //millisek
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      return(0);
    }
    time(&now);
    return now;
}
  
String pad(int liczba){
  String p="";
  if (liczba<10) p="0";
  return p+String(liczba);
}

String getRealTime(){
      //Serial.println("#43-czas.h---showTime()---");
       struct tm timeinfo;
       if(!getLocalTime(&timeinfo)){
          Serial.println("Failed to obtain time");
          return "";
      }
      int godzina = timeinfo.tm_hour;
      int minuta  = timeinfo.tm_min;
      int sekunda = timeinfo.tm_sec;
      //Serial.print("time="); Serial.print(godzina);   Serial.print(":");  Serial.println(minuta);
      return pad(godzina)+":"+pad(minuta)+":"+pad(sekunda);
       //epochTime = getTimeStamp();
}

/*






*/
/*
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
      int dayflag= timeinfo.tm_isdst;   //Daylight Saving Time flag
      snprintf(buffer, 16, daysOFweek[weekDay]);
      return pad(rok)+"-"+pad(miesiac)+"-"+pad(dzien)+", "+pad(godzina)+":"+pad(minuta)+":"+pad(sekunda)+", "+ String(buffer);
}
*/



//unsigned long licznik=0;
 //long epochTime; 
 //long TcurrentMillis  = 0;
 //long TpreviousMillis = 0;
 //long TintervalMillis = 1000*60;

/*
   %a Abbreviated weekday name
   %A Full weekday name
   %b Abbreviated month name
   %B Full month name
   %c Date and time representation for your locale
   %d Day of month as a decimal number (01−31)
   %H Hour in 24-hour format (00−23)
   %I Hour in 12-hour format (01−12)
   %j Day of year as decimal number (001−366)
   %m Month as decimal number (01−12)
   %M Minute as decimal number (00−59)
   %p Current locale's A.M./P.M. indicator for 12−hour clock
   %S Second as decimal number (00−59)
   %U Week of year as decimal number,  Sunday as first day of week (00−51)
   %w Weekday as decimal number (0−6; Sunday is 0)
   %W Week of year as decimal number, Monday as first day of week (00−51)
   %x Date representation for current locale
   %X Time representation for current locale
   %y Year without century, as decimal number (00−99)
   %Y Year with century, as decimal number
   %z %Z Time-zone name or abbreviation, (no characters if time zone is unknown)
   %% Percent sign
   You can include text literals (such as spaces and colons) to make a neater display or for padding between adjoining columns.
   You can suppress the display of leading zeroes  by using the "#" character  (%#d, %#H, %#I, %#j, %#m, %#M, %#S, %#U, %#w, %#W, %#y, %#Y)
*/
