#include "unittest_pub.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ClientPub"
#define TOPIC       "MQTT/Collision"
#define FILE        "/home/cristian/c++Project/mqtt/mqtt_paho/test/in_out_files/unittest_1.txt"



int main(){
    publish::UnittestPub publisher(FILE, TOPIC, ADDRESS, CLIENTID);
    publisher.sendPayload();

    return 0;
}