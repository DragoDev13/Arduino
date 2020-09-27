#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DUGME 2 //pin na kome je dugme

volatile int interupt = 0;
int brojac = 0;

void isr_funckija(){
    interupt = 1;
  }

void setup() {
  pinMode(DUGME, INPUT);
  attachInterrupt(digitalPinToInterrupt(DUGME),isr_funckija,RISING);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.print("Crtanje");
  display.setCursor(25, 40);
  display.print("oblika");
  display.display();

}

void loop() {
  if(interupt){
    switch(brojac){
      case 0:
            display.clearDisplay();
            display.drawRect(30, 10, 50, 30, WHITE);
            display.display();
            break;
      case 1:
            display.clearDisplay();
            display.fillRect(30, 10, 50, 30, WHITE);
            display.display();
            break;
     case 2:
            display.clearDisplay();
            display.drawCircle(64, 32, 10, WHITE);
            display.display();
            break;
     case 3:
            display.clearDisplay();
            display.fillCircle(64, 32, 10, WHITE);
            display.display();
            break;
     case 4:
            display.clearDisplay();
            display.drawTriangle(10, 10, 55, 20, 5, 40, WHITE);
            display.display();
            break;
     case 5:
            display.clearDisplay();
            display.fillTriangle(10, 10, 55, 20, 5, 40, WHITE);
            display.display();
            break;
    }
    brojac++;
    if(brojac==6)brojac=0;
    delay(200);
    interupt = 0;
    }
}
