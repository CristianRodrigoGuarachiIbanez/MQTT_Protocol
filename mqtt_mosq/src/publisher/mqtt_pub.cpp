#include "mqtt_pub.h"
#include <iostream>
#include <cstring>

namespace mqtt_pub{
    publisher::publisher(const char * pr, const char *host, int port, bool pr2){
        mosquitto_lib_init();
        this->_mosq = mosquitto_new(pr, pr2, NULL);
        _rc = mosquitto_connect(_mosq, host, port, 60);
        if(_rc !=0){
            std::cout<< "Client could not connect to the broker! Error code: %d\n" << _rc << std::endl;
            mosquitto_destroy(_mosq);
            std::cerr<< "quitted"<< std::endl;
            exit(0);
        }
        std::cout << "connected" << std::endl;

    }
    char *publisher::concat(const char * a, const char *b){

        int la = strlen(a);
        int lb = strlen(b);
        char * con = (char*)malloc(la+lb+1);

        memcpy(con, a, la);
        memcpy(con + la, b, lb+1);
        return con;
    }

    void publisher::set_message(char hostname[], char*argv[], int argc){
        this->_msg = hostname;
        this->_msg = concat(this->_msg, " ");
        this->_arg = argv;
        if(argc > 0){
            std::cout << "current message:" << std::endl; 

            for(int i = 0; i<argc; i++)
            {
                std::cout<< _arg[i] << std::endl;

                if(i>3){
                    _msg = concat(_msg, _arg[i]);
                    _msg = concat(_msg, " ");
                } 
            }
            
        }else{
            std::string msgg = "is anybody there?";
            strcpy(_msg, msgg.c_str());
            std::cout << "no message introduced:" << _msg<< std::endl;
        }
    }
    void publisher::publish(const char * topic, int qual){
        int channel = strlen(get_message());

        mosquitto_publish(this->_mosq, NULL, topic, channel, get_message(), qual, false);
        // mosquitto_disconnect(mosq);
        // mosquitto_destroy(mosq);
        // mosquitto_lib_cleanup();
    }

    /*char *concat(const char * a, const char *b){

        int la = strlen(a);
        int lb = strlen(b);
        char * con = (char*)malloc(la+lb+1);

        memcpy(con, a, la);
        memcpy(con + la, b, lb+1);
        return con;
    }*/

}