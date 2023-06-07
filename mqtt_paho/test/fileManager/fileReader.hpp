
#include "assert.h"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

namespace FileManager{
    class FileReader{
        private:
            const char * filename_;
            std::ifstream file_;
            std::string line_, msg_;
            void read();
            int nthSubstr(int n);
            static bool is_empty(std::ifstream& pFile){
                return pFile.peek() == std::ifstream::traits_type::eof();
            }
        public:
            std::string lastMsg_;
            explicit FileReader(const char * filename);

            std::string getLastMessage(){
                std::string lastSig("|");
                std::string lastMsg;
                std::string copy(msg_);
                if(msg_.size()>0){
                    size_t n = std::count(msg_.begin(), msg_.end(), '|');
                    int lastPos = msg_.find_last_of("|");
                    //std::cout << " position" <<lastPos << " "<< msg_ << std::endl;
                    if(lastPos != std::string::npos && n > 1){
                        int pLastPos = nthSubstr(n-1); 
                        lastMsg = msg_.substr(pLastPos +2, msg_.size() - pLastPos);
                        //std::cout << " ex " << lastMsg << "  " << msg_.size() - pLastPos << std::endl;
                    }else{
                        lastMsg = msg_;
                    }     
            
                    return lastMsg;
                }
            }
            bool operator == (FileManager::FileReader&fileReader) const {
                return (lastMsg_.size() == fileReader.lastMsg_.size() && lastMsg_ == fileReader.lastMsg_);
            }
    };

}
