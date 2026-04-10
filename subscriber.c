#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mosquitto.h"
#include<time.h>

/* O objetivo deste script é rodar no mesmo servidor que estará rodando o broker
dessa forma, quando o módulo publicar no broker, o script estará inscrito e irá capturar a mensagem publicada,
salvará a mensagem num arquivo csv junto com a data em que foi coletada.
Resumi algumas funções da biblioteca do mosquitto para facilitar os estudos, estão no rquivo mosquitto_study.
*/

// IMPORTANTE: Lembre-se de ao compilar adicionar a flag -lmosquitto para incluir a biblioteca (obviamente você precisa tê-la instalada também)
// Exemplo de linha de código para compilação:
//      gcc subscriber.c -o subcriber -lmosquitto

//Configs

const char *MQTT_BROKER = "localhost";//se for testar fora do servidor precisa por o ip do broker aqui!
const int MQTT_PORT = 1883;
const char *MQTT_TOPIC = "rega/rega_auto/casa";

//Declaração das funções (tentei sem mas dá muito erro de compilação)

void on_connect(struct mosquitto *mosq, void *userdata, int result);
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void save_csv(const char *payload_str);

//Aqui vão existir duas funções com "on_" na frente, elas vão lidar com os resultados dos callbacks
//e trabalhar as respostas.

void on_connect(struct mosquitto *mosq, void *userdata, int result) {
    if (result == 0) {
        printf("Conectado ao Broker com sucesso!\n");
        mosquitto_subscribe(mosq,NULL, MQTT_TOPIC,0); //para entender esse 0 procure por Quality of Service na documentação, será 0,1 ou 2.
    } else {
        fprintf(stderr,"Falha ao conectar ao broker.\n"); //Se tirar o stderr fica impossível achar o erro.

    }
}

void on_message(struct mosquitto *mosq, void *userdata,const struct mosquitto_message *message){
    //O conteúdo da mensagem dá muito problema aqui, depois de muita pesquisa descobri
    //que ele pode não terminar com o \0 padrão do c, precisa construir um buffer pra lidar com isso
    //A documentação recomenda o uso de mosquitto_message_copy() para salvar o conteúdo da mensagem publicada antes de lidar com ela
    //E só então fazer as modificações necessárias.
    //Mas como não é um ambiente de prod externo na web, não utilizei.
    //Obs: o " -> " é a forma rápida de acessar o conteúdo de uma struct guardada via ponteiro.
    char buffer[message -> payloadlen + 1]; //garantindo o tamanho extra para o \0
    memcpy(buffer, message -> payload, message -> payloadlen); //cópia em memória
    buffer[message -> payloadlen] = '\0';

    save_csv(buffer); // função que vai de fato salvar a mensagem depois do tratamento.

}

void save_csv(const char *payload_str) {
  FILE *fp;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t); //Vai pegar o horário atual da publicação da mensagem para salvar.
  
  fp = fopen("infos.csv","a");
  if (fp == NULL) {
    printf("Erro ao abrir arquivo csv\n");
    return;}
  //Formatação da data:
    //ano-mes-dia hh:mn:seg:payload
    //Como ainda não sabemos a quantidade de informação que virá no payload, precisaremos incrementar essa função para tratar o conteúdo e encaixar no csv.
  fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d,%s\n",
  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, payload_str);
  fclose(fp);
  printf("Dados salvo com sucesso!\n");
    
}

int main(int argc, char *argv[]) { // vou manter argc e argv aqui porque pretendo dar opções de linha de comando para esse script, como apontar um broker na web, ou mudar o topico.
    struct mosquitto *mosq = NULL; //Essa struct é necessária para receber o novo cliente mosquitto que irá trabalhar as infos e callbacks.
    int con;

    //Inicializar a biblioteca.
    mosquitto_lib_init();

    //Criar um novo cliente mosquitto
    mosq = mosquitto_new(NULL,true,NULL); // Parâmetros: Client_id,clean_session, userdata.
    if (!mosq) {
      fprintf(stderr, "Erro: Não foi possível criar o cliente Mosquitto.\n");
      return 1;
      }
    
    //Chamar as funções de callbacks dos eventos:
    mosquitto_connect_callback_set(mosq,on_connect);
    mosquitto_message_callback_set(mosq,on_message);

    //Conectar no broker (que dor de cabeça isso aqui)
    con = mosquitto_connect(mosq, MQTT_BROKER,MQTT_PORT,60); //60 é o keepalive, existe uma relão com a função loop_forever mais abaixo.
    if (con != MOSQ_ERR_SUCCESS) {
      fprintf(stderr,"Erro: Não foi possível conectar ao broker: %s\n", mosquitto_strerror(con)); // sem essa linha foi muito difícil debugar erros de conexão.
      return 1;
    }
    printf("Conectando ao broker...\n");

    //Loop de rede pra ficar coletando publicações direto.
    //essa função de loop do mosquitto mantém o script funcionando direto, ela toma conta de basicamente todo o restante que teríamos que fazer
    //incluindo os callbacks.
    mosquitto_loop_forever(mosq,-1,1); //Parâmetros: Cliente, timeout, max_packets
    
    //Em tese o programa nunca chega nas linhas a seguir, mas é boa prática em C liberar memória e etc.
    mosquitto_destroy(mosq); //Fecha o client (tem critérios de segurança caso seja um ambiente na internet)
    mosquitto_lib_cleanup(); // Vários processos de free()
    return 0;
  }
