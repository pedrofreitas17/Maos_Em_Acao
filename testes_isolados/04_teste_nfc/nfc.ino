#include <SPI.h>
#include <MFRC522.h>
#include "DFRobotDFPlayerMini.h"

// --- Configurações do RFID (SPI) ---
#define RST_PIN   5     
#define SS_PIN    34    // Este é o NSS/SDA
MFRC522 mfrc522(SS_PIN, RST_PIN);

// --- Configurações do Som (Serial) ---
HardwareSerial somSerial(1); 
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  delay(2000); 

  // 1. Inicia o SPI para o RFID (Pinos: SCK, MISO, MOSI, SS)
  SPI.begin(32, 33, 35, 34); 
  mfrc522.PCD_Init();
  Serial.println(F("RFID pronto para ler!"));

  // 2. Inicia o Serial para o Som (Pinos: RX=18, TX=17)
  somSerial.begin(9600, SERIAL_8N1, 18, 17); 
  
  if (!myDFPlayer.begin(somSerial, true, false)) {
    Serial.println(F("Erro: DFPlayer nao encontrado."));
    // Nao paramos o código aqui para poderes testar o RFID mesmo sem som
  } else {
    Serial.println(F("DFPlayer OK!"));
    myDFPlayer.volume(15); // Volume seguro
  }

  Serial.println(F("Aguardando aproximacao do cartao..."));
}

void loop() {
  // Verifica se há um cartão novo
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Tenta ler o cartão
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Se chegou aqui, um cartão foi lido!
  Serial.print(F("Cartao detectado! UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // TOCA O ÁUDIO 1 (Abertura do Robô)
  Serial.println(F("Tocando audio de boas-vindas..."));
  myDFPlayer.play(0002); 

  // Pequena pausa para nao ler o mesmo cartao mil vezes seguidas
  myDFPlayer.play(0002); 
  delay(500); // Dá tempo ao módulo para processar
  
  // Isto vai imprimir o erro real no Monitor Serial
  if (myDFPlayer.readType() == DFPlayerError) {
    Serial.print("Erro do DFPlayer: ");
    Serial.println(myDFPlayer.read());
  }
  // Prepara o RFID para a próxima leitura
  mfrc522.PICC_HaltA();
}