#include <iostream>
#include <string>
#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants {

    public:
        // J1 Encoder Constants //
        static const int J1_ENCODER_SDA = 4;
        static const int J1_ENCODER_SCL = 5;
        static const int J1_ENCODER_DIR = 6;

        static const int J1_ENCODER_OFFSET = 0.0;
        static const int J1_ENCODER_GEARING = 1.0;
        static const bool J1_ENCODER_INVERT = false;

        static const int J1_DRIVER_MS1 = 18;
        static const int J1_DRIVER_MS2 = 17;
        static const int J1_DRIVER_MS3 = 16;
        static const int J1_DRIVER_DIR = 7;
        static const int J1_DRIVER_STEP = 15;
        static const int J1_DRIVER_SLP = 9;
        static const int J1_DRIVER_ENABLE = 8;
};

#endif