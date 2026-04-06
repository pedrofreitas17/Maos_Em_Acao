#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <MFRC522.h>

// --- PINOS E CONFIGURAÇÕES ---
#define SS_PIN 5   
#define RST_PIN 4  
#define OLED_RESET -1

// Pinos para o Som (Hardware Serial 2 do ESP32)
#define RXD2 16
#define TXD2 17

// --- OBJETOS ---
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
RoboEyes eyes(display);
DFRobotDFPlayerMini leitorMP3;
MFRC522 rfid(SS_PIN, RST_PIN);

// 1. FUNÇÕES DOS OLHOS
void setupOlhos() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Erro: OLED não encontrado!"));
    return;
  }
  eyes.begin(128, 64, 100); 
  eyes.setAutoblinker(true); 
  eyes.setIdleMode(true);
}

void mudarExpressao(int estado) {
  switch (estado) {
    case 1: eyes.setMood(HAPPY); break;
    case 2: eyes.setMood(TIRED); break;
    case 3: eyes.setMood(ANGRY); break;
    default: eyes.setMood(DEFAULT); break;
  }
}

// 2. FUNÇÕES DE SOM (Atualizado para Serial2)
void setupSom() {
  // Inicializa a porta serial 2 do ESP32 a 9600 baud
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); 
  
  if (!leitorMP3.begin(Serial2)) {
    Serial.println(F("Erro: DFPlayer não encontrado. Verifique fios e 5V!"));
  } else {
    leitorMP3.volume(25); // Volume (0 a 30)
    Serial.println(F("Som Online"));
  }
}

void tocarPasso(int numero) {
  // Toca o ficheiro 00x.mp3 dentro da pasta "01"
  leitorMP3.playFolder(1, numero); 
}

// 3. CONFIGURAÇÃO INICIAL (SETUP)
void setup() {
  Serial.begin(115200);
  SPI.begin();      
  rfid.PCD_Init();  
  
  setupOlhos();     
  setupSom();       
  
  Serial.println(F("--- PROJETO 1: SISTEMA PRONTO ---"));
}

// 4. CICLO PRINCIPAL (MAIN)
void loop() {
  eyes.update(); 

  // Verifica se há um cartão novo
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Lê o ID do cartão
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uidString.concat(String(rfid.uid.uidByte[i], HEX));
  }
  uidString.toUpperCase();
  uidString.trim();

  Serial.print("ID Lido: ");
  Serial.println(uidString);

 
  if (uidString == "8C C8 D7 D4") { 
    mudarExpressao(1); 
    tocarPasso(1);
  } 
  else if (uidString == "AC C2 D7 D4") { 
    mudarExpressao(1);
    tocarPasso(2);
  }
  else if (uidString == "1C B2 D6 D4") { 
    mudarExpressao(1);
    tocarPasso(3);
  }
  else if (uidString == "FC E9 D7 D4") { 
    mudarExpressao(1);
    tocarPasso(4);
  }
  else {
    // Cartão desconhecido
    mudarExpressao(3); 
    tocarPasso(5); // Som de alerta/erro
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}