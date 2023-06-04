#include "fileManager/fileReader.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class ApplicationTest : public ::testing::Test{
    public:
    FileManager::FileReader input_;
    FileManager::FileReader output_;
    std::string in_, out_;

    ApplicationTest() : input_("/home/cristian/c++Project/mqtt/mqtt_paho/test/in_out_files/unittest_1.txt"), 
        output_("/home/cristian/c++Project/mqtt/mqtt_paho/test/in_out_files/received_message.txt")
    {

    }
    virtual void SetUp(){

        in_ = input_.getLastMessage();
        out_ = output_.getLastMessage();
    }

    virtual void TearDown() {
        std::cout << " the test was succesfully run! \n";
    }
    
};


TEST_F(ApplicationTest, inOutTest){
    EXPECT_THAT(in_, out_);
}

// TEST_F(ApplicationTest, ObjectTest){
//     EXPECT_THAT(input_, output_);
// }