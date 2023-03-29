#include "mqtt_sub.h"

subscriber::subscriber(const char * ID, const char*topic, char * hostname, int port, int obj):
    _ID(ID),
    _hostname(hostname),
    _topic(topic), 
    _port(port),
    _obj(obj)
    {
    mosquitto_lib_init();
    _mosq = mosquitto_new(_ID, true, &_obj);
    mosquitto_connect_callback_set(_mosq, on_connect);
    //subscription(mosq, this->_topic, 0);

    mosquitto_message_callback_set(_mosq, on_message);

    _rc = mosquitto_connect(_mosq, _hostname, _port, 10);
    if(_rc){
        std::cout<<"Could not connect to brocker with return code" << _rc<< std::endl;
        exit(-1);
    }
    mosquitto_loop_start(_mosq);
    std::cout << "Press enter to quit ..." << std::endl;
    getchar();
    mosquitto_loop_stop(_mosq, true);

}
subscriber::~subscriber(){
    mosquitto_disconnect(_mosq);
    mosquitto_destroy(_mosq);
    mosquitto_lib_cleanup();
}
void subscriber::on_connect(struct mosquitto *mosq, void*clientObj, int rc){
    std::cout << "current used ID: " << *(int*)clientObj << std::endl;
    if(rc){
        std::cout<< "Error with the result code" << rc<< std::endl;
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "test/round2", 0);
}

void subscriber::subscription(struct mosquitto * mosq, const char *topic, int qsl){
    mosquitto_subscribe(mosq, NULL, topic, qsl);
}

void subscriber::on_message(struct mosquitto * mosq, void*clientObj, const struct mosquitto_message *msg){
    std::cout<< "new message with topic " << msg->topic << " : " << (char*)msg->payload << std::endl;
}

