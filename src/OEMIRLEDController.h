#pragma once

#include <Arduino.h>
#include <IRremote.h>

#include "Protocol.h"
#include "ControllerState.h"

#define BRIGHTNESS_MAX 10
#define BRIGHTNESS_MIN 1

class OEMIRLEDController {
    public:
        OEMIRLEDController();
        void powerOn();
        void powerOff();
        void setColour(unsigned long colorCode);
        void brighten();
        void darken();
        void reset();
        void setBrightness(int brightnessLevel);
        void setMode(int mode);


    private:
        void sendBurst(unsigned long command);
        IRsend * _irsend;
        int _currentBrightness;
        unsigned long _currentColour;
        int _currentState;
};