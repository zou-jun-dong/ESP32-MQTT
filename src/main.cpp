#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <secrets.h>

WiFiClient espClient;  //Creat a Wi-Fi client
PubSubClient client(espClient);  //Pass the WiFi client to the MQTT client

const char* mqtt_server="broker.emqx.io";  //Free public test broker provided by EMQX
const int mqtt_port=1883;   //Default non-encrypted port for MQTT
const char* led_topic="foshan/ESP32/ledControl";
const char* sensor_topic="foshan/ESP32/sensorData";  //Publish topic
const int ledPin=2;

unsigned long lastTime=0;  //Time tracking variable

//Initialize WiFi
void setup_wifi(){
	WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
	Serial.print("Connecting to");
	Serial.println(WIFI_SSID);
	while (!WiFi.isConnected())
	{
		delay(500);
		Serial.print("."); 
	}
	Serial.println("");
	Serial.println("WiFi connected!");
	Serial.print("WiFI address:");
	Serial.println(WiFi.localIP());
}

void reconnect() {
	while (!client.connected())
	{
		Serial.println("Attempting MQTT connection...");
		//Create a random client ID
		String clientId="ESP32Client-";
		clientId+=String(random(0xffff),HEX);
		if (client.connect(clientId.c_str()))
		{
			/* code */
			Serial.println("connected!");
			client.subscribe(led_topic);
		}
		else{
            Serial.print("failed,rc=");
			Serial.print(client.state());
			Serial.println("try again 5 seconds");
			delay(5000);
		}
		
	}
	
}

void callback(char* topic,byte* palyload,unsigned int length) {
	String message;
	for (int i = 0; i < length; i++)
	{
		message+=(char)palyload[i];
	}
	Serial.println(message);
	if (message=="ON")
	{
		digitalWrite(ledPin,HIGH);
		Serial.println("LED is now ON");
	}else if (message=="OFF")
	{
		digitalWrite(ledPin,LOW);
		Serial.println("LED is now OFF");
	}
	
	
}

void setup(){
	Serial.begin(115200);  
	pinMode(ledPin,OUTPUT);
	setup_wifi();  //Start Wi-Fi
    client.setServer(mqtt_server,mqtt_port);  //Set the MQTT broker address and port
	client.setCallback(callback);
}

void loop(){
	if (!client.connected())
	{
		reconnect();
	}
  client.loop();  //Maintain MQTT client heartbeat and network processing
  unsigned long now=millis();  //Get current uptime in milliseconds
  if (now-lastTime>5000)
  {
	lastTime=now;
	float temp=random(200,350)/10;  //Generate a random float between 20.0 and 35.0 to simulate temperature
	//Convert float to C++ String
	String tempStr=String(temp,1);  //Keep 1 decimal place
	Serial.println(tempStr);
	client.publish(sensor_topic,tempStr.c_str());
  }
  
}