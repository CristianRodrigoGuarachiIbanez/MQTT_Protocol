#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include <mosquitto.h>

class subscriber{
    public:
        explicit subscriber(const char * ID, const char*topic, char * hostname, int port, int obj);
        ~subscriber();
        void subscription(struct mosquitto * mosq, const char *topic, int qsl);
        static void on_connect(struct mosquitto *mosq, void*clientObj, int rc );
        static void on_message(struct mosquitto * mosq, void*clientObj, const struct mosquitto_message *msg);

    private:
        const char * _topic, *_ID;
        char * _hostname;
        struct mosquitto *_mosq;
        int _obj;
        int _rc, _port;
};