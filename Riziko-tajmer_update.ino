#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>

// Ovo su fabričke konfiguracije za pokretanje shield modula
LiquidCrystal lcd(8,9,4,5,6,7); 
volatile int flag=1; //koristi se za tajmer
// Definisanje slike života (tenkova) na displeju
byte tank_p1[8] =
{
        B00000,
        B00000,
        B00000,
        B00001,      
        B00010,
        B01110,
        B11111,
        B11111
};

byte tank_p2[8] =
{
        B00000,
        B00000,
        B00000,
        B10000,      
        B01000,
        B01110,
        B11111,
        B11111
};
ISR(TIMER1_COMPA_vect){
  flag=0;
}
void delajPrekoInterrupta(int vremeUMiliSekundama){
  float vremeUSekundama = (float)vremeUMiliSekundama / 1000.0;
  float frekvencija = (float)1 / (float)vremeUSekundama;
  float matchRegistar = (float)16000000 / (float)((float)1024 * frekvencija) - 1;
  OCR1A = (int)matchRegistar;
  flag = 1;
  TIMSK1 = (1 << OCIE1A); //paljenje tajmera
  while(flag){};
  TIMSK1 = (0 << OCIE1A); //gasenje tajmera
}
// Funkcija za printovanje broja bacanja kockica pre početka svake runde 
void num_of_dice(int num) {

  lcd.setCursor(0,0);    
  lcd.print(num);
  lcd.setCursor(15,0);
  lcd.print(num);
}

//Funkcija za printovanje broja života oba igrača tokom igre
void lives(int player, int num){

  if (player == 1){
    // U zavisnosti od broja života se menja mesto printovanja života (prvi igrač)
    switch(num){
      case 1: lcd.setCursor(6,0); 
              lcd.write(1); 
              break;
      case 2: lcd.setCursor(5,0); 
              lcd.write(1); 
              lcd.setCursor(6,0); 
              lcd.write(1); 
              break;
      case 3: lcd.setCursor(4,0); 
              lcd.write(1); 
              lcd.setCursor(5,0); 
              lcd.write(1); 
              lcd.setCursor(6,0); 
              lcd.write(1); 
              break;
    }
  } else {
    switch(num) {
      // U zavisnosti od broja zivota se menja mesto printovanja života (drugi igrač)
      case 1: lcd.setCursor(8,0); 
              lcd.write(2); 
              break;
      case 2: lcd.setCursor(8,0); 
              lcd.write(2); 
              lcd.setCursor(9,0); 
              lcd.write(2); 
              break;
      case 3: lcd.setCursor(8,0); 
              lcd.write(2); 
              lcd.setCursor(9,0); 
              lcd.write(2); 
              lcd.setCursor(10,0); 
              lcd.write(2); 
              break;
    }
  }
}

// Aktiviranje tastature
DFR_LCD_Keypad keypad(A0, &lcd);

int last_key, key;                     // Poslednji pritisnut i trenutni pritisnut taster
int dice = 1;                             // Promenljiva za broj bacanja kockica sa kojom prolazimo kroz faze koda
int lives_1 = 3, lives_2 = 3;     // Promenljive za brojeve života igraca
int select = 0;                          // Trenutna faza koda u odnosu na izabrani broj bacanja kockica 

int num_choice_1[8] = {0,0,0,0,0,0,0,0};  // Čuvanje svih dobijenih kockica u niz prvog igraca
int num_choice_2[8] = {0,0,0,0,0,0,0,0};  // Čuvanje svih dobijenih kockica u niz drugog igraca
int num = 0;                                              // Trenutni broj kockice
int sum_1 = 0;                                          // Zbir svih kockica prvog igrača
int sum_2 = 0;                                          // Zbir svih kockica drugog igrača
int br = 0;

void setup()
{
  //podesavanje tajmera
  cli(); //gasenje svih interuptova
  TCCR1A = 0; 
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624; //default jedna sekunda
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  sei(); //paljenje svih interuptova
  //-----------------------------------------
  lcd.begin(16,2);                                      // Pokrećemo lcd
  lcd.clear();                                              // Čistimo ekran na prazan
  lcd.setCursor(0,0);
  lcd.createChar(1, tank_p1);                   // Pravimo karaktere života (tenkova)
  lcd.createChar(2, tank_p2);
  Serial.begin(9600);
  delajPrekoInterrupta(200);
}

