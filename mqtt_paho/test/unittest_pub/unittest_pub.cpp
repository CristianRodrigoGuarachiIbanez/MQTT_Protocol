


#include "unittest_pub.h"

namespace publish{

    MockPublisher::MockPublisher(const char *filename): 
        filename_(filename),
        QOS_(1)
    {
        //std::cout << " filename -> " << filename_ << std::endl;
    }
    void MockPublisher::sendPayload(const char * topic, const char *address, const char * clientID){
        if(std::string(topic)=="MQTT/Collision"){
            loadStringPayload(filename_, PAYLOAD_);
            assert(PAYLOAD_.size()>0);
            try{
                mqtt::MqttPublisher pub(address, clientID, 20);
                pub.MqttMessage(topic, PAYLOAD_.c_str(), 1);  
                
                  
            }catch(const char *message){
                std::cerr << message << std::endl;
            }
        }else{

            std::cerr << " the topic " << topic << " could not be found\n";
        }
    }
 
    void MockPublisher::loadStringPayload(const char * filename, std::string&payload){
        readFile(filename, payload);
        return;
    }

    void MockPublisher::readFile(const char * filename, std::string &payload){
        std::string line;
        file_.open(filename, std::ios::in);
        if(!file_.is_open()){
            std::cerr << "[ERROR]: file could not be opend!\n";
            exit(1);
        }
        while(getline(file_, line)){
            if(line.find("PAYLOAD")!=std::string::npos){
                size_t pos = line.find(":");
                std::string msg = line.substr(pos+2);
                payload.append(msg);
            }else{
                std::cout << "not payload -> " << line << std::endl;
            }     
        }
        payload.append("\n");
        file_.close();
    }




    UnittestPub::UnittestPub(const char * filename, const char *TOPIC, const char *ADDRESS, const char *CLIENTID) : 
        pub_{filename},
        ADDRESS_(ADDRESS),
        CLIENTID_(CLIENTID),
        TOPIC_(TOPIC)
    {
        
    }

    void UnittestPub::sendPayload(){
        sendPayloads(pub_, payloads_, TOPIC_, ADDRESS_, CLIENTID_);
    }
    
    UnittestPub::~UnittestPub()
    {
        payloads_.clear();
    }
    void UnittestPub::sendPayloads(MockPublisher&pub, std::string&payloads, const char *TOPIC, const char *ADDRESS, const char *CLIENTID){
        pub.sendPayload(TOPIC, ADDRESS, CLIENTID);
        payloads = pub.getPayload();
    }

}

