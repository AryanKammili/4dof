#include <iostream>
#include <string>
#ifndef REGISTER_H
#define REGISTER_H

class Register {

    public:

        static const uint8_t DATA = 0x36;

        static const uint8_t ZMCO = 0x00;

        static const uint8_t ZPOS_HIGH = 0x01;
        static const uint8_t ZPOS_LOW = 0x02;

        static const uint8_t MPOS_HIGH = 0x03;
        static const uint8_t MPOS_LOW = 0x04;

        static const uint8_t MANG_HIGH = 0x05;
        static const uint8_t MANG_LOW = 0x06;

        static const uint8_t CONF_HIGH = 0x07;
        static const uint8_t CONF_LOW = 0x08;

        static const uint8_t RAW_ANG_HIGH = 0x0C;
        static const uint8_t RAW_ANG_LOW = 0x0D;

        static const uint8_t ANG_HIGH = 0x0E;
        static const uint8_t ANG_LOW = 0x0F;

        static const uint8_t STATUS = 0x0B;
        static const uint8_t AGC = 0x1A;

        static const uint8_t MAG_HIGH = 0x1B;
        static const uint8_t MAG_LOW = 0x1C;

        static const uint8_t BURN = 0xFF;
};

#endif