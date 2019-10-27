#include <elapsedMillis.h>
#define BOUNCE_LOCK_OUT
#include <Bounce2.h>
#include <DS3231.h>
#include <TM1637Display.h>

#define CLK 13
#define DIO 12
#define SDA 10
#define SCL 11
#define DUGME 8

DS3231 rtc(SDA, SCL);
TM1637Display display(CLK, DIO);
Bounce dugme = Bounce();

int stanjeDugmeta = 0;

void ciscenjeEkrana(){
  uint8_t data[] = {0x0, 0x0, 0x0, 0x0};
  display.setSegments(data);  
}

void stavljanjeVremenaNaDS(){
  rtc.setDOW(THURSDAY); 
  rtc.setTime(15, 56, 0);    // sat, minut, sekunda
  rtc.setDate(27, 10, 2019);  // dan, mesec, godina
  
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
  delaySaCitanjemDugmeta(500);
  if(mesec<10){
      display.showNumberDec(0, false, 1, 0);
      display.showNumberDec(mesec, false, 1, 1);
    }else display.showNumberDec(mesec, false, 2, 0);
  if(dan<10){
      display.showNumberDec(0, false, 1, 2);
      display.showNumberDec(dan, false ,1, 3);
    }else display.showNumberDec(dan, false, 2, 2);
    delaySaCitanjemDugmeta(500);
    
}

void menjanjeStanjaDugmeta(){
  if(dugme.fell() && stanjeDugmeta ==0)stanjeDugmeta = 1;
  else if(dugme.fell() && stanjeDugmeta == 1)stanjeDugmeta = 0;
}

void delaySaCitanjemDugmeta(int ddelay){
  elapsedMillis elapsed;
  while(elapsed<ddelay){
     dugme.update();
     menjanjeStanjaDugmeta();
    }     
}

void setup(){
  pinMode(DUGME, INPUT_PULLUP);
  dugme.attach(DUGME);
  dugme.interval(5);
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
  dugme.update();
  menjanjeStanjaDugmeta();
  if(stanjeDugmeta==0)ispisMinutaISekundi(minut, sekunda);
  else ispisDatuma(godina, mesec, dan);
  Serial.println("-----------------------");
}
