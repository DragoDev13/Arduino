#include <avr/io.h>
#include <avr/interrupt.h>
volatile int dodatnoVreme = 0; // mnozi sa vremenom
int vreme = 600; // menjanje vrednostri menja koliko time counter broji
const int buzzer = 9;
volatile int flag = 0; //sluzi za provere time coutnera
volatile bool upaljen = true; //paljenje i gasenje dugmeta
volatile int vrednost; // vrednost sa potenciometra
int nizTranzistora[3] = {6, 7, 8};
int dekoder[4] = {10, 11, 12, 13};
volatile bool temp_b = upaljen; //"prirodno" stanje buzzera
const int dioda2 = 5; //.
const int dioda1 = 4; //-
// S = 5, 0101
// O = 0, 0000
// TIMSK0 = (1 << OCIE0A); // ovo se koristi za aktiviranje
// 11, 8, 7 katode
/*
    morse code
    s = ...
    o = ---
  */
    /*
  1 - dole levo
  2 - dole srednja
  3 - tacka
  4 - dole desno
  5 - srednja
  6 - gore desno
  9 - gore levo
  10 - gornja srednja
*/
void setup (){
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  Serial.begin (115200);
  attachInterrupt(digitalPinToInterrupt(2), pritisakDugmeta, RISING);
  adc_init();
  ADCSRA |= 1 << ADSC;
  TCCR0A = (1 << WGM01);
  OCR0A = 15640;
  TCCR0B = (1 << CS02) | (1 << CS00);
  pinMode(nizTranzistora[0],OUTPUT);
  pinMode(nizTranzistora[1],OUTPUT);
  pinMode(nizTranzistora[2],OUTPUT);
  pinMode(dekoder[0],OUTPUT);
  pinMode(dekoder[1],OUTPUT);
  pinMode(dekoder[2],OUTPUT);
  pinMode(dekoder[3],OUTPUT);
  digitalWrite(nizTranzistora[0], LOW);
  digitalWrite(nizTranzistora[1], LOW);
  digitalWrite(nizTranzistora[2], LOW);
  digitalWrite(dekoder[0], LOW);
  digitalWrite(dekoder[1], LOW);
  digitalWrite(dekoder[2], LOW);
  digitalWrite(dekoder[3], LOW);
  digitalWrite(dioda1, LOW);
  digitalWrite(dioda2, LOW);
}
void pritisakDugmeta(){
  temp_b = !temp_b;
}
ISR(ADC_vect){
  vrednost = ADCH;
  if(temp_b)ADCSRA |= 1<<ADSC;
  if(temp_b && upaljen)tone(buzzer, vrednost*80);
}
ISR(TIMER0_COMPA_vect){
  dodatnoVreme++;
  if(dodatnoVreme>=vreme){
      dodatnoVreme = 0;
      flag = 1;
    }
}
void adc_init(){
  ADCSRA |= (1<<ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADMUX |= (1 << REFS0) | (1 << ADLAR);
}
void postavljanjeSOS(int seg1,int seg2,int seg3){
  // -1 ugasen segment, 0 - O na segmetnu, sve ostalo S na segementu
if(seg1!=-1){
        if(seg1==0){
          digitalWrite(dekoder[0], LOW);
          digitalWrite(dekoder[1], LOW);
          digitalWrite(dekoder[2], LOW);
          digitalWrite(dekoder[3], LOW);
        }
        else{
            digitalWrite(dekoder[0], HIGH);
            digitalWrite(dekoder[1], LOW);
            digitalWrite(dekoder[2], HIGH);
            digitalWrite(dekoder[3], LOW);
          }
        digitalWrite(nizTranzistora[0], HIGH);
        digitalWrite(nizTranzistora[0], LOW);
    }
if(seg2!=-1){
        if(seg2==0){
          digitalWrite(dekoder[0], LOW);
          digitalWrite(dekoder[1], LOW);
          digitalWrite(dekoder[2], LOW);
          digitalWrite(dekoder[3], LOW);
        }
        else{
            digitalWrite(dekoder[0], HIGH);
            digitalWrite(dekoder[1], LOW);
            digitalWrite(dekoder[2], HIGH);
            digitalWrite(dekoder[3], LOW);
          }
        digitalWrite(nizTranzistora[1], HIGH);
        digitalWrite(nizTranzistora[1], LOW);
    }
if(seg3!=-1){
        if(seg3==0){
          digitalWrite(dekoder[0], LOW);
          digitalWrite(dekoder[1], LOW);
          digitalWrite(dekoder[2], LOW);
          digitalWrite(dekoder[3], LOW);
        }
        else{
            digitalWrite(dekoder[0], HIGH);
            digitalWrite(dekoder[1], LOW);
            digitalWrite(dekoder[2], HIGH);
            digitalWrite(dekoder[3], LOW);
          }
        digitalWrite(nizTranzistora[2], HIGH);
        digitalWrite(nizTranzistora[2], LOW);
    }
}
void forceGasenjeBuzzera(){
    ADCSRA |= 0<<ADSC;
    upaljen = false;
    noTone(buzzer);
}
void vracanjeBuzzera(){
    if(temp_b){
      upaljen = true;
      ADCSRA |= 1<<ADSC;
      upaljen = temp_b;
      tone(buzzer, vrednost*80);
    }
}
void tackaMorse(){
  vreme = 300;
  Serial.println("S - morse");
  digitalWrite(dioda2, HIGH);
  digitalWrite(dioda1, LOW);
  vracanjeBuzzera();
  TIMSK0 = (1 << OCIE0A);
  while(flag==0)postavljanjeSOS(5, 0, 5);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  forceGasenjeBuzzera();
  digitalWrite(dioda2, LOW);
  Serial.println("S - morse off");
  // pauza izmedju znakova
  vreme = 600;
  TIMSK0 = (1 << OCIE0A);
  while(flag==0){}
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
}
void crtaMorse(){
  vreme = 1000;
  Serial.println("O - morse");
  digitalWrite(dioda1, HIGH);
  digitalWrite(dioda2, LOW);
  vracanjeBuzzera();
  TIMSK0 = (1 << OCIE0A);
  while(flag==0)postavljanjeSOS(5, 0, 5);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  forceGasenjeBuzzera();
  digitalWrite(dioda1, LOW);
  Serial.println("O - morse off");
  // pauza izmedju znakova
  vreme = 600;
  TIMSK0 = (1 << OCIE0A);
  while(flag==0){}
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
}
void loop () {
  forceGasenjeBuzzera();
  vreme = 600;
  TIMSK0 = (1 << OCIE0A);
  vracanjeBuzzera();
  while(flag==0)postavljanjeSOS(5, -1, -1);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  forceGasenjeBuzzera();
  Serial.println("S");
  TIMSK0 = (1 << OCIE0A);
  while(flag==0)postavljanjeSOS(5, -1, -1);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  //-----------------------------------
  vreme = 600;
  TIMSK0 = (1 << OCIE0A);
  vracanjeBuzzera();
  while(flag==0)postavljanjeSOS(5, 0, -1);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  forceGasenjeBuzzera();
  Serial.println("SO");
  TIMSK0 = (1 << OCIE0A);
  while(flag==0)postavljanjeSOS(5, 0, -1);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  //-----------------------------------
  vreme = 600;
  TIMSK0 = (1 << OCIE0A);
  vracanjeBuzzera();
  while(flag==0)postavljanjeSOS(5, 0, 5);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  forceGasenjeBuzzera();
  Serial.println("SOS");
  TIMSK0 = (1 << OCIE0A);
  while(flag==0)postavljanjeSOS(5, 0, 5);
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  //-----------------------------------
  tackaMorse();
  tackaMorse();
  tackaMorse();
  //-----------------------------------
  crtaMorse();
  crtaMorse();
  crtaMorse();
  //-----------------------------------
  tackaMorse();
  tackaMorse();
  tackaMorse();
  //-----------------------------------
  Serial.println("Pauza do sledeceg puta.");
  vreme = 600;
  TIMSK0 = (1 << OCIE0A);
  while(flag==0){}
  TIMSK0 = (0 << OCIE0A);
  flag = 0;
  Serial.println("Prosla pauza.");
}
