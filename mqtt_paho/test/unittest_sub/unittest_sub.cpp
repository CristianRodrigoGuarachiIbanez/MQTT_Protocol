#include "unittest_sub.h"
#include <iostream>

namespace subscribe{
    
    UnittestSub::UnittestSub(const char *TOPIC, const char *ADDRESS, const char *CLIENTID) : TOPIC_(TOPIC),
        ADDRESS_(ADDRESS),
        CLIENTID_(CLIENTID),
        client_(ADDRESS, CLIENTID)
    {
        std::cout << " constructor " << ADDRESS_ << CLIENTID_ << std::endl;
    }
    UnittestSub::~UnittestSub(){
        
    }
    void UnittestSub::receivePayload(){
        client_.MqttConnect(TOPIC_, 1);
 
    }

}
