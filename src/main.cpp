#include <Adafruit_SGP40.h>
#include <Adafruit_GPS.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <io.hpp> 
// Include your custom library for Adafruit IO and WiFi


// Create instances for the sensors
Adafruit_SGP40 SGP40;
Adafruit_GPS GPS{&Wire};

// Adafruit IO Feeds
AdafruitIO_Feed *aqiFeed = io.feed("aqi");
AdafruitIO_Feed *latFeed = io.feed("latitude");
AdafruitIO_Feed *lonFeed = io.feed("longitude");
AdafruitIO_Feed *healthFeed = io.feed("health_advice");

// User's age (replace with actual input mechanism)
int userAge = 25; 
bool respiratory_issue = false;

//Checks AQi levels and age ranges to provide appropriate health advice 
String getHealthAdvice(int aqi, int age) {
    if (aqi >= 301) {
        return "Extreme risk of severe respiratory and cardiovascular issues";
    } 
    
    else if (aqi >= 201) {
        return "High risk of severe respiratory and cardiovascular issues";
    } 
    
    else if (aqi >= 151) {
        return "Severe respiratory symptoms; cardiovascular strain evident";} 
    
    else if (aqi >= 101) {
        if (age <= 40) {
            return "Increased risk of respiratory issues; asthma exacerbation likely";} 
        
        else if (age <= 60) {
            return "Moderate respiratory issues; possible cardiovascular strain";} 
        
        else {
            return "Moderate respiratory issues; cardiovascular strain likely";}} 
    
    else if (aqi >= 51) {
        if (age <= 60) {
            return "Mild respiratory irritation; increased symptoms in sensitive individuals";}

        else {
            return "Mild respiratory irritation; possible exacerbation of pre-existing conditions";}}

    else {
        return "Air quality is good";
    }
}

void setup() {
    Serial.begin(115200);
    // Initialize the sensors
    while (!SGP40.begin()) {
        Serial.println("Failed to find SGP40 sensor!");
        while (1);
        delay(100);
    }
    
    while (!GPS.begin(0x10)) {
        Serial.println("Failed to find GPS sensor!");
        while (1);
        delay(100);
    }

    // Initialize WiFi and Adafruit IO
    io.connect();

    // Wait for a connection
    while (io.status() < AIO_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("Adafruit IO Connected!");
}

void loop() {
    // io.run() is required for all sketches.
    // It should always be present at the top of your loop function.
    io.run();

    // Get AQI from SGP40 sensor
    uint16_t air_comp = SGP40.measureRaw();
    int aqi = air_comp; // Placeholder conversion, replace with actual conversion logic if available

    // Get GPS data
    float latitude = GPS.latitude;
    float longitude = GPS.longitude;

    // Get health advice
    String healthAdvice = getHealthAdvice(aqi, userAge);

    // Publish data to Adafruit IO
    aqiFeed->save(aqi);
    latFeed->save(latitude);
    lonFeed->save(longitude);
    healthFeed->save(healthAdvice);

    // Print data to Serial
    Serial.print("AQI: ");
    Serial.println(aqi);
    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);
    Serial.print("Health Advice: ");
    Serial.println(healthAdvice);

    // Delay before next reading
    delay(60000); // Delay for 1 minute (adjust as needed)
}
