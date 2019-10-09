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
    int temp;
    digitalWrite(katodaCifraTri, LOW);
    delay(1);
    digitalWrite(katodaCifraTri, HIGH);
    digitalWrite(katodaCifraDva, LOW);
    delay(1);
    digitalWrite(katodaCifraDva, HIGH);
    temp = broj;
    if(temp%2==0)digitalWrite(BD1, LOW);
    else digitalWrite(BD1, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD2, LOW);
    else digitalWrite(BD2, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD3, LOW);
    else digitalWrite(BD3, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD4, LOW);
    else digitalWrite(BD4, HIGH);
    digitalWrite(katodaCifraJedan, LOW);
    delay(1);
    digitalWrite(katodaCifraJedan, HIGH);
}
void brojManjiOd100(int broj){
    int temp;
    digitalWrite(katodaCifraTri, LOW);
    delay(1);
    digitalWrite(katodaCifraTri, HIGH);

    temp = broj/10;
    if(temp%2==0)digitalWrite(BD1, LOW);
    else digitalWrite(BD1, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD2, LOW);
    else digitalWrite(BD2, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD3, LOW);
    else digitalWrite(BD3, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD4, LOW);
    else digitalWrite(BD4, HIGH);
    digitalWrite(katodaCifraDva, LOW);
    delay(1);
    digitalWrite(katodaCifraDva, HIGH);

    temp = broj%10;
    if(temp%2==0)digitalWrite(BD1, LOW);
    else digitalWrite(BD1, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD2, LOW);
    else digitalWrite(BD2, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD3, LOW);
    else digitalWrite(BD3, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD4, LOW);
    else digitalWrite(BD4, HIGH);
    digitalWrite(katodaCifraJedan, LOW);
    delay(1);
    digitalWrite(katodaCifraJedan, HIGH);
}
void brojVeciOd99(int broj){
    int temp;
    temp = broj/100;
    if(temp%2==0)digitalWrite(BD1, LOW);
    else digitalWrite(BD1, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD2, LOW);
    else digitalWrite(BD2, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD3, LOW);
    else digitalWrite(BD3, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD4, LOW);
    else digitalWrite(BD4, HIGH);
    digitalWrite(katodaCifraTri, LOW);
    delay(1);
    digitalWrite(katodaCifraTri, HIGH);
    
    temp = (broj%100)/10;
    if(temp%2==0)digitalWrite(BD1, LOW);
    else digitalWrite(BD1, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD2, LOW);
    else digitalWrite(BD2, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD3, LOW);
    else digitalWrite(BD3, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD4, LOW);
    else digitalWrite(BD4, HIGH);
    digitalWrite(katodaCifraDva, LOW);
    delay(1);
    digitalWrite(katodaCifraDva, HIGH);
    
    temp = broj%10;
    if(temp%2==0)digitalWrite(BD1, LOW);
    else digitalWrite(BD1, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD2, LOW);
    else digitalWrite(BD2, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD3, LOW);
    else digitalWrite(BD3, HIGH);
    temp = temp /2;
    if(temp%2==0)digitalWrite(BD4, LOW);
    else digitalWrite(BD4, HIGH);
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
