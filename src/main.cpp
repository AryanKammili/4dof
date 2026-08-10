#include <Arduino.h>
#include "Constants.h"
#include "as5600.cpp"

#define SDA_PIN 4
#define SCL_PIN 5

AS5600 encoder(0, "J1_Encoder");

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("Program Starting");

  
  if(encoder.begin(Constants::J1_SDA_PIN, Constants::J1_SCL_PIN)) {
    Serial.println("J1_Encoder is ready");
  }
}

void loop() {
  encoder.update();
  encoder.debug();
  delay(1000);
}
