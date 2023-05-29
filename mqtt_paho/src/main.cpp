#include "paho_publisher/MQTTPublisher.h"
#include "paho_subscriber/MQTTSubscriber.h"
#include <cstdlib>
#include <string>
#include <iostream>

#define PUB  "pub"
#define SUB  "sub"
int main(int argc, char *argv[] ){

    if(argc < 4){
        std::cerr << "One or more arguments to run this app are missing \n <Module...> <API> <Port> <Topic> <'Message'>";
    }
    std::string mod = argv[1];
    int module = (mod==PUB) ? 0 : 1;
    std::string ID = argv[2];
    std::string port = argv[3];
    std::string topic = argv[4];
    std::string mssg = argv[5];
    if((module != 0) && (module != 1)){
       std::cerr << "Unexpected module name " << module <<" the expected module names are 'sub' -> for subscriber and 'pub' -> for publisher\n";
    }else{
        if(module == 0){
            try{
                mqtt::MqttPublisher pub(port.c_str(), ID.c_str(), 20);
                pub.MqttMessage(topic.c_str(), mssg.c_str(), 1);    
            }catch(const char *message){
                std::cerr << message << std::endl;
            }
        }else{
            try{
                mqtt::MqttSubscriber client(port.c_str(), ID.c_str());
                client.MqttConnect(topic.c_str(), 1);
            }catch(const char * message){
                std::cerr << message << std::endl;
            }
        }
    }
    std::cout << "<Module> " << argv[1] << " <API> " << ID << " <Port> " << port << " <Topic> " << topic << " <Message> " << mssg<< std::endl; 
    std::system("echo 'the module was finished running'");
    return 0;
}