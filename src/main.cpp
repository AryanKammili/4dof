#include "plotter.h"
#include "as5600.cpp"
#include "nema17.cpp"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Constants.h"

#define WIFI "ATTpTE3eee"
#define PASSWORD "t8y%qtvhxn8%"

AS5600 encoderJ1(
  Constants::J1_ENCODER_OFFSET,
  Constants::J1_ENCODER_GEARING,
  Constants::J1_ENCODER_INVERT,
  Constants::J1_ENCODER_SDA,
  Constants::J1_ENCODER_SCL,
  Constants::J1_ENCODER_DIR,
  "J1_Encoder");

Plotter plotter(Serial, 20);

float j1PositionDeg = 0;
float j1VelocityDegPerSec = 0;
float j1RawAngleDeg = 0;
float j1Turns = 0;

NEMA17 motorJ1(
  "J1_Motor",
  Constants::J1_DRIVER_DIR,
  Constants::J1_DRIVER_STEP);

void setup() {
  Serial.begin(115200);
  Serial.println("Program Starting");

  if (encoderJ1.begin()) {
    Serial.println("J1_Encoder is ready");
  }

  if (motorJ1.begin()) {
    Serial.println("J1_Motor is ready");
  }

  plotter.subscribe("j1_position_deg", j1PositionDeg);
  plotter.subscribe("j1_velocity_deg_s", j1VelocityDegPerSec);
  plotter.subscribe("j1_raw_angle_deg", j1RawAngleDeg);
  plotter.subscribe("j1_turns", j1Turns);
}

void loop() {
  encoderJ1.update();
  j1PositionDeg = encoderJ1.getAbsolutePositionDegrees();
  j1VelocityDegPerSec = encoderJ1.getVelocityDegreesPerSecond();
  plotter.update();
}


