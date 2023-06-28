# mqtt_drafts
This is a sample project for MQTT C++ library https://github.com/eclipse/paho.mqtt.cpp.
You need to run a MQTT server (by default on localhost:1883), e.g. mosquitto, and have paho-mqtt library installed.

The clients use timestamps as payload, include their hostname and PID in the MQTT topic.
The 'server' (which is a MQTT subscriber client) prints the message statistics from known clients.

# Example output
![image](https://github.com/lacca0/mqtt_drafts/assets/20230553/c591e74c-9d61-4d78-aa2d-ed360929ffa6)
