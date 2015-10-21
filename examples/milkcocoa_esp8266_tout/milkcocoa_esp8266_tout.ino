#include <ESP8266WiFi.h>
#include <Milkcocoa.h>
extern "C" {
#include "user_interface.h"
}

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "...SSID..."
#define WLAN_PASS       "...PASS..."


/************************* Your Milkcocoa Setup *********************************/

#define MILKCOCOA_APP_ID      "...YOUR_MILKCOCOA_APP_ID..."
#define MILKCOCOA_DATASTORE   "esp8266/tout"

/************* Milkcocoa Setup (you don't need to change this!) ******************/

#define MILKCOCOA_SERVERPORT  1883

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ "client1";

Milkcocoa *milkcocoa;

void setup() {
  Serial.begin(115200);
  delay(10);

  milkcocoa = new Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID, "sessionid");

  Serial.println(F("Milkcocoa SDK demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println( milkcocoa->on(MILKCOCOA_DATASTORE, "push", onpush) );
};

void loop() {
  milkcocoa->loop();
  DataElement a = DataElement();

  uint ADC_Value = 0;
  ADC_Value = system_adc_read();

  a.setValue("v", (int)ADC_Value);
  milkcocoa->push(MILKCOCOA_DATASTORE, a);
  delay(2000);
};

void onpush(DataElement elem) {
  Serial.println("onpush");
  Serial.println(elem.getInt("v"));
};