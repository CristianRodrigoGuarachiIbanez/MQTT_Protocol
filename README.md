# MQTT Protocol

## Install Mosquitto on Ubuntu

    sudo apt update -y && sudo apt install mosquitto mosquitto-clients -y

## Setup the Mostquitto- Libs 
    
    sudo apt-get install libmosquitto-dev

## PUBLISHER 

    This programm is emplemented in C++ using Mosquitto dev. The Publisher receives information like host name and port number to stablish a connection to the broker and deliver a message.


## SUBSCRIBER 
    This programm is emplemented in C++ using Mosquitto dev. The Subscriber receives information like host name and port number to stablish a connection to the broker and listen for messages. 


## Installation in Linux
    To build this programm follow the following instructions:

        - install CMake 

                sudo apt-get install cmake cmake-qt-gui 
        
            the last part install the graphic interface. This could be left out.

        - open the terminal
        - insert the following commads:
                
                mkdir ./build
                cmake -S ./ -B ./build
                make -C ./build

## Run 

Once the last step was successfully executed, then the following line can be inserted:

        ./build/MQTT


    


