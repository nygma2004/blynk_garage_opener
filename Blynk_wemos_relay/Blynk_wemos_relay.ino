/*************************************************************
This is a sketch based on Blynk demo sketches.

This example controls relays and uses a DHT temperature and humidity
sensor to provide update to the app.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "a86cb6dc91ca493fa36960cb98eaa2fe";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "VD";
char pass[] = "Daniviki";
int chargeState = LOW;
int chargeTime = 60*6; // charge times in minutes
int chargeCounter = 0; // counting down the charge time in seconds

#define DHTPIN 2          // Pin the DHT sensor is connected to
#define DHTTYPE DHT11     // DHT 11
#define CHARGEPIN 0       // Pin for the charger door relay
#define GARAGEPIN 4       // Pin for the garage door relay
#define GATEPIN 5         // Pin for the gate relay
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer sensor;         // Timer object for the DHT sensor
BlynkTimer charger;        // Timer object for the charger

// This function reads the DHT 11 sensor and saved the temparature and humdity reading to V5 and V6
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

// This function initiates, completed the charger relay, and counts down the charge time
void countCharger()
{
  if (chargeState) {
    // charger is running  
    chargeCounter--;
    Blynk.virtualWrite(V2, chargeCounter/60+1); // Update the counter to V2 to display the remaining time in the app (divide by 60 to show minutes)
    if (chargeCounter == 0) {
      // Counter has reached zero, the charge time is up
      Serial.println("Charging completed...");
      chargeState = LOW;                // Set the internal flag low
      digitalWrite(CHARGEPIN, HIGH);    // Turn off the relay  
      Blynk.virtualWrite(V1, LOW);      // Turn off the button state in the app
      Blynk.virtualWrite(V2, "-");      // Update the counter in the app to "-"
    }
  }
}

// This function gets called when the charger button is turned on/off in the app
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  if (pinValue) {
    // Charger was turned on in the app
    Serial.println("Charger starting...");
    chargeState = HIGH;
    digitalWrite(CHARGEPIN, LOW);               // Turn on the relay
    chargeCounter = chargeTime * 60;            // Set the internal counter value in second
    Blynk.virtualWrite(V2, chargeCounter/60);   // Update the remaining time in the app (divide by 60 to show minutes)
  } else {
    // Charger was turned off in the app
    Serial.println("Charging stopped...");
    chargeState = LOW;
    digitalWrite(CHARGEPIN, HIGH);              // Turn on the relay
    Blynk.virtualWrite(V2, "-");                // Update the counter in the app to "-"
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Serial.println("Booting up...");
  Serial.println("Setting initial pin states");
  // In the next few lines the initial states of the relay outputs are set
  pinMode(GARAGEPIN, OUTPUT);
  pinMode(GATEPIN, OUTPUT);
  pinMode(CHARGEPIN, OUTPUT);
  digitalWrite(GARAGEPIN, HIGH);
  digitalWrite(GATEPIN, HIGH);
  digitalWrite(CHARGEPIN, HIGH);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8441);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

  dht.begin();

  // Setup up the callback functions. DHT read every 2 seconds, charger countdown ever second.
  sensor.setInterval(2000L, sendSensor);
  charger.setInterval(1000L, countCharger);
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  // Kick off the timers
  sensor.run();
  charger.run();
}
