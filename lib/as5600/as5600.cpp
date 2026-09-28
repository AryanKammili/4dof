#include <Wire.h>
#include "Register.h"
#include <cmath>
#include <string>


//TODO  : Figure out inverse logic //
//TODO  : Figure out gearing logic //
//TODO  : Add active warning setup //
class AS5600 {

    private:
        float offsetAngle;
        float gearing;
        bool invert;
        std::string name;

        int SDA_PIN;
        int SCL_PIN;
        int DIR_PIN;

        int previousAngle;
        float rawPositionDegrees;
        float relativePositionDegrees;
        float velocityDegreesPerSecond;
        int numTurns;
        uint32_t lastUpdateMs;

        bool CCWPositive = false;

    public:
        AS5600(float offsetAngle, float gearing, bool invert, int SDA_PIN, int SCL_PIN, int DIR_PIN, std::string name) {
            this->offsetAngle = offsetAngle;
            this->gearing = gearing;
            this->invert = invert;
            this->name = name;

            this->SDA_PIN = SDA_PIN;
            this->SCL_PIN = SCL_PIN;
            this->DIR_PIN = DIR_PIN;

            previousAngle = 0;
            rawPositionDegrees = 0;
            relativePositionDegrees = 0;
            velocityDegreesPerSecond = 0;
            numTurns = 0;
            lastUpdateMs = 0;

        }

        bool begin() {
            Wire.begin(SDA_PIN, SCL_PIN);
            Wire.setClock(400000L);

            while(!hasMagnet()) {
               // wait around
            }

            rawPositionDegrees = getPositionDegrees();
            previousAngle = rawPositionDegrees;
            relativePositionDegrees = getRelativePositionDegrees(rawPositionDegrees);
            lastUpdateMs = millis();

            return true;
        }

        void update() {
            const float previousRelativePosition = relativePositionDegrees;
            const uint32_t now = millis();
            rawPositionDegrees = getPositionDegrees();
            relativePositionDegrees = getRelativePositionDegrees(rawPositionDegrees);

            // Calculation for Velocity is going to happen just within the loop //
            const uint32_t elapsedMs = now - lastUpdateMs;
            if (elapsedMs > 0) {
                velocityDegreesPerSecond =
                    ((relativePositionDegrees - previousRelativePosition) * 1000.0f / elapsedMs);
            }
            lastUpdateMs = now;
        };

        // getter functions to be used outside of class //
        float getAbsolutePositionDegrees() {
            return rawPositionDegrees;
        }

        float getRelativePositionDegrees() {
            return relativePositionDegrees;
        }

        float getVelocityDegreesPerSecond() {
            return velocityDegreesPerSecond;
        }

        int getNumTurns() {
            return numTurns;
        }

        std::string getName() {
            return name;
        }

        bool hasMagnet(){
            int statusRead = 0;

            // Grab 1 byte or 8 bits form the register //
            Wire.beginTransmission(Register::DATA);
            Wire.write(Register::STATUS);
            Wire.endTransmission();
            Wire.requestFrom(Register::DATA, 1);

            // returns num of bytes available to read, if 0 then we need to wait for data //
            while (Wire.available() == 0);
            statusRead = Wire.read();

            // Register returns the status as 0, 0, MD, ML, MH, 0, 0, 0 //
            // MD is at bit 5, thus 2^5 = 32. &32 zeros the rest of the bits //
            return (statusRead & 32) == 32;
        };

        float getAGC() {
            uint8_t agc = 0;
            Wire.beginTransmission(Register::DATA);
            Wire.write(Register::AGC);
            Wire.endTransmission(false);
            Wire.requestFrom(Register::DATA, 1);
            agc = Wire.read();
            return agc;
        };

        float getMagnitude() {
            int lowRead = 0; 
            int highRead = 0;

            Wire.beginTransmission(Register::DATA);
            Wire.write(Register::MAG_HIGH);
            Wire.endTransmission(false);
            Wire.requestFrom(Register::DATA, 2);

            highRead = Wire.read();
            lowRead = Wire.read();

            return (highRead << 8) | lowRead;
        };
        
        float getRawAngleDegrees() {
            int lowRead = 0;
            int highRead = 0;
            float totalRead = 0;

            Wire.beginTransmission(Register::DATA);
            Wire.write(Register::RAW_ANG_HIGH);
            Wire.endTransmission(false);
            Wire.requestFrom(Register::DATA, 2);

            highRead = Wire.read();
            lowRead = Wire.read();

            highRead = highRead << 8;
            totalRead = lowRead | highRead;

            return totalRead * (360.0 / std::pow(2, 12));
        };

        void debug() {
            std::cout << "======Encoder " << name << " debug string ======" << std::endl;
            std::cout << "Relative Position: " << relativePositionDegrees << std::endl;
            std::cout << "Absolute Position: " << rawPositionDegrees << std::endl;  
        }

    float getPositionDegrees(){
        int lowRead = 0;
        int highRead = 0;
        float totalRead = 0;
        float degAngle = 0;

        Wire.beginTransmission(Register::DATA);
        Wire.write(Register::RAW_ANG_HIGH);
        Wire.endTransmission(false);
        Wire.requestFrom(Register::DATA, 2);

        highRead = Wire.read();
        lowRead = Wire.read();

        // Shift the high read up 8 so that it represents the original 12 bit info //
        highRead = highRead << 8;

        totalRead = lowRead | highRead;

        // 12 bits -> 2^12 ticks. Thus (/ticks) * 360.0 //
        degAngle = totalRead * (360.0 / std::pow(2, 12));

        float correctedAngle = degAngle - offsetAngle;

        if(correctedAngle < 0)
            correctedAngle += 360;

        return correctedAngle;
    };

    float getRelativePositionDegrees(float pAngle){
        float angleDelta = pAngle - previousAngle;

        const float wrapThreshold = 300.0;   // near-360 jump = real wrap
        const float noiseFloor = 180.0;      // above this but below wrap = suspect read

        if (angleDelta >= wrapThreshold) {
            numTurns--;                       // wrapped backward through 0
        } 
        
        else if (angleDelta <= -wrapThreshold) {
            numTurns++;                       // wrapped forward through 360
        } 
        
        else if (fabs(angleDelta) > noiseFloor) {
            return relativePositionDegrees;
        }

        previousAngle = pAngle;

        return numTurns * 360 + pAngle;
    };

};
