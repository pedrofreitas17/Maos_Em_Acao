#include <SPI.h>
#include <MFRC522.h>
#include "DFRobotDFPlayerMini.h"


#define RST_PIN   5     
#define SS_PIN    34    
MFRC522 mfrc522(SS_PIN, RST_PIN);


HardwareSerial somSerial(1); 
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  delay(2000); 

  SPI.begin(32, 33, 35, 34); 
  mfrc522.PCD_Init();
  Serial.println(F("RFID pronto para ler!"));

  somSerial.begin(9600, SERIAL_8N1, 18, 17); 
  
  if (!myDFPlayer.begin(somSerial, true, false)) {
    Serial.println(F("Erro: DFPlayer nao encontrado."));

  } else {
    Serial.println(F("DFPlayer OK!"));
    myDFPlayer.volume(15); 
  }

  Serial.println(F("Aguardando aproximacao do cartao..."));
}

void loop() {

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

 
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

 
  Serial.print(F("Cartao detectado! UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  
  Serial.println(F("Tocando audio de boas-vindas..."));
  myDFPlayer.play(0002); 

  
  myDFPlayer.play(0002); 
  delay(500); 
  
  if (myDFPlayer.readType() == DFPlayerError) {
    Serial.print("Erro do DFPlayer: ");
    Serial.println(myDFPlayer.read());
  }
  
  mfrc522.PICC_HaltA();
}