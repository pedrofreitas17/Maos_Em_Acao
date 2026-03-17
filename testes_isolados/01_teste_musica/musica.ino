void setupSom() {
  somSerial.begin(9600);
  
  Serial.println(F("A iniciar DFPlayer..."));
  
  if (!leitorMP3.begin(somSerial)) {
    Serial.println(F("Erro: DFPlayer nao encontrado."));
    // Isto é para nao bloqueamos o codigo para os olhos poderem continuar a funcionar blz
  } else {
    leitorMP3.volume(20); //  ISTO! é aonde definimos o volume (0 a 30)
    Serial.println(F("DFPlayer Online"));
  }
}

void tocarPasso(int numero) {
  /*
     Isso é a função que toca o ficheiro correspondente na pasta do SD:
     numero 1 -> toca 001.mp3 (Cartão 1)
     numero 2 -> toca 002.mp3 (Cartão 2)
     numero 3 -> toca 003.mp3 (Cartão 3)
     numero 4 -> toca 004.mp3 (Cartão 4)
     numero 5 -> toca 005.mp3 (Som de Erro)
     não mudem os numeros se nao vai dar uma confusao 
  */
  
  leitorMP3.playFolder(1, numero);
  
  Serial.print(F("Comando de som enviado para o Passo:"));
  Serial.println(numero);
}