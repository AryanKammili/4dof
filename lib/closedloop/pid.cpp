

class pid {

    public: 

        float kP;
        float kI;
        float kD;

        float kS;
        float kV;
        float kG;

        float setpoint;

        float currentError;
        float lastError;

        float timestamp;


        pid(float p, float i, float d, float s, float v, float g, float timestamp) {
            this->kP = p;
            this->kI = i;
            this->kD = d;
            this->kS = s;
            this->kV = v;
            this->kG = g;

            this->timestamp = timestamp;

            currentError = 0.0;
            lastError = 0.0;

        }

        float calculate(float currentPosition, float setpoint) {
            lastError = currentError;
            currentError = setpoint - currentPosition; 

            float derivativeTerm = (currentError - lastError) / (timestamp);

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