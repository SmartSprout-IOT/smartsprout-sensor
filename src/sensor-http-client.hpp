#pragma once
#ifndef SENSOR_HTTP_CLIENT_HPP
#define SENSOR_HTTP_CLIENT_HPP

#include <ArduinoJson.h>
#include <HTTPClient.h>

struct IdealConditions {
    float idealTemperature;
    float idealHumidity;
};

struct IrrigationSuggestion {
    double actualTemperature;
    double actualHumidity;
    bool isIrrigation;
};

IdealConditions getIdealConditions(const char *token, long cropFieldId);
int sendSensorData(const char *token, long cropFieldId, IrrigationSuggestion irrigationSuggestion);

#endif // HTTP_CLIENT_HPP
