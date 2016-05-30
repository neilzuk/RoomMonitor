// Include required libraries.
#include "Adafruit_BMP085.h"
#include "PietteTech_DHT.h"

/*
	Room Monitor - Neil Hawker
	Uses a BMP085 and DHT11, serializes into JSON and sends via Particle Cloud to ThingSpeak.
	Adafruit & PietteTech libraries used for comms with sensors
*/

Adafruit_BMP085 bmp;
#define DHTTYPE  DHT11       // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   D2           // Digital pin for communications
void dht_wrapper(); // must be declared before the lib initialization
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

String api_key = "UO2O1XN9DKC9INK9"; // ThingSpeak Write API Key.
String field1 = ""; // BMP085 Temp *c
String field2 = ""; // DHT11 Temp *c
String field3 = ""; // Altitude m
String field4 = ""; // Real Alt m
String field5 = ""; // Pressure pa
String field6 = ""; // Humidity %
String field7 = ""; // Dew Point *c
String field8 = ""; // Dew Point Slow *c
String lat = "53.2819";
String lon = "2.8962";
String el = "19";
String status = "";
int sleepDuration = 900; // Sleep for 15 minutes

// Initialize BMP085

void InitializeBMP085(){
	if (!bmp.begin()) {
		//Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	}
}

// Publish Pressure, Altitude
void PopulateBMP085Info(){
		field1 = String(bmp.readTemperature());
	  field5 = String((bmp.readPressure()/1000));
		field3 = String(bmp.readAltitude());
		field4 = String(bmp.readAltitude(101500));
}

void PopulateDHT() {
	int result = DHT.acquireAndWait();
	if(result == DHTLIB_OK) {
		field6 = String(DHT.getHumidity());
		field2 = String(DHT.getCelsius());
		field7 = String(DHT.getDewPoint());
		field8 = String(DHT.getDewPointSlow());
	}
}

void BlinkLED(){
    digitalWrite(D7, HIGH);
    delay(500);
    digitalWrite(D7, LOW);
    delay(500);
}

void setup() {
		pinMode(D7, OUTPUT);
	  InitializeBMP085();
}

void dht_wrapper() {
    DHT.isrCallback();
}

void loop() {
    PopulateBMP085Info();
    PopulateDHT();

		String json;
		createTSjson(json);
		Particle.publish("RoomMonitorUpdate",json,60,PRIVATE);

		ResetValues();
		BlinkLED();

	  delay(1000);
		// Send the system to sleep for a duration to save battery & data.
		System.sleep(SLEEP_MODE_DEEP,sleepDuration);
}

void createTSjson(String &dest)
{
    dest = "{";

    if(field1.length()>0){
        dest = dest + "\"1\":\""+ field1 +"\",";
    }

    if(field2.length()>0){
        dest = dest + "\"2\":\""+ field2 +"\",";
    }

    if(field3.length()>0){
        dest = dest + "\"3\":\""+ field3 +"\",";
    }

    if(field4.length()>0){
        dest = dest + "\"4\":\""+ field4 +"\",";
    }

    if(field5.length()>0){
        dest = dest + "\"5\":\""+ field5 +"\",";
    }

    if(field6.length()>0){
        dest = dest + "\"6\":\""+ field6 +"\",";
    }

    if(field7.length()>0){
        dest = dest + "\"7\":\""+ field7 +"\",";
    }

    if(field8.length()>0){
        dest = dest + "\"8\":\""+ field8 +"\",";
    }

    if(lat.length()>0){
        dest = dest + "\"a\":\""+ lat +"\",";
    }

    if(lon.length()>0){
        dest = dest + "\"o\":\""+ lon +"\",";
    }

    if(el.length()>0){
        dest = dest + "\"e\":\""+ el +"\",";
    }

    if(status.length()>0){
        dest = dest + "\"s\":\""+ status +"\",";
    }

    dest = dest + "\"k\":\"" + api_key + "\"}";
}

void ResetValues() {
	 field1 = ""; // BMP085 Temp *c
	 field2 = ""; // DHT11 Temp *c
	 field3 = ""; // Altitude m
	 field4 = ""; // Real Alt m
	 field5 = ""; // Pressure pa
	 field6 = ""; // Humidity %
	 field7 = ""; // Dew Point *c
	 field8 = ""; // Dew Point Slow *c
}
