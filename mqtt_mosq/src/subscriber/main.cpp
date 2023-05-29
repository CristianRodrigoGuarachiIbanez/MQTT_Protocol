#include "mqtt_sub.h"


int main(){
    subscriber sub("subscriber", "test/round2", "localhost", 1883, 12);
    return 0;
}