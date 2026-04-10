//Comandos de compile e upload

// arduino-cli compile --fqbn esp32:esp32:esp32 rega_auto/
// arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 rega_auto/


// Pinos //

const int pinorele = 27; // Saida P27 na minha esp32
const int pinosensor_umi = 34; // Saida P34 na minha esp32

//Configs de Lógica:
const int limite_umidade = 2000;
const int tempo_rega = 5000;
const long intervalo_veri = 6000;

int isSoloSeco() {
    int valor_umidade = analogRead(pinosensor_umi); // Checa a umidade no sensor
    if (valor_umidade > limite_umidade) //Se o solo estiver seco retorna 1
        {return 1;}
    else 
        {return 0;}

}

void rega(int solo_seco){
    if (solo_seco == 1) {
        digitalWrite(pinorele,0);
        delay(tempo_rega);
        digitalWrite(pinorele,1);
    }
}

//Lembrete: Setup e Loop são partes essenciais do arduino.
//Setup é o estado inicial
//Loop é a repetição da nossa lógica:

void setup() {
    
    pinMode(pinorele, OUTPUT); //Relé como saída digital

    digitalWrite(pinorele,1); //Começar com a bomba desligada.
}

void loop() {
    
    rega(isSoloSeco());

    delay(intervalo_veri); //Delay pra não checar o tempo todo.

}