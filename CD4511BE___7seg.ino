
int D1 = 8; //7 pin
int D2 = 9; //1 pin
int D3 = 10; //2
int D4 = 11; //6
void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);

}

void loop() {
  for(int i=0;i<=9;i++){
     int temp=i;
     if(temp%2==0){
      digitalWrite(D4, LOW);
      Serial.print("0");
     }
     else{
     digitalWrite(D4, HIGH);
     Serial.print("1");
     }
     temp = temp / 2;
     if(temp%2==0){
      digitalWrite(D3, LOW);
      Serial.print("0");
     }
     else{
     digitalWrite(D3, HIGH);
     Serial.print("1");
     }
     temp = temp / 2;
     if(temp%2==0){
      digitalWrite(D2, LOW);
      Serial.print("0");
     }
     else{
     digitalWrite(D2, HIGH);
     Serial.print("1");
     }
     temp = temp / 2;
     if(temp%2==0){
      digitalWrite(D1, LOW);
      Serial.print("0");
     }
     else{
     digitalWrite(D1, HIGH);
     Serial.print("1");
     }
     temp = temp / 2;
     Serial.println();
     delay(500);
}
}
