int BD1 = 13;
int BD2 = 12;
int BD3 = 11;
int BD4 = 10;
int katodaCifraJedan = 4;
int katodaCifraDva = 7;
int katodaCifraTri = 8;
/*
 katode: 11, 8, 7
 dole levo 1
 dole srednje 2
 tacka 3
 dole desno 4
 srednja 5
 gornja levo 9
 gornja desno 7 
 gornja srednja 10
 */
void setup() {
  pinMode(BD1, OUTPUT);
  pinMode(BD2, OUTPUT);
  pinMode(BD3, OUTPUT);
  pinMode(BD4, OUTPUT);
  digitalWrite(BD1, LOW);
  digitalWrite(BD2, LOW);
  digitalWrite(BD3, LOW);
  digitalWrite(BD4, LOW);
  pinMode(katodaCifraJedan, OUTPUT);
  pinMode(katodaCifraDva, OUTPUT);
  pinMode(katodaCifraTri, OUTPUT);
  digitalWrite(katodaCifraJedan, HIGH);
  digitalWrite(katodaCifraDva, HIGH);
  digitalWrite(katodaCifraTri, HIGH);
 
}
void decToBin(int broj){
  if(broj%2==1)digitalWrite(BD1, HIGH);
  else digitalWrite(BD1, LOW);
  broj = broj / 2;
  if(broj%2==1)digitalWrite(BD2, HIGH);
  else digitalWrite(BD2, LOW);
  broj = broj / 2;
  if(broj%2==1)digitalWrite(BD3, HIGH);
  else digitalWrite(BD3, LOW);
  broj = broj / 2;
  if(broj%2==1)digitalWrite(BD4, HIGH);
  else digitalWrite(BD4, LOW);
  broj = broj / 2;
}
void ispisNaDispleju(int broj){
    int temp;
    digitalWrite(BD1, LOW);
    digitalWrite(BD2, LOW);
    digitalWrite(BD3, LOW);
    digitalWrite(BD4, LOW);
    digitalWrite(katodaCifraJedan, HIGH);
    digitalWrite(katodaCifraDva, HIGH);
    digitalWrite(katodaCifraTri, HIGH);
    if(broj<10)brojManjiOd10(broj);
    else if(broj<100)brojManjiOd100(broj);
    else brojVeciOd99(broj);
}
void brojManjiOd10(int broj){
    digitalWrite(katodaCifraTri, LOW);
    delay(1);
    digitalWrite(katodaCifraTri, HIGH);
    digitalWrite(katodaCifraDva, LOW);
    delay(1);
    digitalWrite(katodaCifraDva, HIGH);
    decToBin(broj);
    digitalWrite(katodaCifraJedan, LOW);
    delay(1);
    digitalWrite(katodaCifraJedan, HIGH);
}
void brojManjiOd100(int broj){
    digitalWrite(katodaCifraTri, LOW);
    delay(1);
    digitalWrite(katodaCifraTri, HIGH);
    decToBin(broj/10);
    digitalWrite(katodaCifraDva, LOW);
    delay(1);
    digitalWrite(katodaCifraDva, HIGH);
    decToBin(broj%10);
    digitalWrite(katodaCifraJedan, LOW);
    delay(1);
    digitalWrite(katodaCifraJedan, HIGH);
}
void brojVeciOd99(int broj){
    decToBin(broj/100);
    digitalWrite(katodaCifraTri, LOW);
    delay(1);
    digitalWrite(katodaCifraTri, HIGH);
    decToBin((broj%100)/10);
    digitalWrite(katodaCifraDva, LOW);
    delay(1);
    digitalWrite(katodaCifraDva, HIGH);
    decToBin(broj%10);
    digitalWrite(katodaCifraJedan, LOW);
    delay(1);
    digitalWrite(katodaCifraJedan, HIGH);
}
    
void loop() {
  int i;
  for(i = 0;i<999;i++){
    ispisNaDispleju(i);
    delay(40);
    }
  for(i = 999;i>=0;i--){
    ispisNaDispleju(i);
    delay(40);
    }
}
