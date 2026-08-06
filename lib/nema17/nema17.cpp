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
        int RESET;
        int SLEEP;

    public:
        NEMA17(std::string name, int DIR, int STEP, int MS1, int MS2, int MS3, int ENABLE, int RESET, int SLEEP) {
            this -> name = name;
            this -> DIR = DIR;
            this -> STEP = STEP;
            this -> MS1 = MS1;
            this -> MS2 = MS2;
            this -> MS3 = MS3;
            this -> ENABLE = ENABLE;
            this -> RESET = RESET;
            this -> SLEEP = SLEEP;
        };

        bool begin() {
            pinMode(DIR, OUTPUT);
            pinMode(MS1, OUTPUT);
            pinMode(MS2, OUTPUT);
            pinMode(MS3, OUTPUT);
            pinMode(ENABLE, OUTPUT);

            setSleep(LOW);

            return true;
        }

        enum MICROSTEP {
            FULL,
            HALF,
            QUARTER,
            EIGHT,
            SIXTEENTH
        };

        
    void setSleep(int VALUE) {
        setPIN(SLEEP, VALUE);
    };

    void setMicroStep(MICROSTEP config) {
        switch(config) {
            case FULL:
                setPIN(MS1, LOW);
                setPIN(MS2, LOW);
                setPIN(MS3, LOW);   
                break;           
            case HALF:
                setPIN(MS1, HIGH);
                setPIN(MS2, LOW);
                setPIN(MS3, LOW);
                break;     
            case QUARTER:
                setPIN(MS1, LOW);
                setPIN(MS2, HIGH);
                setPIN(MS3, LOW);    
                break; 
            case EIGHT:
                setPIN(MS1, HIGH);
                setPIN(MS2, HIGH);
                setPIN(MS3, LOW);   
                break;  
            case SIXTEENTH:
                setPIN(MS1, HIGH);
                setPIN(MS2, HIGH);
                setPIN(MS3, HIGH);    
                break; 
        }
    };

    void setPIN(uint8_t PIN, uint8_t VALUE) {
        digitalWrite(PIN, VALUE);
    };
};