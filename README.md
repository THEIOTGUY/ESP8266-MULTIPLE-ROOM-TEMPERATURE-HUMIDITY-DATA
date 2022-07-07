# RECORDING TEMPERATURE AND HUMIDITY OF TWO DIFFERENT ROOMS AND UPLOADING IT TO NODE-RED DASHBOARD

## DESCRIPTION

In this project we are going to use 2 nodemcu to record Temperature and Humidity of two different rooms and then we are going to upload the data to node-red dashboard and also control a relay in each room using MQTT protocol 

## COMPONENTS

1.ESP8266 (2 MODULES)
2.DHT11 SENSOR (2 MODULES) (TEMPERATURE AND HUMIDITY SENSOR)
3.RELAY MODULE 5V (2 MODULES)
4.POWER SUPPLY FOR NODEMCU

## CIRCUIT DIAGRAM

YOU HAVE TO APPLY SAME CONNECTION FOR NODEMCU OF BOTH ROOM

![NODEMCU TEMP](https://user-images.githubusercontent.com/102857010/177711811-e1134612-2847-420b-a19f-3b856216d4c4.png)

## MQTT BROKER

You have to install a MQTT broker service like ActiveMQ or MosquitoMQ , i prefer watching this video 
* https://youtu.be/2zMs-Gj6LSU

## NODE-RED DASHBOARD

![NODE-RED](https://user-images.githubusercontent.com/102857010/177718484-b1936ba0-bbd5-4a19-9ce3-61bc460ae6e3.png)

![NODE-RED DASHBOARD](https://user-images.githubusercontent.com/102857010/177718504-e8be6efe-9de4-47de-8070-310a63eb8950.png)


## CODE FOR ROOM1 NODEMCU
````
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include"DHT.h"
#define DHTTYPE DHT11
uint8_t DHTPin = D8;
const int relay =D5;
DHT dht(DHTPin,DHTTYPE);

float Temperature;
float Humidity;
char ch='°';


WiFiClient net;
MQTTClient client;

String ssid="Vaidande_2";
String pass="960647@vaidande";
unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("nodemcu_client")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  client.subscribe("nodemcu/Humidity1");
    client.subscribe("nodemcu/Temperature1");
    client.subscribe("nodemcu/Relay1");
 
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
    if(topic=="nodemcu/Relay1"){
        if(payload=="ON"){
     digitalWrite(relay,HIGH);
         
     Serial.print(payload);
            
        }
        }
    
    
   if(topic=="nodemcu/Relay1"){
        if(payload=="OFF"){
        digitalWrite(relay,LOW);
}
   
    }
    }
    
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
    pinMode(DHTPin, INPUT);
    pinMode(relay, OUTPUT);

  dht.begin();
  
    
   // attaches the servo on D8 to theservo object
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("192.168.137.152", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
    Temperature =dht.readTemperature();
  Serial.print(Temperature);
  Serial.print("°C");
  Serial.println(""); 
  Humidity =dht.readHumidity();
  Serial.print(Humidity);
  Serial.print("%");
  Serial.println(""); 
  
  
  client.loop();
  delay(200); // <- fixes some issues with WiFi stability
    client.publish("nodemcu/Temperature1",String(Temperature).c_str());
    delay(200);

    client.publish("nodemcu/Humidity1",String(Humidity).c_str());

  if (!client.connected()) {
    connect();
       }
    }
    ````
    
    ## CODE FOR NODEMCU OF ROOM 2
    ````
    #include <ESP8266WiFi.h>
#include <MQTT.h>
#include"DHT.h"
#define DHTTYPE DHT11
uint8_t DHTPin = D8;
const int relay =D5;
DHT dht(DHTPin,DHTTYPE);

float Temperature;
float Humidity;
char ch='°';


WiFiClient net;
MQTTClient client;

String ssid="Vaidande_2";
String pass="960647@vaidande";
unsigned long lastMillis = 0;

const int led= D2;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("nodemcu_client")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nconnected!");

  client.subscribe("nodemcu/Humidity2");
    client.subscribe("nodemcu/Temperature2");
    client.subscribe("nodemcu/Relay2");
 
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
    if(topic=="nodemcu/Relay2"){
        if(payload=="ON"){
     digitalWrite(relay,HIGH);
         
     Serial.print(payload);
            
        }
        }
    
    
   if(topic=="nodemcu/Relay2"){
        if(payload=="OFF"){
        digitalWrite(relay,LOW);
}
   
    }
    }
    
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
    pinMode(DHTPin, INPUT);
    pinMode(relay, OUTPUT);

  dht.begin();
  
    
   // attaches the servo on D8 to theservo object
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("192.168.137.152", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
    Temperature =dht.readTemperature();
  Serial.print(Temperature);
  Serial.print("°C");
  Serial.println(""); 
  Humidity =dht.readHumidity();
  Serial.print(Humidity);
  Serial.print("%");
  Serial.println(""); 
  
  
  client.loop();
  delay(200); // <- fixes some issues with WiFi stability
    client.publish("nodemcu/Temperature2",String(Temperature).c_str());
    delay(200);

    client.publish("nodemcu/Humidity2",String(Humidity).c_str());

  if (!client.connected()) {
    connect();
       }
    }
    ````
