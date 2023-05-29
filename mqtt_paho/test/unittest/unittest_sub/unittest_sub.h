#ifndef UNITTEST_SUB_H
#define UNITTEST_SUB_H

#include "paho_publisher/MQTTPublisher.h"
#include "paho_subscriber/MQTTSubscriber.h"
#include "assert.h"
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <thread>

namespace subscribe{
    class MockPublisher{
        public:
            explicit MockPublisher(const char *filename);
            std::vector<std::string> getPayload(){
                if(PAYLOAD_.size() > 0){
                    for(int i = 0; i<PAYLOAD_.size(); i++){
                        std::cout << " Payload -> " << PAYLOAD_[i] << std::endl;
                    }
                    return PAYLOAD_;
                }else{
                    std::cerr << "the payload is empty\n";
                    exit(1);
                }
            }
            void sendPayload(const char * topic, const char *address, const char * clientID);
        private:
            const char * filename_, * ADDRESS_, * CLIENTID_, * TOPIC_;
            std::vector<std::string> PAYLOAD_;
            int QOS_; 

            friend class UnittestSub;
            std::ifstream file_;
            std::string line_;
            void connectToBrocker(const char *address, const char * clientID);
            std::vector<std::string> loadStringPayload(const char * filename, std::string&line);
            void readFile(const char * filename, std::vector<std::string>&payload, std::string&line);
            

    };

    class UnittestSub
    {   
    
    private:
        mqtt::MqttSubscriber client_;
        MockPublisher pub_;
        std::vector<std::string> payloads_;
        const char *TOPIC_, *ADDRESS_, *CLIENTID_;
        void sendPayloads(MockPublisher&pub, std::vector<std::string>&payloads, const char *TOPIC, const char *ADDRESS, const char *CLIENTID);
    public:
        UnittestSub(const char * filename, const char *TOPIC, const char *ADDRESS, const char *CLIENTID);
        ~UnittestSub();
        void receivePayload();
        void sendPayload();
        void runMultiThread(UnittestSub&sub);
        void operator()(UnittestSub&sub, const char * action){
            if(action == "send"){
                sub.sendPayload();
            }else{
                sub.receivePayload();
            }
        }
        
    };
    
    
}
#endif