#include <SPI.h>
#include <MFRC522.h>

// Definição dos pinos não é para mexer por favorrr
#define SS_PIN  5   // Pino NSS/SDA 
#define RST_PIN 4   // Pino RST 

MFRC522 rfid(SS_PIN, RST_PIN); 

void setup() {
  Serial.begin(115200);
  SPI.begin();      // Isto inicia o barramento SPI não mexer
  rfid.PCD_Init();  // Isto inicia o módulo MFRC522 não mexer rapazes 
  
  Serial.println("--- SCANNER DE CARTÕES MÃOS EM AÇÃO ---");
  Serial.println("Aproxime um cartão do leitor para descobrir o ID...");
}

void loop() {
  // Isto verifica se há um novo cartão presente
  if (!rfid.PICC_IsNewCardPresent()) return;

  // Isto Seleciona o cartão
  if (!rfid.PICC_ReadCardSerial()) return;

  // serve para mostra o UID no Monitor Serial..
  Serial.print("ID detetado:");
  String uidString = "";
  
  for (byte i = 0; i < rfid.uid.size; i++) {
    // Formata o ID em Hexadecimal para ser fácil de ler blz 
    uidString.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uidString.concat(String(rfid.uid.uidByte[i], HEX));
  }
  
  uidString.toUpperCase();
  Serial.println(uidString);
  Serial.println("---------------------------------------");

  // Instrução para o leitor parar de ler o mesmo cartão repetidamente
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}