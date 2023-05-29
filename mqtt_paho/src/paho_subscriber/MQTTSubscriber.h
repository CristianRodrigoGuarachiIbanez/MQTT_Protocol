//#include "publisher/mqtt_pub.h"
//#include "subscriber/mqtt_sub.h"
#include "MQTTClient.h"
#include <iostream>


namespace mqtt
{
    //using MqttPublisher = mqtt_pub::publisher;
    //using MqttSubscriber = mqtt_sub::subscriber;
    

    class MqttSubscriber{
        public:
            explicit MqttSubscriber(const char*ad, const char*id);
            MqttSubscriber();
            void MqttConnect(const char * TOPIC, int QOS);
            ~MqttSubscriber(){
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
        private:

            MQTTClient client_;
            MQTTClient_connectOptions conn_opts_;
            //volatile MQTTClient_deliveryToken deliveredtoken_;
            int rc_, ch_;
            const char*CLIENTID_;
            void MqttInit(const char*ad, const char*id);
            static void delivered(void *context, MQTTClient_deliveryToken dt);
            static int msgArrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
            static void connLost(void *context, char *cause);
        


};
} // namespace name

