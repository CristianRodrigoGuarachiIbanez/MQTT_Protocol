
#include "mqtt_pub.h"
#include <iostream>

/* to test 
mosquitto -v
mosquitto_sub -t <topic>
g++ -std=c++17 main.cpp mqtt_pub.cpp -o publisher -lmosquitto
*/

int main(int argc, char*argv[]){
    char m[] = "Cristian Host: ";
    char *host;
    const char * topic, *ID;
    //char** arg = argv;
    std::cout << "Usage: ./publish <host> <ID> <topic> <message>" << std::endl;
    if(argc < 3){
   
        std::cerr<< "[ERROR]: usage: ./publish <host> <ID> <topic> <message>" << std::endl;
        exit(0);
        
    }else{
        host = argv[1];
        ID = argv[2];
        topic = argv[3]; 
    }

    mqtt_pub::publisher pub(ID, host, 1883, true); // "publisher-test"
    pub.set_message(m, argv, argc);
    pub.publish(topic, 0); // "test/round1"
    //free(pub.get_message());
    std::cout << pub.get_msg() << std::endl;
    return 0;
}