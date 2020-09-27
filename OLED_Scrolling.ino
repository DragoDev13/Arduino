#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Fonts/FreeMonoOblique12pt7b.h> //header fonta

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DUGME 2 //pin na kome je dugme

volatile int interupt = 0;
int pinterupt = interupt; //pomocna promenljiva
int textSize = 1;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void isr_funckija(){
    interupt = 1;
  }
  
void setup() {
  pinMode(DUGME, INPUT);
  attachInterrupt(digitalPinToInterrupt(DUGME),isr_funckija,RISING);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { //provera da li sve uredu radi sa displayom
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); //u slucaju da nije beskonacan loop
  }
  delay(2000);
  //display.setFont(&FreeMonoOblique12pt7b); //setovanje fonta
  display.clearDisplay();
  display.setTextSize(1); // ako se setuje font ovo linija se mora komentovati
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("REC");
  display.startscrollright(0x00, 0x0F);
  display.display();
}

void loop() {
  if(interupt){
    textSize++;
    if(textSize<=8){
      display.setCursor(0, 0);
      display.setTextSize(textSize);
      display.clearDisplay();
      display.print("REC");
      display.display();
      }
    else{
      textSize=1;
      display.setCursor(0, 0);
      display.setTextSize(textSize);
      display.clearDisplay();
      display.print("REC");
      display.display();

      }
    if(pinterupt){
      display.stopscroll();
      display.startscrollleft(0x00, 0x0F);
      }
    else{
      display.stopscroll();
      display.startscrollright(0x00, 0x0F);
      }
      pinterupt = !pinterupt;
      delay(200); //debounce
      interupt = 0;
    }
}
