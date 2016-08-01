# Room Monitor
Particle Electron app that reads data from a BMP085 and a DHT11 sensor before sending them to the Thingspeak API.

I created this simple Room Monitor program to try out Particle's new product the Electron. The code is fairly basic just enough to have a working application to demo the Electron.

Two libraries are used `PietteTech_DHT.h` and `Adafruit_BMP085.h` to help communicate with the sensors.

## Electron & Sensor pins
D0 -> BMP085 SDA  
D1 -> BMP085 SCL  
D2 -> DHT11 data pin.  

Each sensor is connected to the 3V3 and GND pins for power. There is also a 10k Ohms resistor between the DHT11 data pin and the 3V3 rail.

## Thingspeak setup
I'm presuming you have created an account with Thingspeak.
* Create a new Channel, giving it a name and if you like a description.
* Now we need to give the fields 1-8 labels that correspond with the code:
  **Field 1** - BMP085 Temp, **Field 2** - DHT11 Temp, **Field 3** - Altitude, **Field 4** - Real Altitude, **Field 5** - Pressure, **Field 6** - Humidity, **Field 7** - Dew Point, **Field 8** - Dew Point Slow
*  The code also sends the location specified, if you want this to show on your channel you need to tick the enable location box. You can also optionally set the channel to be public.
*  Once you've set the settings as you desire, head over to the API Keys page and grab the Write API key, you'll need to pop this in the api key variable in `RoomMonitor.ino` e.g. `String api_key = "123456";` 

## Particle Dashboard Configuration
* From within your Particle Dashboard you need to head over to the Integrations page and create a new webhook called `RoomMonitorUpdate`. 
* Enter the following URL: `https://api.thingspeak.com/update?`
* Request type: `POST`
* Open the Advanced section and paste the following into the JSON textbox:
* `{
  "status": "{{s}}",
  "elevation": "{{e}}",
  "long": "{{o}}",
  "lat": "{{a}}",
  "field8": "{{8}}",
  "field7": "{{7}}",
  "field6": "{{6}}",
  "field5": "{{5}}",
  "field4": "{{4}}",
  "field3": "{{3}}",
  "field2": "{{2}}",
  "field1": "{{1}}",
  "api_key": "{{k}}"
}`
* Save the webhook.   

## Running the program
Once you have compiled the program and uploaded it to your Electron you should see your device connect to the mobile network, flash twice and then sleep for 900 seconds aka 15 minutes (or whatever interval you have changed the `sleepDuration` variable to). 
