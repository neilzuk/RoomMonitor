// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_BMP085.h"
#include "PietteTech_DHT.h"

/*
	Wiring
	------
	BMP085 Vcc to 3.3V
	BMP085 GND to GND
	BMP085 SCL to D1
	BMP085 SDA to D0
*/

Adafruit_BMP085 bmp;
#define DHTTYPE  DHT11       // Sensor type DHT11/21/22/AM2301/AM2302
#define DHTPIN   D2           // Digital pin for communications
void dht_wrapper(); // must be declared before the lib initialization
PietteTech_DHT DHT(DHTPIN, DHTTYPE, dht_wrapper);

String api_key = "UO2O1XN9DKC9INK9"; // Replace this string with a valid ThingSpeak Write API Key.
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
int sleepDuration = 900;

// Initialize BMP085

void InitializeBMP085(){
	if (!bmp.begin()) {
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	}
}

// Publish Pressure, Altitude
void PopulateBMP085Info(){
		field1 = String(bmp.readTemperature());
	  field5 = String((bmp.readPressure()/1000));
		field3 = String(bmp.readAltitude());
		field4 = String(bmp.readAltitude(101500));
    /*Serial.print("Temperature = ");
    Serial.print(String(bmp.readTemperature()));
    Serial.println(" *C");*/

    /*Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");*/

    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    /*Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");*/

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    /*Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");*/

    //char szEventInfo[64];

    //sprintf(szEventInfo, "Temperature=%.2f °C, Pressure=%.2f hPa", bmp.readTemperature(), bmp.readPressure()/100.0);
		//Serial.println(szEventInfo);
    //Spark.publish("bmpo85info", szEventInfo);
}

void PopulateDHT() {
	int result = DHT.acquireAndWait();
	if(result == DHTLIB_OK) {
		field6 = String(DHT.getHumidity());
		field2 = String(DHT.getCelsius());
		field7 = String(DHT.getDewPoint());
		field8 = String(DHT.getDewPointSlow());
	}
    /*switch (result) {
        case DHTLIB_OK:
            Serial.println("OK");
            break;
        case DHTLIB_ERROR_CHECKSUM:
            Serial.println("Error\n\r\tChecksum error");
            break;
        case DHTLIB_ERROR_ISR_TIMEOUT:
            Serial.println("Error\n\r\tISR time out error");
            break;
        case DHTLIB_ERROR_RESPONSE_TIMEOUT:
            Serial.println("Error\n\r\tResponse time out error");
            break;
        case DHTLIB_ERROR_DATA_TIMEOUT:
            Serial.println("Error\n\r\tData time out error");
            break;
        case DHTLIB_ERROR_ACQUIRING:
            Serial.println("Error\n\r\tAcquiring");
            break;
        case DHTLIB_ERROR_DELTA:
            Serial.println("Error\n\r\tDelta time to small");
            break;
        case DHTLIB_ERROR_NOTSTARTED:
            Serial.println("Error\n\r\tNot started");
            break;
        default:
            Serial.println("Unknown error");
            break;
    }*/

    /*Serial.print("Humidity (%): ");
    Serial.println(DHT.getHumidity(), 2);

    Serial.print("Temperature (oC): ");
    Serial.println(DHT.getCelsius(), 2);

    Serial.print("Temperature (oF): ");
    Serial.println(DHT.getFahrenheit(), 2);

    Serial.print("Temperature (K): ");
    Serial.println(DHT.getKelvin(), 2);

    Serial.print("Dew Point (oC): ");
    Serial.println(DHT.getDewPoint());

    Serial.print("Dew Point Slow (oC): ");
    Serial.println(DHT.getDewPointSlow());*/
}

// Initialize applicaiton
void InitializeApplication(){
    //Serial.begin(9600);
		pinMode(D7, OUTPUT);
}

// Blink LED and wait for some time
void BlinkLED(){
    digitalWrite(D7, HIGH);
    delay(500);
    digitalWrite(D7, LOW);
    delay(500);
}

void setup() {
    InitializeApplication();
	  InitializeBMP085();
		/*while (!Serial.available()) {
      	Serial.println("Press any key to start. v3");
	      delay (1000);
	  }*/
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
		System.sleep(SLEEP_MODE_DEEP,sleepDuration);
}

void createTSjson(String &dest)
{
  // dest = "{ \"k\":\"" + api_key + "\", \"1\":\""+ field1 +"\", \"2\":\""+ field2 +"\",\"3\":\""+ field3 +"\",\"4\":\""+ field4 +"\",\"5\":\""+ field5 +"\",\"6\":\""+ field6 +"\",\"7\":\""+ field7 +"\",\"8\":\""+ field8 +"\",\"a\":\""+ lat +"\",\"o\":\""+ lon +"\",\"e\":\""+ el +"\", \"s\":\""+ status +"\"}";

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
