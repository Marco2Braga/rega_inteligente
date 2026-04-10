//Código base provindo de outo trabalho realizado por alguns alunos do grupo.
//Será utilizado apenas como base inicial para esta disciplina.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int relePin = 3;        // Relé conectado no pino 3
const int pinoSensor = A0;    // Sensor no pino A0

int nivelSolo;
int umidadePercentual;
int contadorAtivacoes = 0;
bool bombaLigada = false;

// Endereço padrão do LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, HIGH);   // Desliga o relé no início (nível lógico invertido)
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.print("Rega Inteligente");
  delay(3000);
  lcd.clear();
}

void loop() {
  nivelSolo = analogRead(pinoSensor);

  // Converter para porcentagem (inverter lógica: seco = 0%, molhado = 100%)
  umidadePercentual = map(nivelSolo, 1023, 300, 0, 100);
  umidadePercentual = constrain(umidadePercentual, 0, 100);

  // Debug no Serial Monitor
  Serial.print("Bruto: ");
  Serial.print(nivelSolo);
  Serial.print(" | Umidade: ");
  Serial.print(umidadePercentual);
  Serial.println("%");

  // Mostrar no LCD
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(umidadePercentual);
  lcd.print("%   ");  // limpa excesso na tela

  lcd.setCursor(0, 1);

  if (umidadePercentual < 20) {  // Solo seco
    lcd.print("Nivel: Seco   ");
    if (!bombaLigada) {
      bombaLigada = true;
      contadorAtivacoes++;
      Serial.println("Bomba LIGADA");
    }
    digitalWrite(relePin, LOW);   // Liga a bomba
  } else {
    if (bombaLigada) {
      Serial.println("Bomba DESLIGADA");
    }
    bombaLigada = false;
    digitalWrite(relePin, HIGH);  // Desliga a bomba

    if (umidadePercentual < 70)
      lcd.print("Nivel: Umido  ");
    else
      lcd.print("Nivel: Molhado");
  }

  delay(2000);
}
