#ifndef MQTT_PUB_H_
#define MQTT_PUB_H

#include <string.h>
#include <iostream>
#include "MQTTClient.h"


namespace mqtt{
    static volatile MQTTClient_deliveryToken deliveredtoken_;

    class MqttPublisher{
        public: 
            explicit MqttPublisher(const char*ad, const char*id, int aliveIntervals);
            MqttPublisher();
            void MqttMessage(const char * TOPIC, const char*PAYLOAD, int QOS);
            ~MqttPublisher(){
                MQTTClient_disconnect(client_, 10000);
                MQTTClient_destroy(&client_);
            }
            int getRC(){
                if(rc_ !=0){
                    return rc_;
                }else{
                    return 0;
                }
                
            }
            ;
        private:
            MQTTClient client_;
            MQTTClient_connectOptions conn_opts_;
            MQTTClient_message pubmsg_;
            MQTTClient_deliveryToken token_;
            
            int rc_;
            const char * CLIENTID_, *ADDRESS_;
            void MqttInit(const char*ad, const char*id, int aliveIntervals);
            void MqttConnect();
            static void delivered(void *context, MQTTClient_deliveryToken dt);
            static int msgArrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
            static void connLost(void *context, char *cause);
        

    };


}


#endif /* MQTT_PUB_H*/