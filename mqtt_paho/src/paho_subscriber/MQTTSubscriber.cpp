#include "MQTTSubscriber.h"


//g++ -std=c++17 main.cpp mqtt.cpp -o main -lpaho-mqtt3cs

namespace mqtt{

    MqttSubscriber::MqttSubscriber(const char*ad, const char*id) : CLIENTID_(id){
        MqttInit(ad, CLIENTID_);
    }

    void MqttSubscriber::MqttInit(const char*ad, const char*id){
        conn_opts_ = MQTTClient_connectOptions_initializer;
        MQTTClient_create(&client_, ad, id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
        conn_opts_.keepAliveInterval = 20;
        conn_opts_.cleansession = 1;
    }

    void MqttSubscriber::delivered(void *context, MQTTClient_deliveryToken dt){
        std::cout<<"Message with token value" <<  dt << "delivery confirmed\n" << std::endl;
        volatile MQTTClient_deliveryToken deliveredtoken;
        deliveredtoken = dt;
    }

    int MqttSubscriber::msgArrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message){
        int i;
        char* payloadptr;

        std::cout <<"Message arrived\n"<<std::endl;
        std::cout <<"Topic: " << topicName << std::endl;
        std::cout <<"Message: " << std::endl;
        
        payloadptr = (char*)message->payload;
        for(i=0; i<message->payloadlen; i++){
            putchar(*payloadptr++);
        }
        putchar('\n');
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
    }

    void MqttSubscriber::connLost(void *context, char *cause){
        std::cout << "\nConnection lost\n" << std::endl;
        std::cout << "     cause: %s\n" << cause << std::endl;
    }

    void MqttSubscriber::MqttConnect(const char * TOPIC, int QOS){
        MQTTClient_setCallbacks(client_, NULL, connLost, msgArrvd, delivered);
       
        rc_ = MQTTClient_connect(client_, &conn_opts_);
        if (rc_ != MQTTCLIENT_SUCCESS)
        {
            std::cout <<"Failed to connect!  returned code: " << rc_ <<std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout <<"Subscribing to topic: " <<  TOPIC << std::endl;
        std::cout << "for client: " << CLIENTID_ << std::endl;
        std::cout << "using QoS: " << QOS << std::endl;
        std::cout << "Press Q<Enter> to quit" << std::endl;
        MQTTClient_subscribe(client_, TOPIC, QOS);
        do{
            ch_ = getchar();
        }while(ch_!='Q' && ch_ != 'q');
        
    }
}