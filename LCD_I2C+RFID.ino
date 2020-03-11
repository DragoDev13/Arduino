#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
typedef struct korsnik_st{
  char ime[15+1];
  char prezime[15+1];
  char pristupi[15+1];
  byte uid[10];
}Korisnik;
//-----------------------------------------------------
MFRC522 mfrc522(10, 9);
MFRC522::MIFARE_Key key;
//-----------------------------------------------------
int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  //blockAddr is the number of the block from 0 to 15.
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }
  byte buffersize = 18;
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(status));
          return 4;//return "4" as error message
  }
  Serial.println("block was read");
}

int writeBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}
  Serial.print(blockNumber);
  Serial.println(" is a data block:");
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }
  status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);//valueBlockA is the block number, MIFARE_Write(block number (0-15), byte array containing 16 values, number of bytes in block (=16))
  if (status != MFRC522::STATUS_OK) {
           Serial.print("MIFARE_Write() failed: ");
           Serial.println(mfrc522.GetStatusCodeName(status));
           return 4;//return "4" as error message
  }
  Serial.println("block was written");
}

LiquidCrystal_I2C lcd(0x3F, 16, 4);

void ocistiDisplej(){
  lcd.setCursor(0,0);
  for(int i=0;i<20;i++)lcd.print(" ");
  lcd.setCursor(0,1);
  for(int i=0;i<20;i++)lcd.print(" ");
  lcd.setCursor(0,2);
  for(int i=0;i<20;i++)lcd.print(" ");
  lcd.setCursor(0,3);
  for(int i=0;i<20;i++)lcd.print(" ");    
}
//koristi promeniljiva odradjeno i funkcija pisanjePodatakaPICC se koriste za pisanje podatak na PICC
int odradjeno=0;
void pisanjePodatakaPICC(){
  char vrednost[16]= "";
  strcpy(vrednost, "nekoime");
  writeBlock(4, vrednost);
  strcpy(vrednost, "nekoprezime");
  writeBlock(5, vrednost);
  strcpy(vrednost, "nekibrojpristupa");
  writeBlock(6, vrednost); 
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); 
  lcd.begin();
  lcd.backlight();
  for (byte i = 0; i < 6; i++) {
       key.keyByte[i] = 0xFF;
  }
  lcd.setCursor(0,0);
  lcd.print("Ceka se korisink... ");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  ocistiDisplej();
  lcd.setCursor(0,0);
  lcd.print("Ceka se korisnik...");
  if (!mfrc522.PICC_ReadCardSerial()){
    return;
  }
  ocistiDisplej();
  Korisnik temp;
  readBlock(4,temp.ime);
  readBlock(5,temp.prezime);
  readBlock(6,temp.pristupi);
  for(int i=0;i<mfrc522.uid.size;i++)temp.uid[i]=mfrc522.uid.uidByte[i];
  lcd.print(temp.ime);
  lcd.setCursor(0,1);
  lcd.print(temp.prezime);
  lcd.setCursor(0,2);
  int pristupi = atoi(temp.pristupi);
  if(pristupi<150)pristupi++;
  lcd.print(pristupi);
  if(pristupi==150)lcd.print(" LIMIT");
  lcd.setCursor(0,3);
  for(int i=0;i<mfrc522.uid.size;i++){
    lcd.print(temp.uid[i], HEX);
    lcd.print(" ");
  }
  sprintf(temp.pristupi, "%d", pristupi);
  writeBlock(6, temp.pristupi);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
