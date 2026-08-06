#include <Arduino.h>
#include "as5600.cpp"

#define SDA_PIN 4
#define SCL_PIN 5

AS5600 encoder(164.004, "J1_Encoder");

float j1RelativePos = 0.0;
float j1AbsolutePos = 0.0;

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("Program Starting");
  if(encoder.begin(SDA_PIN, SCL_PIN)) {
    Serial.println("J1_Encoder is ready");
  }
}

void loop() {
  encoder.update();
  j1RelativePos = encoder.getRelativePositionDegrees();
  j1AbsolutePos = encoder.getAbsolutePositionDegrees();

  std::cout << "Relative Position: " << j1RelativePos << std::endl;
  std::cout << "Absolute Position: " << j1AbsolutePos << std::endl;  
  delay(1000);
}