void loop()
{
  last_key = keypad.get_last_key();       // Funkcije za lakše reagovanje na tastere tastature
  key      = keypad.read_key();

  if(select==1 || select==2)
  {
     if (select==2){
        num_choice_2[br] = num;

        for (int m = 0; m < dice; m++) {
           lcd.setCursor(m,1);
           lcd.print(num_choice_1[m]);
         }
      
         for (int m = 0; m < br; m++) {
           lcd.setCursor(15-m,1);
           lcd.print(num_choice_2[m]);
         }
         lcd.setCursor(15-br,1);
         lcd.print(num);
     }
     else
     {     
        num_choice_1[br] = num;

        for (int m = 0; m < br; m++) {
          lcd.setCursor(m,1);
          lcd.print(num_choice_1[m]);
         }
         lcd.setCursor(br,1);
         lcd.print(num);
     }
   
     num++;
     if (num == 10) num = 0;
  }

  else if(select==3)
  {
    delajPrekoInterrupta(1000);     //delay(1000);
    lcd.clear();                
    lcd.setCursor(7,0);       
    lcd.print("I");
    lives(1,lives_1);           // Ažuriranje svih podataka (života, bacanja)
    lives(2,lives_2);
    num_of_dice(dice);

    Serial.print(num_choice_1[0]);

    // Ispisivanje zbira kockica
    for (int m = 0; m < dice; m++){
        sum_1 += num_choice_1[m];
      }
      
    for (int m = 0; m < dice; m++){
        sum_2 += num_choice_2[m];
    }

    Serial.print(sum_1);

    lcd.setCursor(1,1);
    lcd.print(sum_1);
    lcd.setCursor(14,1);
    lcd.print(sum_2);
    delajPrekoInterrupta(2500);   //delay(2500);
    
    if (sum_2 > sum_1){         // U zavisnosti ko ima manji zbir taj gub i život
      lives_1--;
    }else if (sum_2 < sum_1){
      lives_2--;
    }else if (sum_1 == sum_2){  // Ako oba igrača imaju isti zbir, oba gube po život
      lives_1--;
      lives_2--;
    }
    
    select = 0;               // Resetuje se faza na nultu fazu
  }
  else
  {
    for(int m=0; m<8; m++){
      num_choice_1[m] = 0;
      num_choice_2[m] = 0;
    }
    
    sum_1 = sum_2 = 0;
    if (lives_1 == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Prvi igrac gubi");
    }else if (lives_2 == 0){
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Drugi igrac gubi");
    }
  }
  
  // Funkcija koja čisti ekran i menja podatke samo ako je neki taster pritisnut
  if (key != last_key) {    
    // key has changed
    lcd.clear();
    lcd.setCursor(7,0);
    lcd.print("I");
    lives(1,lives_1);
    lives(2,lives_2);
    num_of_dice(dice);
    lcd.setCursor(0,0);

    // U zavisnosti od tastera koji je pritisnut se vrši drugačija radnja
    switch (key) {
      case KEY_UP:    // Menjanje broja bacanja
        if (select==0 && dice < 8) dice++;
      break;

      case KEY_DOWN:  // Menjanje broja bacanja
        if (select==0 && dice > 1) dice--;
      break;
      case KEY_SELECT:  // Prolazak kroz faze igre
        if(select==0) select++;
        else{
           if(br==dice-1 && (select>=0 && select<3)){
              select++;
              br=0;
           }
        }
      break;

      case KEY_RIGHT:
        select=num=0;
        lives_1=lives_2=3;
        dice=1;
        br=0;
      break;

      case KEY_LEFT:
       if(br<dice-1) br++;
      break;
    }
  }
  delajPrekoInterrupta(20);       //delay(20); // Opciono, ali korisno za debouncovanje tastera.
} 
