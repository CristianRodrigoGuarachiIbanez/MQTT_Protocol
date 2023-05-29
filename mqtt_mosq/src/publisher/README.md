

#  MQTT in C++


This Programm was implemented in c++ using Mosquitto-Dev. The Publisher receives a host name and a port number in order to stablish a connection with a broker and deliver a message in a certain topic. 

## Installation of Mosquitto

        sudo apt update -y && sudo apt install mosquitto mosquitto-clients -y

## Installation of Mosquitto Libs

        sudo apt-get install libmosquitto-dev

## Setup 

    mkdir build
    cd build
    cmake -S ../ -B .
    make -C ./

## Run

    ./MQTT