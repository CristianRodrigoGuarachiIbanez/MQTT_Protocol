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
    
    class UnittestSub
    {   
    
    private:
        mqtt::MqttSubscriber client_;
        const char *TOPIC_, *ADDRESS_, *CLIENTID_;
    public:
        UnittestSub(const char *TOPIC, const char *ADDRESS, const char *CLIENTID);
        ~UnittestSub();
        void receivePayload();
        
    };
    
    
}
#endif