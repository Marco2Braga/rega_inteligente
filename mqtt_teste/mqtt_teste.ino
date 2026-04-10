
#include<WiFi.h>
#include<PubSubClient.h>
#include "secrets.h"

//Rede

const char* ssid = WIFI_SSID;
const char* pass = WIFI_PASSWORD;

//Configs do broker mqtt

const char* mqtt_server = "192.168.1.179";
const int port = 1883;

//Inicializar o wifi no esp32 e posicionar ele como cliente do mqtt.

WiFiClient espClient;
PubSubClient client(espClient);

//wifi:
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


//setup e loop.

void setup() {
    Serial.begin(115200); 
    wifi_con();
    client.setServer(mqtt_server, port);

}

long contador = 0;

void loop() {
    if(!client.connected())
        {broker_con();}
    
    client.loop(); 
    static unsigned long lastmsg = 0;
    if (millis() - lastmsg > 5000) {
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