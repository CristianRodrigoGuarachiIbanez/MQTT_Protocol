#include "fileManager/fileReader.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

int main(int argc, char **argv){
    // FileManager::FileReader input("/home/cristian/c++Project/mqtt/mqtt_paho/test/in_out_files/unittest_1.txt");
    // FileManager::FileReader output("/home/cristian/c++Project/mqtt/mqtt_paho/test/in_out_files/received_message.txt");
    // std::cout << input.getLastMessage() << " vs " << output.getLastMessage() << std::endl;
    // bool equal = input == output;
    // if(equal){
    //     std::cout << "they are equal\n";
    // }else{
    //     std::cout << "They are not equal\n";
    // }

    ::testing::InitGoogleTest(&argc, argv);
    auto result = EXIT_FAILURE;
    try {
        result = RUN_ALL_TESTS();
       
    } catch (const std::exception& e) {
        std::cerr << "EXCEPTION CAUGHT:\n" << e.what() << std::endl;
    } catch (...) {
        std::cerr << "UNKNOWN EXCEPTION CAUGHT" << std::endl;
    }
    std::cout << "result -> " << result << std::endl;
    // // mosqpp::lib_cleanup();
    return result;
}