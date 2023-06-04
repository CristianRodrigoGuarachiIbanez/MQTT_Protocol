
#include "assert.h"
#include <fstream>
#include <string>
#include <iostream>

namespace FileManager{
    class FileReader{
        private:
            const char * filename_;
            std::ifstream file_;
            std::string line_, msg_;
            void read();
            static bool is_empty(std::ifstream& pFile){
                return pFile.peek() == std::ifstream::traits_type::eof();
            }
        public:
            explicit FileReader(const char * filename);
            std::string getLastMessage()const{
                std::string lastMsg;
                if(msg_.size()>0){
                    size_t pos = msg_.find(" | ");
                    if(pos != std::string::npos){
                        lastMsg = msg_.substr(msg_.size() - (pos + 3), pos);
                    }else{
                        lastMsg = msg_;
                    }     
            
                    return lastMsg;
                }
            }
            bool operator == (FileManager::FileReader&fileReader) const {
                return (this->getLastMessage().size() == fileReader.getLastMessage().size() && getLastMessage() == fileReader.getLastMessage());
            }
    };

}
