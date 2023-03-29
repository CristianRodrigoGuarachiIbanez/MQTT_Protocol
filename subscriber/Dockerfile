FROM gcc:latest

COPY . /usr/src/test
WORKDIR /usr/src/test

RUN g++ -o subscriber main.cpp mqtt_sub.cpp -lmosquitto

CMD ["./subscriber"]