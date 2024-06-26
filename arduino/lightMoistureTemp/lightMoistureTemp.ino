// Define sensor and output pins
const int lightSensorPin = A0;
const int moistureSensorPin = A1;
const int pumpPin = 2;
const int lightPin = 3;

// Define threshold values
const int moistureThreshold = 300; // Adjust this value as needed
const int lightThreshold = 400;    // Adjust this value as needed

// Define hysteresis range
const int hysteresisRange = 50; // Adjust this value as needed

// Variables to store sensor values
int moistureValue = 0;
int lightValue = 0;

// Variables to store the state of the pump and light
bool pumpState = false;
bool lightState = false;

// Timer variables for non-blocking delay
unsigned long previousMillis = 0;
const long interval = 1000; // Interval at which to print sensor values (1 second)

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set pump and light pins as outputs
  pinMode(pumpPin, OUTPUT);
  pinMode(lightPin, OUTPUT);

  // Ensure pump and light are initially off
  digitalWrite(pumpPin, LOW);
  digitalWrite(lightPin, LOW);
}

void loop() {
  // Update sensor values
  updateSensorValues();

  // Control pump and light based on sensor values
  controlPump();
  controlLight();

  // Print sensor values to serial monitor every 1 second
  printSensorValues();
}

// Function to update sensor values
void updateSensorValues() {
  moistureValue = analogRead(moistureSensorPin);
  lightValue = analogRead(lightSensorPin);
}

// Function to control the pump based on moisture value with hysteresis
void controlPump() {
  if (!pumpState && moistureValue < moistureThreshold - hysteresisRange) {
    pumpState = true;
    digitalWrite(pumpPin, HIGH); // Turn pump on
  } else if (pumpState && moistureValue > moistureThreshold + hysteresisRange) {
    pumpState = false;
    digitalWrite(pumpPin, LOW); // Turn pump off
  }
}

// Function to control the light based on light value with hysteresis
void controlLight() {
  if (!lightState && lightValue < lightThreshold - hysteresisRange) {
    lightState = true;
    digitalWrite(lightPin, HIGH); // Turn light on
  } else if (lightState && lightValue > lightThreshold + hysteresisRange) {
    lightState = false;
    digitalWrite(lightPin, LOW); // Turn light off
  }
}

// Function to print sensor values to the serial monitor every 1 second
void printSensorValues() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Print sensor values to serial monitor
    Serial.print("Moisture:");
    Serial.print(moistureValue);
    Serial.print(",Light:");
    Serial.println(lightValue);
  }
}

