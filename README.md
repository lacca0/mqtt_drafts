# mqtt_drafts
This is a sample project for MQTT C++ library https://github.com/eclipse/paho.mqtt.cpp.
You need to run a MQTT server (by default on localhost:1883), e.g. mosquitto, and have boost and paho-mqtt libraries installed.

The clients use time as a data example and send messages over MQTT.
The 'server' (which is a MQTT subscriber client) prints the message statistics from known clients.

# Example output
![image](https://github.com/lacca0/mqtt_drafts/assets/20230553/c591e74c-9d61-4d78-aa2d-ed360929ffa6)
