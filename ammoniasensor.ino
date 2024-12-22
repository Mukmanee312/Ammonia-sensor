#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN D5
#define DHTTYPE DHT22
#define Soilpin A0
#define water_pump LED_BUILTIN
#define Fan D2

// Global Variables
float Temp, Humi;
int soil_value, percent;

DHT dht(DHTPIN, DHTTYPE);

// Timing Variables
unsigned long previousMillis = 0;
const long interval = 3000; // 3 seconds

void setup() {
  Serial.begin(9600);

  pinMode(DHTPIN, INPUT);
  pinMode(Soilpin, INPUT);
  pinMode(water_pump, OUTPUT);
  pinMode(Fan, OUTPUT);

  dht.begin();
  Serial.println("System Initialized.");
}

void loop() {
  unsigned long currentMillis = millis();

  // Run every interval
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  
    readSensors();
    controlWaterPump();
    controlFan();
    Serial.println(); // Add spacing for readability
  }
}

void readSensors() {
  // Read Temperature and Humidity
  Temp = dht.readTemperature();
  Humi = dht.readHumidity();

  // Read Soil Moisture
  soil_value = analogRead(Soilpin);
  percent = map(soil_value, 0, 1023, 100, 0);

  // Check for Sensor Errors
  if (isnan(Temp) || isnan(Humi)) {
    Serial.println("Error: Failed to read from sensors. Retrying...");
    return;
  }

  // Display Sensor Data
  Serial.printf("Temperature: %.2f °C\tHumidity: %.2f %%\n", Temp, Humi);
  Serial.printf("Soil Value: %d\tSoil Moisture: %d %%\n", soil_value, percent);
}

void controlWaterPump() {
  if (percent < 45) {
    Serial.println("ความชื้นในดินต่ำ. Activating water pump.");
    digitalWrite(water_pump, LOW); // Active LOW
  } else {
    Serial.println("ความชื้นในดินอยู่ในช่วงปรกติ. Deactivating water pump.");
    digitalWrite(water_pump, HIGH); // Active LOW
  }
}

void controlFan() {
  if (Temp >= 32) {
    Serial.println("อุณภูมิสูงเกินไป. Activating fan.");
    digitalWrite(Fan, HIGH);
  } else {
    Serial.println("อุณหภูมิอยู่ในระดับปรกติ. Deactivating fan.");
    digitalWrite(Fan, LOW);
  }
}
