#include "end-irrigation-http-client.hpp"
#include <ArduinoJson.h>
#include <HTTPClient.h>

//Local
const char *endIrrigationServerAddress = "http://192.168.88.4:8080/api/smartsprout/v1/iot-management/irrigation-complete";

bool endIrrigation(const char *token, long cropFieldId)
{
    Serial.println("Ending irrigation...");
    HTTPClient client;

    // Construct the URL with the cropFieldId as a path variable
    String url = String(endIrrigationServerAddress) + "/" + String(cropFieldId);

    // Create HTTP Client
    client.setTimeout(10000);
    client.begin(url);
    client.addHeader("Content-Type", "application/json");
    client.addHeader("Authorization", "Bearer " + String(token));

    // Send end irrigation request
    int httpResponseCode = client.POST("{}");

    if (httpResponseCode == 200)
    {
        client.end();
        Serial.println("Irrigation ended successfully!");
        return true;
    }
    else
    {
        Serial.print("End Irrigation Error. HTTP response code:");
        Serial.println(httpResponseCode);
        client.end();
        Serial.println("An error occurred during irrigation ending");
        return false;
    }
}