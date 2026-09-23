#include <Arduino.h>
#include "Constants.h"
#include "as5600.cpp"
#include "nema17.cpp"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#define WIFI "ATTpTE3eee"
#define PASSWORD "t8y%qtvhxn8%"

#define SDA_PIN 4
#define SCL_PIN 5
#define DIR_PIN 6

AS5600 encoder(32.61, 4.0, Constants::J1_SDA_PIN, Constants::J1_SCL_PIN, Constants::J1_DIR_PIN, "J1_Encoder");
NEMA17 motor(
  "J1_Motor",
  Constants::J1_DRIVER_DIR, 
  Constants::J1_DRIVER_STEP, 
  Constants::J1_DRIVER_MS1, 
  Constants::J1_DRIVER_MS2, 
  Constants::J1_DRIVER_MS3, 
  Constants::J1_DRIVER_ENABLE, 
  Constants::J1_DRIVER_SLP);

void setup() {

  Serial.begin(115200);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(WIFI, PASSWORD);

  // while(WiFi.waitForConnectResult() != WL_CONNECTED) {
  //   Serial.println("Connection has failed Restarting");
  //   delay(5000);
  //   ESP.restart();
  // }
  // delay(1000);
  // ArduinoOTA.begin();

  Serial.println("Program Starting");
  
  if(motor.begin()) {
    Serial.println("J1_Encoder is ready");
  }

  motor.setMicroStep(NEMA17::FULL);
}

void loop() {
  // ArduinoOTA.handle();
  // encoder.update();

  // Serial.print(encoder.getRelativePositionDegrees());
  // Serial.print(",");
  // Serial.print(encoder.getAbsolutePositionDegrees());
  // Serial.print(",");
  // Serial.print(encoder.getRawAngleDegrees());
  // Serial.print(",");
  // Serial.print(encoder.getAGC());
  // Serial.print(",");
  // Serial.println(encoder.getMagnitude());

  motor.setVelocity(45);
  Serial.println("This is running");
  delay(1000);
  motor.setVelocity(-45);
  delay(1000);
}
