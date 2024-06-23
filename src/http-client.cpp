#include "http-client.hpp"
#include <ArduinoJson.h>
#include <HTTPClient.h>

// Local
const char *authServerAddress = "http://192.168.88.4:8080/api/smartsprout/v1/auth/login";
const char *sensorManagementServerAddress = "http://192.168.88.4:8080/api/smartsprout/v1/iot-management/manage-configuration";

const String authAndGetToken(const char *email, const char *password)
{
    Serial.println("Authenticating...");
    HTTPClient client;

    // Create HTTP Client
    client.setTimeout(10000);
    client.begin(authServerAddress);
    client.addHeader("Content-Type", "application/json");

    // Prepare authentication request body
    String requestBody = "{\"userEmail\":\"" + String(email) + "\",\"userPassword\":\"" + String(password) + "\"}";

    // Send authentication request
    int httpResponseCode = client.POST(requestBody);
    String token = "";

    if (httpResponseCode == 200)
    {
        // Parse the JSON response to extract the token
        String response = client.getString();
        const size_t capacity = JSON_OBJECT_SIZE(10) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);
        token = doc["access_token"].as<String>();
        client.end();
        Serial.println("Authentication successful!");
        return token;
    }
    else
    {
        Serial.print("Authentication Error. HTTP response code:");
        Serial.println(httpResponseCode);
        client.end();
        Serial.println("An error occurred during authentication");
        return "";
    }
}

SensorConfig manageConfigurationSensor(const char *token)
{
    Serial.println("Managing sensor configuration...");
    HTTPClient client;

    // Create HTTP Client
    client.setTimeout(10000);
    client.begin(sensorManagementServerAddress);
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Authorization", "Bearer " + String(token));

    // Send sensor configuration request
    int httpResponseCode = client.GET();
    SensorConfig sensorConfig;

    if (httpResponseCode == 200)
    {
        // Parse the JSON response to extract the sensor configuration
        String response = client.getString();
        const size_t capacity = JSON_OBJECT_SIZE(10) + 1024;
        DynamicJsonDocument doc(capacity);
        deserializeJson(doc, response);

        sensorConfig.isIrrigation = doc["irrigation"].as<bool>();
        sensorConfig.cropFieldId = doc["cropFieldId"].as<long>();
        client.end();
        Serial.println("Sensor configuration managed successfully!");
        return sensorConfig;
    }
    else
    {
        Serial.print("Sensor Configuration Error. HTTP response code:");
        Serial.println(httpResponseCode);
        client.end();
        Serial.println("An error occurred during sensor configuration");
        return sensorConfig;
    }
}
