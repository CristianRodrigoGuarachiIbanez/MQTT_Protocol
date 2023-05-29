#include "unittest_sub/unittest_sub.h"


#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "MQTT/Collision"
#define FILE        "/home/cristian/c++Project/mqtt/mqtt_paho/test/unittest/unittest_sub/unittest_1.txt"



int main(){


    subscribe::UnittestSub sub(FILE, TOPIC, ADDRESS, CLIENTID);
    //sub.runMultiThread(sub);
      
    return 0;
}