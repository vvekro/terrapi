#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Pin configuration
#define DHTPIN 2     // Pin connected to the DHT sensor
#define SOIL_MOISTURE_PIN A0 // Pin connected to the soil moisture sensor

// Uncomment the type of DHT sensor in use
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22   // DHT 22 (AM2302), AM2321

// SENSOR INSTANCES
BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 Advanced begin");
  } else {
    Serial.println("Error initializing BH1750");
  }

  dht.begin();
}

void loop() {
  // light level in lux
  float lux = lightMeter.readLightLevel();

  // soil moisture
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);

  // temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  while (isnan(temperature)) {
    float temperature = dht.readTemperature();
  }
  while (isnan(humidity)) {
    float humidity = dht.readHumidity();
  }

  // Send via the serial port
  Serial.print("Light:");
  Serial.print(lux); // in lx
  Serial.print(",");

  Serial.print("Temperature:");
  Serial.print(temperature); // in *C
  Serial.print(",");

  Serial.print("Humidity:");
  Serial.print(humidity); // in %
  Serial.print(",");

  Serial.print("Soil Moisture:");
  Serial.println(soilMoistureValue); // an analog value

  // Delay between readings
  delay(2000);
}
