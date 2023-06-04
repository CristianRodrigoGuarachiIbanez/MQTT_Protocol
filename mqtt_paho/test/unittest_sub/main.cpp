#include "unittest_sub.h"
//#include "fileManager/fileReader.hpp"
#include <cstdlib>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ClientSub"
#define TOPIC       "MQTT/Collision"



int main(){

 /*    #ifdef __linux__
    std::system("gnome-terminal");
    std::system("ls -l");
    #endif */

    subscribe::UnittestSub sub(TOPIC, ADDRESS, CLIENTID);
    sub.receivePayload();
    return 0;
}