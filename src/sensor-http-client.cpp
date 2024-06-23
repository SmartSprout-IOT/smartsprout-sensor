#include "sensor-http-client.hpp"
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Local
const char *idealConditionsServerAddress = "http://192.168.88.4:8080/api/smartsprout/v1/iot-management/ideal-conditions";
const char *sensorDataServerAddress = "http://192.168.88.4:8080/api/smartsprout/v1/iot-management/irrigation-suggestion";

IdealConditions getIdealConditions(const char *token, long cropFieldId)
{
    Serial.println("Getting ideal conditions...");
    HTTPClient client;

    // Construct the URL with the cropFieldId as a path variable
    String url = String(idealConditionsServerAddress) + "/" + String(cropFieldId);

    // Create HTTP Client
    client.setTimeout(10000);
    client.begin(url);
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Authorization", "Bearer " + String(token));

    // Send ideal conditions request
    int httpResponseCode = client.GET();
    IdealConditions idealConditions;

    if (httpResponseCode == 200)
    {
        // Parse the JSON response to extract the ideal conditions
        String response = client.getString();
        const size_t capacity = JSON_OBJECT_SIZE(10) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);

        idealConditions.idealTemperature = doc["idealTemperature"].as<float>();
        idealConditions.idealHumidity = doc["idealHumidity"].as<float>();
        client.end();
        Serial.println("Ideal conditions retrieved successfully!");
        return idealConditions;
    }
    else
    {
        Serial.print("Ideal Conditions Error. HTTP response code:");
        Serial.println(httpResponseCode);
        client.end();
        Serial.println("An error occurred during ideal conditions retrieval");
        return idealConditions;
    }
}

int sendSensorData(const char *token, long cropFieldId, IrrigationSuggestion irrigationSuggestion)
{
    Serial.println("Sending sensor data...");
    HTTPClient client;

    // Create HTTP Client
    client.setTimeout(10000);
    client.begin(sensorDataServerAddress);
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Authorization", "Bearer " + String(token));

    String isIrrigation = "";

    if (irrigationSuggestion.isIrrigation)
    {
        isIrrigation = "true";
    }
    else
    {
        isIrrigation = "false";
    }
    

    // Prepare sensor data request body
    String requestBody = "{\"cropFieldId\":" + String(cropFieldId) + ",\"actualTemperature\":" + String(irrigationSuggestion.actualTemperature) + ",\"actualHumidity\":" + String(irrigationSuggestion.actualHumidity) + ",\"irrigation\":" + isIrrigation + "}";
    // Send sensor data request
    int httpResponseCode = client.PUT(requestBody);

    if (httpResponseCode == 200)
    {
        client.end();
        Serial.println("Sensor data sent successfully!");
        return httpResponseCode;
    }
    else
    {
        Serial.print("Sensor Data Error. HTTP response code:");
        Serial.println(httpResponseCode);
        client.end();
        Serial.println("An error occurred during sensor data sending");
        return httpResponseCode;
    }
}