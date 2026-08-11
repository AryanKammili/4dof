#include <cmath>
#include <string>
#include <esp32-hal-gpio.h>

class NEMA17{

    private:
        std::string name;
        int DIR;
        int STEP;
        int MS1;
        int MS2;
        int MS3;
        int ENABLE;
        int SLEEP;

        int ledcChannel;
        static int nextLedcChannel;

        int microstepMultipler = 1;
        float currentRPM = 0; // <- Magnitude specific 
        bool isDirectionCW = true;

        static const int STEPS_PER_REVOLUTION = 200;

    public:

        enum MICROSTEP {
                FULL,
                HALF,
                QUARTER,
                EIGHT,
                SIXTEENTH
            };

        MICROSTEP currentStep;

        NEMA17(std::string name, int DIR, int STEP, int MS1, int MS2, int MS3, int ENABLE, int SLEEP) {
            this -> name = name;
            this -> DIR = DIR;
            this -> STEP = STEP;
            this -> MS1 = MS1;
            this -> MS2 = MS2;
            this -> MS3 = MS3;
            this -> ENABLE = ENABLE;
            this -> SLEEP = SLEEP;

            // Defaults to Full //
            currentStep = MICROSTEP::FULL;
            ledcChannel = nextLedcChannel++;


        };

        bool begin() {
            pinMode(DIR, OUTPUT);
            pinMode(MS1, OUTPUT);
            pinMode(MS2, OUTPUT);
            pinMode(MS3, OUTPUT);
            pinMode(ENABLE, OUTPUT);
            pinMode(SLEEP, OUTPUT);

            ledcSetup(ledcChannel, 1000, 8);
            ledcAttachPin(STEP, ledcChannel);

            wake();
            enable();

            return true;
        }

        
    void wake() {
        setPIN(SLEEP, HIGH);
        delay(1);
    };

    void sleep() {
        setPIN(SLEEP, LOW);
    }

    void enable() {
        setPIN(ENABLE, LOW);
    }

    void disable() {
        setPIN(ENABLE, HIGH);
    }

    void setMicroStep(MICROSTEP config) {
        switch(config) {
            case FULL:
                setPIN(MS1, LOW);
                setPIN(MS2, LOW);
                setPIN(MS3, LOW);
                microstepMultipler = 1;   
                break;           
            case HALF:
                setPIN(MS1, HIGH);
                setPIN(MS2, LOW);
                setPIN(MS3, LOW);
                microstepMultipler = 2;
                break;     
            case QUARTER:
                setPIN(MS1, LOW);
                setPIN(MS2, HIGH);
                setPIN(MS3, LOW);  
                microstepMultipler = 4;  
                break; 
            case EIGHT:
                setPIN(MS1, HIGH);
                setPIN(MS2, HIGH);
                setPIN(MS3, LOW);   
                microstepMultipler = 8;
                break;  
            case SIXTEENTH:
                setPIN(MS1, HIGH);
                setPIN(MS2, HIGH);
                setPIN(MS3, HIGH);    
                microstepMultipler = 16;
                break; 
        }

        currentStep = config;
        applyVelocityMagnitude();
    };

    void setVelocity(float RPM) {
        isDirectionCW = (RPM >= 0);
        currentRPM = fabs(RPM);
        setDirection(isDirectionCW);
        applyVelocityMagnitude();
    }

    void stop() {
        currentRPM = 0;
        ledcWrite(ledcChannel, 0);
    }

    void applyVelocityMagnitude() {
        if (currentRPM <= 0) {
            ledcWrite(ledcChannel, 0);
            return;
        }

        float stepsPerSec = (currentRPM / 60.0) * STEPS_PER_REVOLUTION * microstepMultipler;
        ledcWriteTone(ledcChannel, stepsPerSec);
    }

    void setDirection(bool clockwise) {
        if(clockwise) {
            setPIN(DIR, HIGH);
        }

        else{
            setPIN(DIR, LOW);
        }
    }

    void setPIN(uint8_t PIN, uint8_t VALUE) {
        digitalWrite(PIN, VALUE);
    };
};

int NEMA17::nextLedcChannel = 0;