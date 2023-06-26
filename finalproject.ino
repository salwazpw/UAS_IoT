#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
int led = D4;
int pirpin = D3;
int pirState = LOW;
int val = 0;

// WiFi
const char*ssid = "OPPO A54";
const char*password = "ssssssss";
const char* mqttTopic = "cahaya/bidadari";

// MQTT
const char* mqtt_server = "broker.hivemq.com";
bool isMqttConnected = false;
//int lux;
#define ldrpin D6
LiquidCrystal_I2C lcd(0x27,16,2);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      isMqttConnected = true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
pinMode(led, OUTPUT);
pinMode(pirpin, INPUT);
pinMode(ldrpin, INPUT);
Serial.begin(9600);
lcd.init();
lcd.backlight();
lcd.setCursor(1,0);
lcd.print("LAMPU OTOMATIS");
lcd.setCursor(2,1);
lcd.print("Kelompok 02");
delay(2000);
lcd.clear();
  // put your setup code here, to run once:
setup_wifi();
client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected() && !isMqttConnected) {
    reconnect();
  }
  if (!client.loop()) {
    client.connect("ESP8266Client");
  }
  val = digitalRead(pirpin);
 // lux = digitalRead(7);

  if(val == LOW && digitalRead(ldrpin) == HIGH){
    digitalWrite(led, LOW);
      lcd.setCursor(0,0);
      lcd.print("Ruangan Redup");
      client.publish(mqttTopic, "ON");
    if(pirState == HIGH){
      lcd.setCursor(0,1);
      lcd.print("Tidak Ada Gerakan");
    }
  }else
  if(val == HIGH && digitalRead(ldrpin) == HIGH){
    digitalWrite(led, LOW);
    lcd.setCursor(0,0);
    lcd.print("Ruangan Redup");
    client.publish(mqttTopic, "ON");
    if(pirState == LOW){
      lcd.setCursor(0,1);
      lcd.print("Ada Gerakan");
    }
    
  }
  else
    if(val == HIGH && digitalRead(ldrpin) == LOW){
      digitalWrite(led, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Ruangan Terang");
      client.publish(mqttTopic, "OFF");
      if(pirState == LOW){
        lcd.setCursor(0,1);
        lcd.print("Ada Gerakan");
      }
    }
  }
