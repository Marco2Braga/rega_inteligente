#include<WiFi.h>
#include<PubSubClient.h>
#include "secrets.h"
#include "driver/adc.h"

// Rede
const char* ssid = WIFI_SSID;
const char* pass = WIFI_PASSWORD;

//Configs do broker mqtt

const char* mqtt_server = "192.168.1.179"; 
const int port = 1883; 

WiFiClient espClient;
PubSubClient client(espClient);

// Pinos //

const int pinorele = 27; // Saida P27 na minha esp32

//GPIO 35 é o ADC1_CHANNEL_7
const adc1_channel_t PINO_SENSOR_ADC = ADC1_CHANNEL_7;

//Configs de Lógica:
const int limite_umidade = 3600;
const int tempo_rega = 5000;
const long intervalo_veri = 6000;
int valor_umidade;
int cont_bomba = 0;

//Funções

//Conexão do esp com wifi
void wifi_con(){
    delay(10);
    Serial.println();
    Serial.print("Conectando em ");
    Serial.println(ssid);

    WiFi.begin(ssid,pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(6000);
        Serial.print("Tentando conexão wifi");
    }

    Serial.println("");
    Serial.println("Wifi conectado");
    Serial.println("IP recebido na rede: ");
    Serial.println(WiFi.localIP());
}

//Conexão com o Broker
void broker_con(){
    while (!client.connected()){
        if (client.connect("ESP32-Client")) 
            {Serial.println("conectado");}
        else{
            Serial.print("falhou, erro = ");
            Serial.print(client.state());
            Serial.print("\n");
            Serial.print("Nova tentativa em 5 seg\n");
            delay(5000);
        }
    }
}

//Retorna o valor da umidade.
int ValUmid() {
    valor_umidade = adc1_get_raw(PINO_SENSOR_ADC);
    return valor_umidade;
}

//Checa se o solo está seco e faz contagem de ativações da bomba
int isSoloSeco(int valor_umidade) {
    if (valor_umidade > limite_umidade) //Se o solo estiver seco retorna 1
        {return 1;}
    else 
        {return 0;}

}


void rega(int solo_seco){
    if (solo_seco == 1) {
        digitalWrite(pinorele,LOW);
        delay(tempo_rega);
        digitalWrite(pinorele,HIGH);
        cont_bomba += 1;
    }
}

//Lembrete: Setup e Loop são partes essenciais do arduino.
//Setup é o estado inicial
//Loop é a repetição da nossa lógica:

void setup() {

    Serial.begin(115200);
    wifi_con();
    client.setServer(mqtt_server, port);


    //Define a "largura" da leitura (12 bits = 0 a 4095)
    adc1_config_width(ADC_WIDTH_BIT_12);
  
    //Define a "atenuação" (MUITO IMPORTANTE)
    //Isso define a faixa de voltagem que o pino pode ler.
    //ADC_ATTEN_DB_11 permite ler a faixa completa (0V a 3.3V)
    adc1_config_channel_atten(PINO_SENSOR_ADC, ADC_ATTEN_DB_11);
    
    pinMode(pinorele, OUTPUT); //Relé como saída digital

    digitalWrite(pinorele,HIGH); //Começar com a bomba desligada.
}

void loop() {
    
    rega(isSoloSeco(ValUmid())); // Core do programa

    delay(intervalo_veri); //Delay pra não checar o tempo todo.

    if(!client.connected())
        {broker_con();}
    
    client.loop() //loop do mosquito para conexão com cliente; 


    //formatando buffer da mensagem.
    char buff_texto[30];
    sprintf(buff_texto, "%d, %d", ValUmid(), cont_bomba);

    //Publicação
    Serial.print("Publicando mensagem: ");
    Serial.println(buff_texto);
    client.publish("rega/rega_auto/casa",buff_texto);
    }
