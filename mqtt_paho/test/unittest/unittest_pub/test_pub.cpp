#include "unittest_pub.h"
#include "gtest/gtest.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World! this is working"
#define QOS         1
#define TIMEOUT     10000L
// g++ ../src/paho_publisher/MQTTPublisher.cpp  test_pub.cpp -lgtest -lpthread -lgtest_main -lpaho-mqtt3cs -o test



TEST(MqttPublisher_test, test_pub){

    mqtt::MqttPublisher pub(ADDRESS, CLIENTID, 20);
    pub.MqttMessage(TOPIC, PAYLOAD, QOS);
    EXPECT_EQ(0, mqtt::deliveredtoken_);

}

// int main(int argc, char ** argv){

//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();

// }