#include <TM1637Display.h>
#include <Wire.h>
#include <DS3231.h>
#include <Bounce2.h>
#include <elapsedMillis.h>
#include <TimerOne.h> // koriscenje timer interupta

#define BOUNCE_LOCK_OUT
#define DIO 13
#define CLK 12
#define BUZZER 6

TM1637Display display(CLK, DIO);
DS3231 clock;
RTCDateTime dt;
Bounce dugme = Bounce();

int stanjeDugmeta = 0;

void ciscenjeEkrana(){
  uint8_t data[] = {0x0, 0x0, 0x0, 0x0};
  display.setSegments(data);  
}

void ispisMinutaISekundi(int minuta,int sekunda){
    int broj = minuta * 100 + sekunda;
    display.showNumberDecEx(broj, 64, true);
}

void ispisDatuma(int godina, int mesec, int dan){
    if(mesec<10){
            display.showNumberDec(0, false, 1, 0);
            display.showNumberDec(mesec, false, 1, 1);
        }else display.showNumberDec(mesec, false, 2, 0);
    if(dan<10){
            display.showNumberDec(0, false, 1, 2);
            display.showNumberDec(dan, false ,1, 3);
        }else display.showNumberDec(dan, false, 2, 2);
    delaySaCitanjemDugmeta(750);
    if(stanjeDugmeta==0)return;
    display.showNumberDec(godina, false, 4, 0);
    delaySaCitanjemDugmeta(750);
}

void menjanjeStanjaDugmeta(){
  if(dugme.fell() && stanjeDugmeta == 0)stanjeDugmeta = 1;
  else if(dugme.fell() && stanjeDugmeta == 1)stanjeDugmeta = 0;
}
void delaySaCitanjemDugmeta(int ddelay){
  elapsedMillis elapsed;
  while(elapsed<ddelay){
     if(dugme.update()){
         if(clock.isAlarm1())Serial.println("ALARM RADI");
         menjanjeStanjaDugmeta();
         return;
     }
     
    }     
}

void gasenjeAlarma(){
    Timer1.stop();
    noTone(BUZZER);
}

void setup() {
    Wire.begin();
    Serial.begin(9600);
    clock.begin();
    clock.armAlarm1(false);
    clock.armAlarm2(false);
    clock.clearAlarm1();
    clock.clearAlarm2();
    //clock.setDateTime(__DATE__, __TIME__); postavlja vreme sa racunaru u trenutku upload-a
    ciscenjeEkrana();
    display.setBrightness(7);
    pinMode(8, INPUT_PULLUP);
    dugme.attach(8);
    dugme.interval(2);
    Timer1.attachInterrupt(gasenjeAlarma);
    Timer1.initialize(2000); //koliko dugo ce se alarm cuti u mikrosekundama
    clock.setAlarm1(0, 0, 0, 3, DS3231_MATCH_S);
}

void loop() {
    dt = clock.getDateTime();
    dugme.update();
    menjanjeStanjaDugmeta();
    if(stanjeDugmeta==0)ispisMinutaISekundi(dt.minute, dt.second);
    else ispisDatuma(dt.year, dt.month, dt.day);
    dugme.update();
    menjanjeStanjaDugmeta();

    if(clock.isAlarm1()){
        Serial.println("ALARM ZVONI");
        tone(BUZZER, 2000);
        Timer1.start();
    }
}
