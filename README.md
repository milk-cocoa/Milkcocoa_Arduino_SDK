Milkcocoa Arduino SDK
=====

[Milkcocoa](https://mlkcca.com/) SDK for Arduino with ESP8266(use AT).

Works with Arduino M0 & M0 pro with ESP8266.

To use thie library, you also download and install ESP8266 library 
(https://github.com/exshonda/ESP8266_Arduino_AT)

## How To Use

Include libraries(`#include <Milkcocoa.h> and #include "ESP8266.h" and #include "Client_ESP8266.h"`), and write a code like below.

```
ESP8266Client wifi;
Milkcocoa milkcocoa = Milkcocoa(&wifi, "milkcocoa_app_id.mlkcca.com", 1883, "milkcocoa_app_id", "mqtt_client_id");

void setup() {
	//"Serial5" Serial port connected to ESP8266.
	wifi.begin(Serial5, 115200);

	//Set ESP8266 as Station mode.
	if (wifi.setOprToStation()) {
		Serial.print("to station ok\r\n");
	} else {
		Serial.print("to station err\r\n");
	}

	//Connect to Wifi AP
	if (wifi.joinAP(WLAN_SSID, WLAN_PASSWORD)) {
		Serial.print("Join AP success\r\n");
		Serial.print("IP: ");
		Serial.println(wifi.getLocalIP().c_str());    
	} else {
		Serial.print("Join AP failure\r\n");
	}
    
	//Set single mode
	if (wifi.disableMUX()) {
		Serial.print("single ok\r\n");
	} else {
		Serial.print("single err\r\n");
	}

	//"on" API was able to call in setup
	milkcocoa.on("milkcocoa_datastore_name", "push", onpush);
}

void loop() {
	//milkcocoa.loop must be called in loop()
	milkcocoa.loop();

	//push
	DataElement elem = DataElement();
	elem.setValue("name", "Milk");
	elem.setValue("age", 35);
	milkcocoa.push("milkcocoa_datastore_name", elem);

	delay(10000);
}

void onpush(DataElement *pelem) {
  Serial.println(pelem->getString("name"));
  Serial.println(pelem->getInt("age"));
  // Output:
  // Milk
  // 35
};
```

### Using API Key

If you use Milkcocoa API Key Authantication, please use `createWithApiKey`.

```
Milkcocoa *milkcocoa = Milkcocoa::createWithApiKey(&client, "milkcocoa_app_id.mlkcca.com", 1883, "milkcocoa_app_id", "mqtt_client_id", "API_KEY", "API_SECRET");
```

## Patch for Arduino library 1.7.8

Arduino library version 1.7.8 has memory leak bug at USB driver. To fix this bug, apply following path.


```
./hardware/arduino/samd/cores/arduino/USB/samd21_device.c : Line 152

for (i = 0; i < sizeof(usb_endpoint_table)/4; i++) {
    (*(uint32_t *)((uint32_t *)(&usb_endpoint_table[0])+i)) = 0;
}
```

## Examples

- [Simple ESP8266 Example](https://github.com/milk-cocoa/Milkcocoa_Arduino_SDK/blob/master/examples/milkcocoa_esp8266/milkcocoa_esp8266.ino): simple test of `push()` and `on("push")`.
- [ESP8266 TOUT Example](https://github.com/milk-cocoa/Milkcocoa_Arduino_SDK/blob/master/examples/milkcocoa_esp8266_tout/milkcocoa_esp8266_tout.ino): get sensor data from TOUT of ESP8266
- [ESP8266 API Key Authantication Example](https://github.com/milk-cocoa/Milkcocoa_Arduino_SDK/blob/master/examples/milkcocoa_esp8266_apikey_auth/milkcocoa_esp8266_apikey_auth.ino): auth with Milkcocoa API Key


## LICENSE

MIT



Copyright (c) 2015 Technical Rockstars.

Copyright (C) 2015 Embedded and Real-Time Systems Laboratory
              Graduate School of Information Science, Nagoya Univ., JAPAN

- Milkcocoa.h
- Milkcocoa.cpp

### Using

- https://github.com/adafruit/Adafruit_MQTT_Library
- https://github.com/interactive-matter/aJson

