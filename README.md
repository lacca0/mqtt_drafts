# mqtt_drafts
This is a sample project for MQTT C++ library https://github.com/eclipse/paho.mqtt.cpp.
You need to run a MQTT server (by default on localhost:1883), e.g. mosquitto, and have boost and paho-mqtt libraries installed.

The clients use time as a data example and send messages over MQTT.
The 'server' (which is a MQTT subscriber client) prints the message statistics from known clients.
