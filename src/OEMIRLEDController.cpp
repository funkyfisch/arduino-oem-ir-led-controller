
#include "OEMIRLEDController.h"

/**
 * Initialises the controller object
*/ 
OEMIRLEDController::OEMIRLEDController() {
    #ifndef _AVR_INTERRUPT_H_
    _irsend = new IRsend(4);
    _irsend->begin();
    #else
    _irsend = new IRsend();
    #endif
    reset();
}

/**
 * Sends a command using the IRemote library.
 * These LEDs use the NEC protocol for communication.
 * All commands are listed on Protocol.h
*/ 
void OEMIRLEDController::sendBurst(unsigned long command) {
    _irsend->sendNEC(command, LSIR_BITS);
    delay(30);
}

/**
 * Powers on the LEDs
*/ 
void OEMIRLEDController::powerOn() {
    sendBurst(LSIR_ON);
}

/**
 * Powers off the LEDs
*/ 
void OEMIRLEDController::powerOff() {
    sendBurst(LSIR_OFF);
}

/**
 * Sets a specific colour to the LEDs
*/ 
void OEMIRLEDController::setColour(unsigned long colorCode) {
    sendBurst(colorCode);
}

/**
 * Directly sets a specific brightness level to the LEDs
 * Currently a range of 10 levels is supported.
 * If the given level is out of the range, it falls back to the nearest extreme value
 * (e.g. If given value is 12, the level will be set to BRIGHTNESS_MAX)
 * Otherwise, we determine if the target level is higher or lower of 
 * the current one, and then we use the existing methods to raise or lower 
 * the brightness to match the target level
**/ 
void OEMIRLEDController::setBrightness(int brightnessLevel) {
    
    if (brightnessLevel > BRIGHTNESS_MAX) brightnessLevel = BRIGHTNESS_MAX;
    if (brightnessLevel < BRIGHTNESS_MIN) brightnessLevel = BRIGHTNESS_MIN;

    if (brightnessLevel > _currentBrightness) {
        while (brightnessLevel != _currentBrightness) brighten();
    } else if (brightnessLevel < _currentBrightness) {
        while (brightnessLevel != _currentBrightness) darken();
    }

    _currentBrightness = brightnessLevel;
}

/**
 * Turn up the brightness. This will only work if the LEDs are in state IDLE.
 * Otherwise if the same command is sent, it will intensify the equivalent effect 
 * (e.g. if the LEDs are strobing, they will strobe faster)
 * Therefore, we first set the state to IDLE by resetting back to a white colour.
 * Then, if we are not already at maximum brigthness, we increment the brightness level.
**/
void OEMIRLEDController::brighten() {
    
    if (_currentState != CONTROLLER_STATE_IDLE) {
        sendBurst(LSIR_WHITE);
        _currentState = CONTROLLER_STATE_IDLE;
    }

    if (_currentBrightness == 10) return;

    sendBurst(LSIR_BRIGHTNESS_PLUS);
    _currentBrightness++;
}

/**
 * The exact reverse of IRLEDController::brighten()
**/
void OEMIRLEDController::darken() {
    
    if (_currentState != CONTROLLER_STATE_IDLE) {
        sendBurst(LSIR_WHITE);
        _currentState = CONTROLLER_STATE_IDLE;
    }

    if (_currentBrightness == 1) return;

    sendBurst(LSIR_BRIGHTNESS_MINUS);
    _currentBrightness--;
}

/**
 * Method to synchronise this object's state with the actual LEDs' state.
 * We turn on the LEDs, set the colour to white, set the brightness as 
 * low as possible, turn off the LEDs and store the state in this object.
**/
void OEMIRLEDController::reset() {
    sendBurst(LSIR_ON);
    sendBurst(LSIR_WHITE);
    for(int i = 0; i < 10; i++) {
        sendBurst(LSIR_BRIGHTNESS_MINUS);
    }
    sendBurst(LSIR_OFF);
    _currentBrightness = 1;
    _currentState = CONTROLLER_STATE_OFF;
}

/**
 * Method to switch between the various modes of the LED.
 * Refer to ControllerState.h.
**/
void OEMIRLEDController::setMode(int mode) {
    if (mode != _currentState) {
        switch(mode) {
            case CONTROLLER_STATE_FADE:
                sendBurst(LSIR_MODE_FADE);
                break;
            case CONTROLLER_STATE_SMOOTH:
                sendBurst(LSIR_MODE_SMOOTH);
                break;
            case CONTROLLER_STATE_STROBE:
                sendBurst(LSIR_MODE_STROBE);
                break;
            case CONTROLLER_STATE_IDLE:
                sendBurst(LSIR_WHITE);
            default: break;
        }
        _currentState = mode;
    }
}
