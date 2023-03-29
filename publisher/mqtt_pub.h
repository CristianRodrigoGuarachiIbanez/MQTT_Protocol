#include <stdio.h>
#include <mosquitto.h>

#include <string>

namespace mqtt_pub{

    class publisher{
        public:
            explicit publisher(const char * pr, const char *host, int port, bool pr2);
            ~publisher(){
                mosquitto_disconnect(_mosq);
                mosquitto_destroy(_mosq);
                mosquitto_lib_cleanup();
                free(_msg);
            }
            void set_message(char hostname[], char*argv[], int argc);
            void publish(const char * topic, int qual);
        
            std::string get_msg(){
                return std::string(this->_msg);
            }
        private:
            struct mosquitto *_mosq;
            int _rc;
            char *_msg;
            char**_arg;

            char*concat(const char * a, const char *b);
            const char * get_message(){
                return _msg; 
            }
    };

}