#include <set>
#include <string>
#include <iostream>

class ErrorEnsamble{
    private:
        int type_;
        std::string description_;
    public:    
        ErrorEnsamble(const std::string& des, int type) : 
        description_(des), 
        type_(type){

        }
    int getType()const{
        return type_;
    }

    std::string getDescription()const{
        return description_;
    }

    bool operator < (const ErrorEnsamble& ee) const {
        return type_ < ee.getType();
    }
};


int main(){

    std::set<ErrorEnsamble> SetOfErrors;
    SetOfErrors.insert({"ValueError", 1010});
    SetOfErrors.insert({"TypeError", 1011});
    SetOfErrors.insert({"memoryError", 1210});
   

    for(const auto& error : SetOfErrors){
        std::cout << error.getDescription() << " " << error.getType() << std::endl;
    }
    return 0;
}