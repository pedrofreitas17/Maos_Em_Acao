void setupOlhos() {
  // Inicializa o hardware do ecrã
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Erro: OLED não encontrado!"));
    return;
  }
  
  // Inicializa o motor de animação (Largura, Altura, Velocidade)
  eyes.begin(128, 64, 100); 
  
  
  eyes.setAutoblinker(true); 
  eyes.setIdleMode(true);
}

void mudarExpressao(int estado) {
  /*
    Para que voces entenderem qdo chamamos a função basta mudar 
    o estado para o numero correspondente:
     0 = Normal (Padrão)
     1 = Feliz (Sucesso no passo)
     2 = Cansado (A executar a lavagem)
     3 = Zangado (Erro na sequência ou cartão errado)
  */
  switch (estado) {
    case 1:
      eyes.setMood(HAPPY);
      break;
    case 2:
      eyes.setMood(TIRED);
      break;
    case 3:
      eyes.setMood(ANGRY);
      break;
    default:
      eyes.setMood(DEFAULT);
      break;
  }
}