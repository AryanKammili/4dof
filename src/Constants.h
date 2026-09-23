#include <iostream>
#include <string>
#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants {

    public:
        static const int J1_SDA_PIN = 4;
        static const int J1_SCL_PIN = 5;
        static const int J1_DIR_PIN = 6;

        static const int J1_DRIVER_MS1 = 18;
        static const int J1_DRIVER_MS2 = 17;
        static const int J1_DRIVER_MS3 = 16;
        static const int J1_DRIVER_DIR = 7;
        static const int J1_DRIVER_STEP = 15;
        static const int J1_DRIVER_SLP = 9;
        static const int J1_DRIVER_ENABLE = 8;

        static const int J3_SDA_PIN = 0;
        static const int J3_SCL_PIN = 0;
        static const int J3_DIR_PIN = 0;

        static const int J0_SDA_PIN = 0;
        static const int J4_SCL_PIN = 0;
        static const int J4_DIR_PIN = 0;
};

#endif