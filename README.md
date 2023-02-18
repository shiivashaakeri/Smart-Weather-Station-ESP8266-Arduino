# Smart Weather Station
In this project, we’ll implement an IoT system that is normally in Deep Sleep mode. It’ll
wake up every one minute to get the sensor data and publish it on out server using MQTT and
Node-Red. It will be awakened also by a pushbutton which restarts the ESP8266 development
board.
## Components
-  ESP8266 Development Board
-  Temperature and Humidity Sensor (SHT20)
-  Graphic Display Module (NOKIA-5110 Graphic LCD)
-  Breadboard and Wires
-  Battery (or a power bank)
-  Pushbutton

## Installation
To use this weather station, you will need to install the following software and libraries:
- Arduino IDE - The Arduino Integrated Development Environment (IDE) is used to write and upload code to the ESP8266 and Arduino boards. You can download the latest version of the Arduino IDE from the official website: https://www.arduino.cc/en/software
- ESP8266 Library - This library is required to program the ESP8266 Wi-Fi module using the Arduino IDE. You can install the ESP8266 library from the Arduino IDE's Library Manager.
- Adafruit SHT20 Library - This library is required to interface with the SHT20 sensor. You can download this library from the Arduino IDE's Library Manager.
## Usage
1- Connect the SHT20 sensor to the ESP8266 and Arduino board according to the wiring diagram provided in the "report.pdf".

2- Open the "SmartWeatherStation.ino" file in the Arduino IDE.

3- Set up the Wi-Fi connection by entering your Wi-Fi network name and password in the "ssid" and "password" variables.

4- Configure the server connection by entering the IP address and port number of your cloud server in the "server" and "port" variables.

5- Compile and upload the code to the ESP8266 and Arduino board.

6- Open the serial monitor to monitor the weather data collected by the SHT20 sensor and sent to the server.



## Acknowledgement
- [Create A Simple ESP8266 Weather Station With BME280](https://lastminuteengineers.com/bme280-esp8266-weather-station/?utm_content=cmp-true)
- [ESP8266 and Node-RED with MQTT ](https://randomnerdtutorials.com/esp8266-and-node-red-with-mqtt/)
- [ESP8266 NodeMCU MQTT Communication with ESP32 and Node-RED](https://microcontrollerslab.com/esp8266-nodemcu-mqtt-esp32-node-red/)

Please refer to these links to gain a better understanding of the project's concepts and techniques.
