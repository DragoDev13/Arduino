#include "Arduino.h"
#include <Bounce2.h>
#include <DS3231.h>
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include <elapsedMillis.h>
#define BUTTON 7
#define DIODA 8
#define ECHO 9
#define TRIG 10
#define BOUNCE_LOCK_OUT
Bounce dugme = Bounce();
DS3231 clock;
LiquidCrystal_I2C lcd(0x3F, 20, 4);
RTCDateTime dt;

bool stanjeDugmeta = false;

void ocistiDisplej(){
  lcd.setCursor(0,0);
  for(int i=0;i<20;i++)lcd.print(" ");
  lcd.setCursor(0,1);
  for(int i=0;i<20;i++)lcd.print(" ");
  lcd.setCursor(0,2);
  for(int i=0;i<20;i++)lcd.print(" ");
  lcd.setCursor(0,3);
  for(int i=0;i<20;i++)lcd.print(" ");    
}

double udaljenostObjekta(){
    double udaljenost;
    double vreme;
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    vreme = pulseIn(ECHO, HIGH);

    udaljenost = vreme * 0.034/2;

    return udaljenost;
}

void ispisNaDispleju(){
    char buf[20+1];
    lcd.setCursor(0,0);
    //ispis vremena u prvom redu
    if(stanjeDugmeta){
        sprintf(buf, "%02d:%02d", dt.hour, dt.minute);
        lcd.print(buf);
    }
    else{
        if(dt.hour==00 || dt.hour==24)sprintf(buf, "12:%02d AM", dt.minute);
        else if(dt.hour < 13)sprintf(buf, "%02d:%02d", dt.hour, dt.minute);
        else if(dt.hour < 24)sprintf(buf, "%02d:%02d", dt.hour - 12, dt.minute);
        lcd.print(buf);
    }

    //ispis udaljenosti objekta
    double udaljenost = udaljenostObjekta();
    lcd.setCursor(0,1);
    if(stanjeDugmeta && udaljenost<=450){
        // kada je objekat jako daleko ili tacno ispred senzora vracace vrednosti preko 450
        // a u dokmentaciji pise da je 450cm max sto senzor moze
        lcd.print("Udaljenost: ");
        lcd.print(udaljenost);
        lcd.print("cm     ");
    }else for(int i=0;i<21;i++)lcd.print(" ");
    //ispis datuma
    lcd.setCursor(0,2);
    sprintf(buf, "%02d.%02d.%04d", dt.day, dt.month, dt.year);
    lcd.print(buf);
    //ispis dana u nedelji
    lcd.setCursor(0,3);
    if(dt.dayOfWeek==1)lcd.print("Ponedeljak      ");
    else if(dt.dayOfWeek==2)lcd.print("Utorak      ");
    else if(dt.dayOfWeek==3)lcd.print("Sreda      ");
    else if(dt.dayOfWeek==4)lcd.print("Cetvrtak      ");
    else if(dt.dayOfWeek==5)lcd.print("Petak      ");
    else if(dt.dayOfWeek==6)lcd.print("Subota      ");
    else printf("Nedelja      ");
}

void delaySaCitanjemDugmeta(int ddelay){
  elapsedMillis elapsed;
  while(elapsed<ddelay){
    if(dugme.update() && dugme.rose()){
         stanjeDugmeta = !stanjeDugmeta;
         if(stanjeDugmeta)digitalWrite(DIODA, HIGH);
         else digitalWrite(DIODA, LOW);
        }
    }     
}

void setup() {
    Wire.begin();
    clock.begin();
    //clock.setDateTime(__DATE__, __TIME__); //postavlja vreme sa racunaru u trenutku upload-a
    pinMode(BUTTON, INPUT_PULLUP);
    dugme.attach(BUTTON);
    dugme.interval(5);
    lcd.begin();
    lcd.backlight();
    ocistiDisplej();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(DIODA, OUTPUT);
    digitalWrite(DIODA, LOW);
}

void loop() {
    dugme.update();
    dt = clock.getDateTime();
    ispisNaDispleju();
    delaySaCitanjemDugmeta(100);
}