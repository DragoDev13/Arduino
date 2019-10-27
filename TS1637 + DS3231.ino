#include <DS3231.h>
#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 13
#define DIO 12
#define SDA 10
#define SCL 11
DS3231 rtc(SDA, SCL);
TM1637Display display(CLK, DIO);
void ciscenjeEkrana(){
  uint8_t data[] = {0x0, 0x0, 0x0, 0x0};
  display.setSegments(data);  
}
void stavljanjeVremenaNaDS(){
  rtc.setDOW(THURSDAY); 
  rtc.setTime(9, 41, 20);    // sat, minut, sekunda
  rtc.setDate(3, 10, 2019);  // dan, mesec, godina
  
}
void kopiranjeStringa(char* string, char* original){
  while(*original!='\0'){
      *string = *original;
      string++;
      original++;
    }  
   *string = '\0';
}
void datumIzStringa(char* datum, int* dan, int* mesec, int* godina){
  char* pomocni;
  pomocni = datum;
  while(*pomocni!='\0'){
    if(*pomocni=='.')*pomocni = ' ';
    pomocni++;
  }
  sscanf(datum, "%d %d %d",dan,mesec,godina);
}

void vremeIzStringa(char* vreme, int* sat, int* minuta, int* sekunda){
  char* pomocni;
  pomocni = vreme;
  while(*pomocni!='\0'){
    if(*pomocni==':')*pomocni = ' ';
    pomocni++;
  }
  sscanf(vreme, "%d %d %d",sat,minuta,sekunda);
  
}
void ispisMinutaISekundi(int minuta,int sekunda){
  int broj = minuta * 100 + sekunda;
  display.showNumberDecEx(broj, 64, true);
}
void ispisDatuma(int godina, int mesec, int dan){
  
  display.showNumberDec(godina, false, 4, 0);
  delay(2000);
  if(mesec<10){
      display.showNumberDec(0, false, 1, 0);
      display.showNumberDec(mesec, false, 1, 1);
    }else display.showNumberDec(mesec, false, 2, 0);
  if(dan<10){
      display.showNumberDec(0, false, 1, 2);
      display.showNumberDec(dan, false ,1, 3);
    }else display.showNumberDec(dan, false, 2, 2);
    delay(2000);
}
void setup(){
  rtc.begin();
  ciscenjeEkrana();
  display.setBrightness(7);
  Serial.begin(9600);
  //stavljanjeVremenaNaDS();
 }

void loop(){
  Serial.println("-----------------------");
  Serial.println(rtc.getDOWStr());
  Serial.println(rtc.getDateStr());
  Serial.println(rtc.getTimeStr());
  Serial.println("-----------------------");
  char datum [20] = {'\0'};
  char vreme [20] = {'\0'};
  int sat, minut, sekunda;
  int dan,mesec,godina;
  kopiranjeStringa(datum, rtc.getDateStr());
  kopiranjeStringa(vreme, rtc.getTimeStr());
  datumIzStringa(datum, &dan, &mesec, &godina);
  vremeIzStringa(vreme, &sat, &minut, &sekunda);
  ispisMinutaISekundi(minut, sekunda);
  //ispisDatuma(godina, mesec, dan);
  Serial.println("Pisanje na displej");
  Serial.println("-----------------------");
}
