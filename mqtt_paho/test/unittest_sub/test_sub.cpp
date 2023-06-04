#include <iostream>
#include <string>
#include <functional>
#include <utility>
#include <memory>
// #include "MQTTSubscriber.h"
// #include "unittest_sub.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L



// int main_sub(){
    
    
//     mqtt::MqttSubscriber client(ADDRESS, CLIENTID);
//     client.MqttConnect(TOPIC, CLIENTID, QOS);
//     std::cout << client.getRC() << std::endl;

//     //SubPrint<Manager>(Msg);
//     //std::cout << "hola" <<std::endl;
//     return 0;
// }