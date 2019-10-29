int dataPin = 13;
int clockPin = 12;
int latchPin = 11;
void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}
void ispisBroja(int osamBitova, int cetiriBita){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, osamBitova);
  shiftOut(dataPin, clockPin, LSBFIRST, cetiriBita);
  digitalWrite(latchPin, HIGH);
}

void deljenjeNaBitove(int broj, int* osam, int* cetiri){
  int i;
  *osam = 0;
  *cetiri = 0;
  for(i=0;i<8;i++){
      if(broj%2==1)bitWrite(*osam, i, 1);
      broj = broj / 2;
    }
    
  for(i=0;i<4;i++){
      if(broj%2==1)bitWrite(*cetiri, i, 1);
      broj = broj / 2;
    }
  
}
void loop() {
  int cetiri,osam;
  for(int i=0;i<=4095;i++){
      deljenjeNaBitove(i, &osam, &cetiri);
      Serial.println(i);
      ispisBroja(osam, cetiri);
      delay(10);
    }
  for(int i=4095;i>=0;i--){
      deljenjeNaBitove(i, &osam, &cetiri);
      Serial.println(i);
      ispisBroja(osam,cetiri);
      delay(10);
    }
}
