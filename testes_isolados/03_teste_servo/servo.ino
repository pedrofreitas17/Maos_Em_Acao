#include <ESP32Servo.h>

// --- CONFIGURAÇÕES ---
const int pinoServoCerebro = 14; // GPIO onde o servo está ligado
Servo servoCerebro;

// Definição dos ângulos para 90 graus de amplitude
const int posicaoBaixo = 0;   // 90 graus abaixo do topo (ajustável)
const int posicaoAlto = 80;   // 90 graus acima da base (ajustável)
const int delayMovimento = 5; // Velocidade (menor = mais rápido)
const int posicaoAltogiggle = 90;

void setup()
{
    Serial.begin(115200);

    // Necessário para o ESP32-S2 gerir o PWM do servo
    ESP32PWM::allocateTimer(0);

    servoCerebro.setPeriodHertz(80);
    servoCerebro.attach(pinoServoCerebro, 500, 2400);

    // Inicia na posição central (90 graus)
    servoCerebro.write(40);
    delay(1000);
}

void gestoAlegria()
{
    // 1. Expressão visual de risada
    // A biblioteca RoboEyes tem uma função específica para isto

    // 2. Movimento de "Giggle" (tremidinho de braços/cérebro)
    // Fazemos um ciclo rápido de pequenos movimentos
    for (int i = 0; i < 8; i++)
    {
        servoCerebro.write(100); // Sobe um pouco
        delay(60);               // Movimento muito rápido
        servoCerebro.write(80);  // Desce um pouco
        delay(60);
    }

    // 3. Gesto final de celebração (Braços/Cérebro bem abertos)
    servoCerebro.write(180);
    delay(1000);

    // Volta ao estado normal

    servoCerebro.write(90);
}
void loop()
{
    // 1. Sobe 90 graus (de 45 para 135)
    Serial.println("A subir 90 graus...");
    for (int pos = posicaoBaixo; pos <= posicaoAlto; pos++)
    {
        servoCerebro.write(pos);
        delay(delayMovimento);
    }

    delay(1000); // Pausa no topo

    // 2. Desce 90 graus (de 135 para 45)
    Serial.println("A descer 90 graus...");
    for (int pos = posicaoAlto; pos >= posicaoBaixo; pos--)
    {
        servoCerebro.write(pos);
        delay(delayMovimento);
    }

    delay(1000); // Pausa na base
}