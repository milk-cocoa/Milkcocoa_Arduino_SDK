#include "ESP8266.h"
#include "Milkcocoa.h"
#include "Client_ESP8266.h"

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

// Create an ESP8266Client class to connect to the MQTT server.
ESP8266Client wifi;

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&wifi, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Milkcocoa SDK demo"));

  wifi.begin(Serial5, 115200);
    
  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());
    
  if (wifi.setOprToStation()) {
      Serial.print("to station ok\r\n");
  } else {
      Serial.print("to station err\r\n");
  }
    
  if (wifi.joinAP(WLAN_SSID, WLAN_PASS)) {
      Serial.print("Join AP success\r\n");
      Serial.print("IP: ");
      Serial.println(wifi.getLocalIP().c_str());    
  } else {
      Serial.print("Join AP failure\r\n");
  }
        
  if (wifi.disableMUX()) {
      Serial.print("single ok\r\n");
  } else {
      Serial.print("single err\r\n");
  }

  if(milkcocoa->on(MILKCOCOA_DATASTORE, "push", onpush)){
      Serial.println("milkcocoa on sucesss");   
  }
  else {
      Serial.println("milkcocoa on failure");   
  }
};



void loop() {
  milkcocoa.loop();

  uint ADC_Value = 0;
  ADC_Value = system_adc_read();

  DataElement elem = DataElement();
  elem.setValue("v", (int)ADC_Value);
  milkcocoa.push(MILKCOCOA_DATASTORE, &elem);

  delay(7000);
};

void onpush(DataElement *pelem) {
  Serial.println("onpush");
  Serial.println(pelem->jgetInt("v"));
};
