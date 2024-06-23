#pragma once
#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <ArduinoJson.h>
#include <HTTPClient.h>

struct SensorConfig {
    bool isIrrigation;
    long cropFieldId;
};

const String authAndGetToken(const char *email = "adminSmartsprout@mail.com", const char *password = "admin13#01#2004");
SensorConfig manageConfigurationSensor(const char *token);

#endif // HTTP_CLIENT_HPP
