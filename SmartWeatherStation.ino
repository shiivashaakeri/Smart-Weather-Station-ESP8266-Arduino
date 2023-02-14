#ifndef STASSID
// Define the wifi information
#define STASSID "MobinNet6879-1"
#define STAPSK  "EB8E3J8F7EA"
#define MQTT_HOST IPAddress(192, 168, 1, 153)

#define MQTT_PORT 1883
// Define the tpoics we're going to publish
#define MQTT_PUB_TEMP "esp/dht/temperature"
#define MQTT_PUB_HUM "esp/dht/humidity"
#define MQTT_PUB_WiFi "WiFiConnection"


#endif

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "DFRobot_SHT20.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <String.h>


// Define the client
AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

// Define event handlers
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisonnectHandler;
Ticker wifiReconnectTimer;

unsigned long previousMillis = 0;
const long interval = 50;


void connectToWiFi(){  // Function: connect to WiFi.
  Serial.println();
  Serial.println("Connecting to WiFi...");
  WiFi.begin(STASSID, STAPSK); 
}

void onWiFiConnect(const WiFiEventStationModeGotIP& event){ // Function: check if the WiFi is connected.
  Serial.println("Connected To WiFi.");
  connectToMqtt();
}

void onWiFiDisconnect(const WiFiEventStationModeDisconnected& event){ // Function: check if the WiFi is disconnected.
  Serial.println("Disconnected from WiFi.");
  mqttReconnectTimer.detach();
  wifiReconnectTimer.once(2, connectToWiFi);  
  }

void connectToMqtt(){ // // Function: Connect to mqtt client.
  Serial.println("Connecting to MQTT..."); 
  mqttClient.connect();

}

void onMqttConnect(bool sessionPresent){ // Function: Check if mqtt is connected.
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason){ // Function: Check if mqtt is disconnected.
  Serial.println("Disconnected from MQTT.");
  if(WiFi.isConnected()){
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetID){ // Function: Check if publish is done.
  Serial.print("Publish acknowledged.");
  Serial.print("packetID:");
  Serial.println(packetID);
}

//Define Push Button
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button = {13, 0, false}; //D7

void IRAM_ATTR isr(void* arg) { //Function: Define pushbutton action: press.
  Button* s = static_cast<Button*>(arg);
  s->numberKeyPresses += 1;
  s->pressed = true;
}

void IRAM_ATTR isr() {
  button.numberKeyPresses += 1;
  button.pressed = true;
}

//Define SHT20 Sensor
Adafruit_PCD8544 display = Adafruit_PCD8544(15, 2, 0, 4, 5); // SHT20 GPIO connections.
int contrastValue = 60;
DFRobot_SHT20 sht20; // Define SHT20 as a sensor

///Web server
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80); //Define Server



void setup(void)
{

  Serial.begin(115200);
  Serial.println("I'm awake");
  delay(500); 
 
  wifiConnectHandler = WiFi.onStationModeGotIP(onWiFiConnect);
  wifiDisonnectHandler = WiFi.onStationModeDisconnected(onWiFiDisconnect);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT) ;

  connectToWiFi();
  delay(10000);

  if(WiFi.isConnected()){ //Publish Wifi Connected.

    uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_WiFi, 1, true, String(STASSID).c_str());

  }
  
  //pinMode(button.PIN, INPUT_PULLUP);

  attachInterrupt(button.PIN, isr, FALLING);



  // initialize the display
  display.begin();
  Wire.begin(D6, D5);  // set I2C pins [SDA = D6, SCL = D5], default clock is 100kHz
  display.setContrast(contrastValue);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(3, 0);
  display.print("SHT20 SENSOR");
  display.display();
  display.setCursor(7, 10);
  display.print("TEMPERATURE:");
  display.setCursor(16, 31);
  display.print("HUMIDITY:");
  display.display();


  sht20.initSHT20();
  delay(100);
  sht20.checkSHT20();
  delay(100);

  // SHT20 reads the temprature and humidity
  float humd = sht20.readHumidity();
  float temp = sht20.readTemperature();
    
  Serial.println();
  Serial.print(temp);
  Serial.print(" ");
  Serial.print(humd);
  Serial.println();

  

  // Publish temprature and humidity
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());
  Serial.println("Temp Sent");
  uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(humd).c_str());
  
  Serial.println("Humd Sent");
  
  delay(1000);

  // Show sensor data on NOKIA LCD
  display.setCursor(15, 20);
  if(temp < 0)
    display.printf("-%02u.%02u C", (int)abs(temp) % 100, (int)(abs(temp) * 100) % 100 );
  else
    display.printf(" %02u.%02u C", (int)temp % 100, (int)(temp * 100) % 100 );
    display.drawRect(53, 20, 3, 3, BLACK); // print degree symbol ( ° )
  
  display.setCursor(9, 41);
  display.printf("%04u.%02u", (int)(humd/100), (int)((uint32_t)humd % 100) );
  
  display.display();
  
  delay(10000);
  String noneCon = "Disconnected";
  // Publish WiFi disconnection.
  uint16_t packetIdPub4 = mqttClient.publish(MQTT_PUB_WiFi, 1, true, noneCon.c_str()); 
  delay(500);


  display.clearDisplay();
  display.display();
  Serial.println("I'm awake, but I'm going into deep sleep mode for 60 seconds");

  // Deep Sleep for 1 minute
  ESP.deepSleep(60e6); //60 second

}
 
// main loop
void loop(){

}
