#include <elapsedMillis.h>
#include <Bounce2.h>

int dataPin1 = 13;
int clockPin1 = 12;
int latchPin1 = 11;

int dataPin2 = 10;
int clockPin2 = 9;
int latchPin2 = 8;

int broj = 0;

#define startPin 7
#define incrementPin 6

int stanjeStart = 1;
int stanjeIncrement = 1;

Bounce start = Bounce();
Bounce increment = Bounce();
/*
0 - 00111111 - 63
1 - 00001100 - 12
2 - 01011011 - 91
3 - 01011110 - 94
4 - 01101100 - 108
5 - 01110110 - 118
6 - 01110111 - 119
7 - 00011100 - 28
8 - 01111111 - 127
9 - 01111110 - 126
*/
int brojevi[10] = {63, 12, 91, 94, 108, 118, 119, 28, 127, 126};

void prikazBroja(){
  int desetica = broj/10;
  int jedinica = broj%10;
  digitalWrite(latchPin1, LOW);
  digitalWrite(latchPin2, LOW);
  shiftOut(dataPin1, clockPin1, MSBFIRST, brojevi[desetica]);
  shiftOut(dataPin2, clockPin2, MSBFIRST, brojevi[jedinica]);
  digitalWrite(latchPin1, HIGH);
  digitalWrite(latchPin2, HIGH);
}

void menjanjeStanjaIncrementDugmeta(){
  if(increment.fell() && stanjeIncrement == 0)stanjeIncrement = 1;
  else if(increment.fell() && stanjeIncrement == 1)stanjeIncrement = 0;
}

void menjanjeStanjaStartDugmeta(){
  if(start.fell() && stanjeStart ==0)stanjeStart = 1;
  else if(start.fell() && stanjeStart == 1)stanjeStart = 0;
}

void delaySaCitanjemDugmeta(int ddelay){
  elapsedMillis elapsed;
  while(elapsed<ddelay){
     start.update();
     increment.update();
     menjanjeStanjaStartDugmeta();
     menjanjeStanjaIncrementDugmeta();
    }     
}

void menjanjeBroja(){
    if(stanjeStart == 0)return;
    else{
        if(stanjeIncrement == 1 && broj<99)broj++;
        else if(broj>0 && stanjeIncrement == 0)broj--;
      }
}

void setup() {
  pinMode(latchPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(startPin, INPUT_PULLUP);
  start.attach(startPin);
  start.interval(5);
  pinMode(incrementPin, INPUT_PULLUP);
  increment.attach(incrementPin);
  increment.interval(5);
  Serial.begin(9600);
}

void loop(){
  prikazBroja();
  menjanjeBroja();
  Serial.println(broj);
  delaySaCitanjemDugmeta(250);
}
