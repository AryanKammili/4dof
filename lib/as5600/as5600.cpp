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
        std::string name;

        int previousQuadrant;
        float rawPositionDegrees;
        float relativePositionDegrees;
        int numTurns;

    public:
        AS5600(float offsetAngle, float gearing, std::string name) {
            this->offsetAngle = offsetAngle;
            this->gearing = gearing;
            this->name = name;

            previousQuadrant = 0;
            rawPositionDegrees = 0;
            relativePositionDegrees = 0;
            numTurns = 0;

        }

        bool begin(int pSDA, int pSCL) {
            Wire.begin(pSDA, pSCL);
            Wire.setClock(400000L);

            while(!hasMagnet()) {
               // wait around
            }

            rawPositionDegrees = getPositionDegrees();
            relativePositionDegrees = getRelativePositionDegrees(rawPositionDegrees);

            return true;
        }

        void update() {
            rawPositionDegrees = getPositionDegrees();
            relativePositionDegrees = getRelativePositionDegrees(rawPositionDegrees);
        };

        // getter functions to be used outside of class //
        float getAbsolutePositionDegrees() {
            return rawPositionDegrees;
        }

        float getRelativePositionDegrees() {
            return relativePositionDegrees;
        }

        int getNumTurns() {
            return numTurns;
        }

        std::string getName() {
            return name;
        }

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
        Wire.write(Register::ANG_HIGH);
        Wire.endTransmission(false);
        Wire.requestFrom(Register::DATA, 2);

        highRead = Wire.read();
        lowRead = Wire.read();

        // Shift the high read up 8 so that it represents the original 12 bit info //
        highRead = highRead << 8;

        totalRead = lowRead | highRead;

        // 12 bits -> 2^12 ticks. Thus (/ticks) * 360.0 //
        degAngle = totalRead * (360.0 / std::pow(2, 12)) * 1;

        float correctedAngle = degAngle - offsetAngle;

        if(correctedAngle < 0)
            correctedAngle += 360;

        return correctedAngle;
    };

    int getQuadrant(float pAngleDegrees){
        int quadrantNum = 0;

        if(pAngleDegrees >= 0 && pAngleDegrees <= 90) quadrantNum = 1;
        else if(pAngleDegrees > 90 && pAngleDegrees <= 180) quadrantNum = 2;
        else if(pAngleDegrees > 180 && pAngleDegrees <= 270) quadrantNum = 3;
        else if(pAngleDegrees > 270 && pAngleDegrees < 360) quadrantNum = 4;
        else if(pAngleDegrees==360) quadrantNum = 1;

        return quadrantNum;
    }

    float getRelativePositionDegrees(float pAngle){

        int currentQuadrant = 0;

        // Assuming CW is + //
        currentQuadrant = getQuadrant(pAngle);

        if(currentQuadrant != previousQuadrant) {
            if(currentQuadrant == 1 && previousQuadrant == 4) numTurns++;
            if(currentQuadrant == 4 && previousQuadrant == 1) numTurns--;

            previousQuadrant = currentQuadrant;
        };

        pAngle = numTurns * 360 + pAngle;

        return pAngle;
    };

    public:

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
        


};