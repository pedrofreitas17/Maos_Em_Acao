#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <MFRC522.h>

// --- PINOS E CONFIGURAÇÕES ---
#define SS_PIN 5   // SDA do RFID
#define RST_PIN 4  // RST do RFID
#define OLED_RESET -1

// Objetos
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
RoboEyes eyes(display);
SoftwareSerial somSerial(16, 17); // RX, TX para o DFPlayer
DFRobotDFPlayerMini leitorMP3;
MFRC522 rfid(SS_PIN, RST_PIN);

// --- 1. FUNÇÕES DOS OLHOS ---
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

// --- 2. FUNÇÕES DE SOM ---
void setupSom() {
  somSerial.begin(9600);
  if (!leitorMP3.begin(somSerial)) {
    Serial.println(F("Erro: DFPlayer não encontrado."));
  } else {
    leitorMP3.volume(20); 
    Serial.println(F("Som Online"));
  }
}

void tocarPasso(int numero) {
  leitorMP3.playFolder(1, numero); // Pasta 01, Ficheiro 00x.mp3
}

// --- 3. CONFIGURAÇÃO INICIAL (SETUP) ---
void setup() {
  Serial.begin(115200);
  SPI.begin();      // Inicia barramento SPI para RFID
  rfid.PCD_Init();  // Inicia o leitor RFID
  
  setupOlhos();     // Inicia o ecrã
  setupSom();       // Inicia o som
  
  Serial.println(F("--- PROJETO 1: SISTEMA PRONTO ---"));
}

// --- 4. CICLO PRINCIPAL (MAIN) ---
void loop() {
  eyes.update(); // Mantém os olhos ativos (ANIMAÇÃO)

  // Verifica se há um cartão novo
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Lógica do teu Scanner para ler o ID
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    uidString.concat(String(rfid.uid.uidByte[i], HEX));
  }
  uidString.toUpperCase();
  uidString.trim();

  Serial.print("Lido: ");
  Serial.println(uidString);

  // --- COMPARAÇÃO COM OS IDS DO TEU CADERNO ---
  
  if (uidString == "ID_DO_PASSO_1") { // Substitui pelo ID do caderno
    mudarExpressao(1); 
    tocarPasso(1);
  } 
  else if (uidString == "ID_DO_PASSO_2") { // Substitui
    mudarExpressao(1);
    tocarPasso(2);
  }
  else if (uidString == "ID_DO_PASSO_3") { // Substitui
    mudarExpressao(1);
    tocarPasso(3);
  }
  else if (uidString == "ID_DO_PASSO_4") { // Substitui
    mudarExpressao(1);
    tocarPasso(4);
  }
  else {
    // Se o cartão for desconhecido
    mudarExpressao(3); 
    tocarPasso(5); // Som de erro
  }

  // Finaliza leitura atual
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}