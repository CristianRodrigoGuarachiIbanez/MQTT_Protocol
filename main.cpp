#include <iostream>
#include <string>
#include <functional>
#include <utility>
#include <memory>

typedef void(Manager)(const std::string&msg, std::string&input);

struct Print{
    Print(const std::string&msg, std::string&input){
        std::cout << "desde dentro de la Struct -> "<< input << std::endl;
        std::cout << "print shared_ptr -> "<<msg << std::endl;
    }
};

template <Manager manager> class SubPrint{
    SubPrint(std::string&msg) : _cl{"message from SubPrint ->"},
    _msg(msg)
    {
        std::cout << "template class" << std::endl;
    }
    std::string getMsg(){
        return _msg;
    }
    private:
    std::string _cl, _msg;
};

template<Manager manager>Print printt(std::string&inf){
    const auto h = std::make_shared<SubPrint<manager>>(inf);
    return Print(h->getMsg(), manager);
}



void pprint(std::string&in){
    std::cout<< in <<std::endl;
}

int main(){

    std::string Msg("hola tu pendejo");
    std::function<void(std::string&)> p = pprint;
    p(Msg);
    return 0;
}