#include "fileReader.hpp"
#include <iostream>

namespace FileManager{
    FileReader::FileReader(const char * filename): filename_(filename)
    {  
        read();
    }

    void FileReader::read(){
        file_.open(filename_, std::ios::in);
   
        if(!file_.is_open()){
            std::cerr << "[ERROR] file reader could be opened!" << std::endl;
            exit(1);
        }
        if(is_empty(file_)){
            std::cerr << "File is empty" << std::endl;
            exit(1);
        }
        while(getline(file_, line_)){
            std::size_t pos;
            std::string m;
            if(!line_.empty()){
                pos = line_.find(" : ");
                m = line_.substr(pos + 3);
                //std::cout <<" message "<< m << " " << pos << std::endl;
            }else{
                //m.append(" | ");
                continue;
            }
            msg_.append(m);
            msg_.append(" | ");
        }
        // std::cout << "msg " << msg_ << std::endl;
        file_.close();
    }
    

}