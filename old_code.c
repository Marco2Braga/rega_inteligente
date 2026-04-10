#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int relePin = 3;
// Agora o relé está conectado no pino 3
const int pinoSensor = A0; // Sensor continua no A0
int nivelSolo;
int contadorAtivacoes = 0;
bool bombaLigada = false;
// Endereço padrão do módulo I2C: 0x27 ou 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
pinMode(relePin, OUTPUT);
digitalWrite(relePin, HIGH); // Começa desligado
Serial.begin(9600);
lcd.init();
lcd.backlight();
lcd.print("Rega Inteligente");
delay(3000);
lcd.clear();
}
void loop() {
nivelSolo = analogRead(pinoSensor);
lcd.setCursor(0, 0);
lcd.print("Umidade: ");
lcd.print(nivelSolo);
lcd.print(" "); // Limpa resíduos na tela
lcd.setCursor(0, 1);
if (nivelSolo < 200) { // Limiar de solo seco
lcd.print("Nivel: Seco ");
if (!bombaLigada) {
bombaLigada = true;
contadorAtivacoes++;
Serial.println("Bomba LIGADA");
}
digitalWrite(relePin, LOW); // Liga a válvula solenóide
} else {if (bombaLigada) {
Serial.println("Bomba DESLIGADA");
}
bombaLigada = false;
digitalWrite(relePin, HIGH); // Desliga a válvula
if (nivelSolo < 700)
lcd.print("Nivel: Umido ");
else
lcd.print("Nivel: Molhado");
}
delay(2000);
}
—-------------------------------------------------------------
#include <LiquidCrystal.h>
const int relePin = 8;
const int pinoSensor = A0;
int nivelSolo;
int contadorAtivacoes = 0;
bool bombaLigada = false;
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);
void setup() {
pinMode(relePin, OUTPUT);
digitalWrite(relePin, LOW);
Serial.begin(9600);
lcd_1.begin(16, 2);
lcd_1.print("Rega Inteligente");
delay(3000);
lcd_1.clear();}
void loop() {
nivelSolo = analogRead(pinoSensor);
lcd_1.setCursor(0, 0);
lcd_1.print("Umidade: ");
lcd_1.print(nivelSolo);
lcd_1.setCursor(0, 1);
if (nivelSolo < 200) {
lcd_1.print("Nivel: Seco ");
if (!bombaLigada) {
bombaLigada = true;
contadorAtivacoes++;
}
digitalWrite(relePin, HIGH);
} else {
bombaLigada = false;
digitalWrite(relePin, LOW);
if (nivelSolo < 700)
lcd_1.print("Nivel: Umido ");
else
lcd_1.print("Nivel: Molhado");
}
// Envio de dados pela Serial
Serial.print("Umidade:");
Serial.print(nivelSolo);
Serial.print(",Bomba:");
Serial.print(bombaLigada ? "ON" : "OFF");
Serial.print(",Ativacoes:");
Serial.println(contadorAtivacoes);
delay(2000);
}
—---------------------------------------------------------------------------------------
// Inclui a biblioteca para o LCD
#include <LiquidCrystal.h>// Define os pinos do LCD conectados ao Arduino
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, EN, D4, D5, D6, D7
// Define o pino do sensor de umidade do solo
const int sensorUmidadePin = A0;
// Define os pinos do sensor ultrassônico
const int trigPin = 10;
const int echoPin = 9;
// Define o pino de controle da válvula solenoide (via transistor)
const int valvulaPin = 8;
// Define os limites de umidade para a rega
const int umidadeSeca = 300; // Ajuste este valor de acordo com as leituras do seu sensor
const int umidadeIdeal = 600; // Ajuste este valor de acordo com as leituras do seu sensor
// Define o intervalo de leitura do sensor (em milissegundos)
const long intervaloLeitura = 5000; // Lê a cada 5 segundos
unsigned long tempoAnteriorLeitura = 0;
// Variável para armazenar a leitura da umidade do solo
int leituraUmidade;
// Variáveis para o sensor ultrassônico
long duracao;
int distanciaNivelAgua;
// Define o nível mínimo de água (em cm) para permitir a rega
const int nivelMinimoAgua = 10; // Ajuste este valor conforme seu reservatório
void setup() {
// Inicializa o LCD com 16 colunas e 2 linhas
lcd.begin(16, 2);
lcd.print("Rega Inteligente");
lcd.setCursor(0, 1);
lcd.print("Inicializando...");
// Define o pino da válvula como saída
pinMode(valvulaPin, OUTPUT);
digitalWrite(valvulaPin, LOW); // Garante que a válvula comece desligada
// Define os pinos do sensor ultrassônico como saída (Trig) e entrada (Echo)pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
// Inicializa a comunicação serial (para depuração, se necessário)
Serial.begin(9600);
Serial.println("Sistema de Rega Inteligente Iniciado");
// Pequena pausa para inicialização
delay(2000);
lcd.clear();
}
void loop() {
// Verifica se já passou o intervalo para realizar uma nova leitura
unsigned long tempoAtual = millis();
if (tempoAtual - tempoAnteriorLeitura >= intervaloLeitura) {
tempoAnteriorLeitura = tempoAtual;
leituraUmidade = analogRead(sensorUmidadePin);
Serial.print("Umidade Bruta: ");
Serial.println(leituraUmidade);
// Mapeia a leitura analógica para uma escala de umidade (opcional, ajuste conforme
necessário)
int porcentagemUmidade = map(leituraUmidade, 0, 1023, 100, 0);
porcentagemUmidade = constrain(porcentagemUmidade, 0, 100); // Garante que esteja entre
0 e 100
Serial.print("Umidade (%): ");
Serial.println(porcentagemUmidade);
// Mede o nível da água
distanciaNivelAgua = medirNivelAgua();
Serial.print("Nivel da Agua (cm): ");
Serial.println(distanciaNivelAgua);
// Exibe as informações no LCD
lcd.setCursor(0, 0);
lcd.print("Umidade: ");
lcd.print(porcentagemUmidade);
lcd.print("% ");
lcd.setCursor(0, 1);
lcd.print("Agua: ");
if (distanciaNivelAgua > 0) {
lcd.print(distanciaNivelAgua);lcd.print("cm ");
} else {
lcd.print("?"); // Indica que não foi possível ler o nível
}
// Lógica de controle da rega
if (leituraUmidade < umidadeSeca && distanciaNivelAgua >= nivelMinimoAgua) {
Serial.println("Iniciando a rega...");
lcd.setCursor(10, 0);
lcd.print("REGANDO");
digitalWrite(valvulaPin, HIGH); // Liga a válvula
delay(5000); // Tempo de rega (ajuste conforme necessário)
digitalWrite(valvulaPin, LOW); // Desliga a válvula
Serial.println("Rega finalizada.");
lcd.setCursor(10, 0);
lcd.print("
"); // Limpa a mensagem de regando
} else if (distanciaNivelAgua < nivelMinimoAgua) {
Serial.println("Nível de água baixo. Rega não iniciada.");
lcd.setCursor(10, 0);
lcd.print("SEM AGUA");
} else {
Serial.println("Umidade ideal. Rega não necessária.");
lcd.setCursor(10, 0);
lcd.print("OK
");
}
}
delay(100); // Pequeno delay para não sobrecarregar o processador
}
// Função para medir a distância com o sensor ultrassônico (retorna em cm)
int medirNivelAgua() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duracao = pulseIn(echoPin, HIGH);
// A velocidade do som é aproximadamente 0.0343 cm/microssegundo
// Dividimos por 2 porque o som vai e volta
return duracao * 0.0343 / 2;
}