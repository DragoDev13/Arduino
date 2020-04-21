#include "LedControl.h" /// https://github.com/wayoda/LedControl/
#include "binary.h"
#include <TM1638.h>
#include <TM1638QYF.h> ///https://github.com/rjbatista/tm1638-library
#define CLK 13
#define DIN 12
#define CS 11 // dot matrix
#define STB 10 // QYF

LedControl lc=LedControl(DIN,CLK,CS,1);
TM1638QYF module(DIN, CLK, STB);

byte nula[] = {
    B00111111,
    B00100001,
    B00100001,
    B00100001,
    B00100001,
    B00100001,
    B00100001,
    B00111111
};

byte jedan[] = {
  B00011,
  B00101,
  B01001,
  B10001,
  B00001,
  B00001,
  B00001,
  B00001
};

byte dva[] = {
  B0011110,
  B0100001,
  B1000010,
  B0000100,
  B0001000,
  B0010000,
  B0100000,
  B1111111
};

byte tri[] = {
  B0001110,
  B0000001,
  B0000001,
  B0001111,
  B0000001,
  B0000001,
  B0000001,
  B0001110
};

word mode;

void setup() {
    lc.shutdown(0,false);
    lc.setIntensity(0,8);
    lc.clearDisplay(0);  
}
void ispis(byte* array){ // ispis za dot matrix
    lc.setRow(0,0,array[0]);
    lc.setRow(0,1,array[1]);
    lc.setRow(0,2,array[2]);
    lc.setRow(0,3,array[3]);
    lc.setRow(0,4,array[4]);
    lc.setRow(0,5,array[5]);
    lc.setRow(0,6,array[6]);
    lc.setRow(0,7,array[7]);
}

void update(TM1638QYF* module, word* mode) { // proverava da li je neko dugme stisnuto
    word buttons = module->getButtons();
    if (buttons != 0) {
        *mode = buttons >> 1;
        if (*mode < 128) {
            ispis(ciscenje);
            module->clearDisplay();
            delay(100);
        }
  }
  switch (*mode) {//u zavisnosti od kojeg dugmeta je stisnuto
    case 0:
      module->setDisplayToDecNumber(1, 1);
      ispis(jedan);
      break;
    case 1:
      module->setDisplayToDecNumber(2, 1);
      ispis(dva);
      break;
    case 2:
      module->setDisplayToDecNumber(3, 1);
      ispis(tri);
      break;
    default:
      module->setDisplayToDecNumber(0, 1); // svako dugme koje nije definisano samo ispisuje nulu
      ispis(nula);
  }
}

void loop(){
    update(&module, &mode);
}