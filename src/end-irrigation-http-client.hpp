#pragma once
#ifndef END_IRRIGATION_HTTP_CLIENT_HPP
#define END_IRRIGATION_HTTP_CLIENT_HPP

#include <ArduinoJson.h>
#include <HTTPClient.h>

bool endIrrigation(const char *token, long cropFieldId);

#endif // END_IRRIGATION_HTTP_CLIENT_HPP