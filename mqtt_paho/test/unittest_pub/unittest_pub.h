
#ifndef UNITTEST_PUB_H
#define UNITTEST_PUB_H

#include "paho_publisher/MQTTPublisher.h"
#include "paho_subscriber/MQTTSubscriber.h"
#include "assert.h"
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <thread>
namespace publish{

    class MockPublisher{
        public:
            explicit MockPublisher(const char *filename);
            std::string getPayload(){
                if(PAYLOAD_.size() > 0){
                    std::cout << " Payload -> " << PAYLOAD_ << std::endl;
                    return PAYLOAD_;
                }else{
                    std::cerr << "the payload is empty\n";
                    exit(1);
                }
            }
            void sendPayload(const char * topic, const char *address, const char * clientID);
        private:
            const char * filename_, * ADDRESS_, * CLIENTID_, * TOPIC_;
            std::string PAYLOAD_;
            int QOS_; 

            friend class UnittestSub;
            std::ifstream file_;
            void connectToBrocker(const char *address, const char * clientID);
            void loadStringPayload(const char * filename, std::string&payload);
            void readFile(const char * filename, std::string&payload);
            

    };


    class UnittestPub{   
    
    private:
        MockPublisher pub_;
        std::string payloads_;
        const char *TOPIC_, *ADDRESS_, *CLIENTID_;
        void sendPayloads(MockPublisher&pub, std::string&payloads, const char *TOPIC, const char *ADDRESS, const char *CLIENTID);
    public:
        UnittestPub(const char * filename, const char *TOPIC, const char *ADDRESS, const char *CLIENTID);
        ~UnittestPub();
        void sendPayload();

        
    };
}

#endif // UNITTEST_PUB_H