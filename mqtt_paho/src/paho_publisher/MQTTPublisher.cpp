#include "MQTTPublisher.h"


namespace mqtt{
    MqttPublisher::MqttPublisher(const char*ad, const char*id, int aliveIntervals) : ADDRESS_(ad) , CLIENTID_(id){
       
        MqttInit(ad, id, aliveIntervals);
        MqttConnect();
    }

    void MqttPublisher::MqttInit(const char*ADDRESS, const char*CLIENTID, int aliveIntervals) {
        conn_opts_ = MQTTClient_connectOptions_initializer;
        pubmsg_ = MQTTClient_message_initializer;
        MQTTClient_create(&client_, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
        conn_opts_.keepAliveInterval = aliveIntervals;
        conn_opts_.cleansession = 1;

    }

    void MqttPublisher::MqttConnect(){

        MQTTClient_setCallbacks(client_, NULL, connLost, msgArrvd, delivered);
        rc_ = MQTTClient_connect(client_, &conn_opts_);
        if (rc_ != MQTTCLIENT_SUCCESS)
        {
            std::cout << "Failed to connect, return code\n" << rc_<< std::endl;
            exit(EXIT_FAILURE);
        }
      
    }

    void MqttPublisher::MqttMessage(const char * TOPIC, const char*PAYLOAD, int QOS){

        pubmsg_.payload = (char*) PAYLOAD;
        pubmsg_.payloadlen = strlen(PAYLOAD);
        pubmsg_.qos = QOS;
        pubmsg_.retained = 0;
        deliveredtoken_ = 0;
        MQTTClient_publishMessage(client_, TOPIC, &pubmsg_, &token_);
        std::cout <<"\n Waiting for publication of " << PAYLOAD << "\n on topic: " << TOPIC << "\n for client with ClientID: " << CLIENTID_ << std::endl;
        while(deliveredtoken_ != token_);
    }

    void MqttPublisher::delivered(void *context, MQTTClient_deliveryToken dt){
        std::cout<<" Message with token value '" << dt << "' delivery confirmed\n" <<std::endl;
        deliveredtoken_ = dt;
    }

    int MqttPublisher::msgArrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message){
        int i;
        char* payloadptr;

        std::cout <<"Message arrived\n"<<std::endl;
        std::cout <<"     topic: " << topicName << std::endl;
        std::cout <<"   message: " << std::endl;
        payloadptr = (char*)message->payload;
        for(i=0; i<message->payloadlen; i++){
            putchar(*payloadptr++);
        }
        putchar('\n');
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    }

    void MqttPublisher::connLost(void *context, char *cause){
        std::cout << "\nConnection lost\n" << std::endl;
        std::cout << "     cause: %s\n" << cause << std::endl;
    }
}


