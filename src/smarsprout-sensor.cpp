#include "smartsprout-sensor.hpp"
#include <DHT.h>

const int DHT_PIN = 13;

DHT dht(DHT_PIN, DHT22);

void setupSmartSproutSensor() {
  dht.begin();
}

float measureTemperature() {
    float temperature = dht.readTemperature();
    if(isnan(temperature)) {
        Serial.println("Failed to read temperature from DHT sensor!");
    }
    return temperature;
}

float measureHumidity() {
    float humidity = dht.readHumidity();
    if(isnan(humidity)) {
        Serial.println("Failed to read humidity from DHT sensor!");
    }
    return humidity;
}