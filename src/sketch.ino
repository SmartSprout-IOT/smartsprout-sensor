#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "time.h"
#include "http-client.hpp"
#include "sensor-http-client.hpp"
#include "smartsprout-sensor.hpp"
#include "wifi-setup.hpp"
#include "end-irrigation-http-client.hpp"


#define irrigationLed 4
#define temperatureLed 15
#define humidityLed 2

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

String authToken;
SensorConfig sensorConfig;
IdealConditions idealConditions;

unsigned long previousTempMillis = 0;
unsigned long previousHumiMillis = 0;

float currentTemperature;
float currentHumidity;

void setup()
{
    pinMode(temperatureLed, OUTPUT);
    pinMode(irrigationLed, OUTPUT);
    pinMode(humidityLed, OUTPUT);

    Serial.println("Hello, ESP32!");

    Serial.begin(115200);

    LCD.init();
    LCD.backlight();
    LCD.setCursor(0, 0);
    LCD.print("Starting...");
    delay(500);

    // Initialize WiFi connection
    setupWiFi();
    // Authenticate and get the token
    authToken = authAndGetToken();
    // Initialize the DHT sensor
    setupSmartSproutSensor();
    delay(1000);
    configTime(-9000, -9000, "1.south-america.pool.ntp.org");

    LCD.clear();
}

void initialFunction(float temp, float humi)
{
    LCD.setCursor(0, 0);
    LCD.print("Actual Temp: ");
    LCD.print(temp);
    LCD.print("C");
    LCD.setCursor(0, 1);
    LCD.print("Actual Humi: ");
    LCD.print(humi);
    LCD.print("%");
    delay(2000);
}

void smartsproutFunction(float temp, float humi, SensorConfig sensorConfig)
{
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Irrigation: ");
    if (sensorConfig.isIrrigation)
    {
        LCD.print("True");
    }
    else
    {
        LCD.print("False");
    }
    delay(2000);
    LCD.clear();

    if (sensorConfig.isIrrigation)
    {
        idealConditions = getIdealConditions(authToken.c_str(), sensorConfig.cropFieldId);
        LCD.setCursor(0, 0);
        LCD.print("Ideal Conditions...");
        delay(2000);
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Ideal Temp: ");
        LCD.print(idealConditions.idealTemperature);
        LCD.print("C");
        LCD.setCursor(0, 1);
        LCD.print("Ideal Hum: ");
        LCD.print(idealConditions.idealHumidity);

        if (temp > idealConditions.idealTemperature)
        {
            digitalWrite(temperatureLed, HIGH);
        }
        else
        {
            digitalWrite(temperatureLed, LOW);
        }

        if (humi < idealConditions.idealHumidity)
        {
            digitalWrite(humidityLed, HIGH);
        }
        else
        {
            digitalWrite(humidityLed, LOW);
        }

        if (sensorConfig.isIrrigation)
        {
            digitalWrite(irrigationLed, HIGH);
        }
        else
        {
            digitalWrite(irrigationLed, LOW);
        }

    }
}

void endIrrigationFunction(float temp, float humi, SensorConfig sensorConfig)
{
    if (sensorConfig.isIrrigation)
    {
        if (temp <= idealConditions.idealTemperature && humi >= idealConditions.idealHumidity)
        {
            endIrrigation(authToken.c_str(), sensorConfig.cropFieldId);
        }
    }
}

void configurationFunction(const char *token)
{
    sensorConfig = manageConfigurationSensor(token);
    if (sensorConfig.cropFieldId != 0)
    {
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Irrigation Started");
        delay(2000);
    }
    else
    {
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("Irrigation Stopped");
        delay(2000);
    }
}

void loop()
{
    delay(5000);
    // Measure temperature and humidity
    float temperature = measureTemperature();
    float humidity = measureHumidity();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("ºC ");
    Serial.print("Humidity: ");
    Serial.println(humidity);
    initialFunction(temperature, humidity);
    configurationFunction(authToken.c_str());
    smartsproutFunction(temperature, humidity, sensorConfig);
    //endIrrigationFunction(temperature, humidity, sensorConfig);
    int httpResponseCode = sendSensorData(authToken.c_str(), sensorConfig.cropFieldId, {temperature, humidity, sensorConfig.isIrrigation});
    Serial.println(httpResponseCode);
}