/*
Passo a passso para instalação do mosquitto no servidor debian:

sudo apt-get install mosquitto
sudo apt-get install mosquitto-clients

verificar se está rodando com:
 systemctl status mosquitto

visualizar no servidor um tópico de assinatura que o client irá publicar:
    mosquitto_sub -h localhost -t "rega/rega_auto/casa" -v
    disclaimer: -h de host -t de topico -v verboso

Para utilizar a biblioteca PubSubClient é preciso instalar:
Via cli:
arduino-cli lib download PubSubClient
arduino-cli lib install PubSubClient

Sem ela o código vai dar erro de compilação.

//Verificações da porta serial:
// arduino-cli monitor -p /dev/ttyUSB0 --config baudrate=115200

*/

#include<WiFi.h>
#include<PubSubClient.h>
#include "secrets.h"

//Rede

const char* ssid = WIFI_SSID;
const char* pass = WIFI_PASSWORD;

//Configs do broker mqtt

const char* mqtt_server = "192.168.1.179"; // Lembrar de colocar um íp estático na rede para o servidor.
const int port = 1883; //Porta padrão que o mosquitto usa

//Inicializar o wifi no esp32 e posicionar ele como cliente do mqtt.

WiFiClient espClient;
PubSubClient client(espClient);

//Função para conectar ao wifi:
void wifi_con(){
    delay(10);
    Serial.println();
    Serial.print("Conectando em ");
    Serial.println(ssid);

    WiFi.begin(ssid,pass);

    while (WiFi.status() != WL_CONNECTED) { //Loop pra ficar tentando conectar
        delay(6000);
        Serial.print("Tentando conexão wifi");
    }

    Serial.println("");
    Serial.println("Wifi conectado");
    Serial.println("IP recebido na rede: ");
    Serial.println(WiFi.localIP());
}

void broker_con(){
    while (!client.connected()){
        if (client.connect("ESP32-Client")) //Muita atenção nesse nome, se já houver outra sessão aberta no broker com esse nome, dá erro.
            {Serial.println("conectado");}
        else{
            Serial.print("falhou, erro = ");
            Serial.print(client.state()); // Dá alguns erros estranhos, não sei se é a forma correta.
            Serial.print("\n");
            Serial.print("Nova tentativa em 5 seg\n");
            delay(5000);
        }
    }
}


//setup e loop.

void setup() {
    Serial.begin(115200); //Possibilita ver a resposta do esp32,esse número precisa ser o mesmo do bound-rate no comando do terminal.
    wifi_con();
    client.setServer(mqtt_server, port); //Apontar o servidor mqtt

}

long contador = 0;

void loop() {
    if(!client.connected())
        {broker_con();}
    
    client.loop(); //Executa o processo interno de leitura e envio de mensagens do mqtt.

    //Estrutura de teste só pra ver se funciona
    static unsigned long lastmsg = 0;
    if (millis() - lastmsg > 5000) { //milisegundo.
        lastmsg = millis();
        contador++;

    //Mensagem de teste
    char m[] = "Mensagem de teste, it works!";
    printf("%s",m);

    //Teste de publicação

    Serial.print("Publicando mensagem: ");
    Serial.println(m);
    client.publish("rega/rega_auto/casa",m);
    }
}