#ifndef IO_HPP
#define IO_HPP

#include "AdafruitIO_WiFi.h"

// Passwords file
#include <config.hpp>

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);


#endif // IO_HPP
