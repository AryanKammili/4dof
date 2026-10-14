
#include <cmath>
#include <string>
#include <esp32-hal.h>

class pid {

    private: 

        float kP;
        float kI;
        float kD;

        float kS;
        float kV;
        float kG;

        float setpoint;
        float currentError;
        float lastError;

        float lastTimeUpdateMs;

    public:
        pid(float p, float i, float d, float s, float v, float g) {
            this->kP = p;
            this->kI = i;
            this->kD = d;
            this->kS = s;
            this->kV = v;
            this->kG = g;

            currentError = 0.0;
            lastError = 0.0;
            lastTimeUpdateMs = millis();
        }

        // This loop will run in the main loop in main.cpp and will be used to update timestamps along with error tracking //
        void update() {
            float currentTimeMs = millis();
            float dt = currentTimeMs = lastTimeUpdateMs;


            lastTimeUpdateMs = currentTimeMs;

        }

        float calculate(float currentPosition, float setpoint, float dT) {
            lastError = currentError;
            currentError = setpoint - currentPosition; 

            float derivativeTerm = (currentError - lastError) / (dT);

            float output = (kP * currentError) + kI + (kD * derivativeTerm);

            return output;
        }

        void setPID(float p, float i, float d) {
            kP = p;
            kI = i;
            kD = d;
        }

        void setFF(float s, float v, float g) {
            kS = s;
            kV = v;
            kG = g;
        }
};