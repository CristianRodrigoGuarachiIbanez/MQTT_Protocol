#include "unittest_sub.h"
#include <iostream>

namespace subscribe{
    MockPublisher::MockPublisher(const char *filename): 
        filename_(filename),
        QOS_(1)
    {
        //std::cout << " filename -> " << filename_ << std::endl;
    }
    void MockPublisher::sendPayload(const char * topic, const char *address, const char * clientID){
        if(std::string(topic)=="MQTT/Collision"){
            PAYLOAD_ = loadStringPayload(filename_, line_);
            assert(PAYLOAD_.size()>0);
            try{
                mqtt::MqttPublisher pub(address, clientID, 20);
                for(int i = 0; i<PAYLOAD_.size(); i++){
                    pub.MqttMessage(topic, PAYLOAD_[i].c_str(), 1);  
                }
                  
            }catch(const char *message){
                std::cerr << message << std::endl;
            }
        }else{

            std::cerr << " the topic " << topic << " could not be found\n";
        }
    }
 
    std::vector <std::string> MockPublisher::loadStringPayload(const char * filename, std::string&line){
        std::vector<std::string> payload;
        readFile(filename, payload, line);
        return payload;
    }

    void MockPublisher::readFile(const char * filename, std::vector<std::string>&payload, std::string&line){

        file_.open(filename, std::ios::in);
        if(!file_.is_open()){
            std::cerr << "[ERROR]: file could not be opend!\n";
            exit(1);
        }
        while(getline(file_,line)){
            size_t size = line.length();
            std::string p = line.substr(0,7);
            if((p.compare("PAYLOAD") ==0) && (size > 7)){
                payload.push_back(line.substr(8, size));
            }else{
                payload.push_back(" ");
            }
            
        }
        file_.close();
    }

    UnittestSub::UnittestSub(const char * filename, const char *TOPIC, const char *ADDRESS, const char *CLIENTID) : 
        pub_{filename},
        ADDRESS_(ADDRESS),
        CLIENTID_(CLIENTID),
        TOPIC_(TOPIC),
        client_{ADDRESS, CLIENTID}
    {
        
    }
    void UnittestSub::receivePayload(){
        client_.MqttConnect(TOPIC_, 1);
    }
    void UnittestSub::sendPayload(){
        sendPayloads(pub_, payloads_, TOPIC_, ADDRESS_, CLIENTID_);
    }
    UnittestSub::~UnittestSub()
    {
        payloads_.clear();
    }
    void UnittestSub::sendPayloads(MockPublisher&pub, std::vector<std::string>&payloads, const char *TOPIC, const char *ADDRESS, const char *CLIENTID){
        pub.sendPayload(TOPIC, ADDRESS, CLIENTID);
        payloads = pub.getPayload();
    }

    // void UnittestSub::runMultiThread(UnittestSub&subscribe){
    //     std::thread pub(subscribe,this, "send");
    //     std::thread sub(subscribe,this, "receive");
    //     pub.join();
    //     sub.join();
    // }

}
