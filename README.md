# OEM IR LED Controller

## A simple way to control generic off-the-shelf IR LEDS, with just an IR LED!

### Work in Progress - Watch the repo for follow-ups or contribute. Check the TODOs.md for ideas

This library provides a simple control interface for generic purpose IR LEDs found in the market.
This is the result of a home automation project where I wanted to hijack control from the provided IR remote
control.

## Usage

### Hardware
For <b> ESP8266 </b> based boards (Wemos, NodeMCU, etc), connect the long leg of your IR LED to the D2 pin.
For <b> Arduino Nano/UNO </b>, connect it to the D3 pin.
These pins work with PWM, and are suitable for sending pulse bursts with an LED.
Depending on your LED, you should also consider adding a resistor between the long led of your IR LED and the D# pin.
Connect the short leg of the LED on to a GND pin.

### API
You need to include this library and create an instance of the IR LED controller.
```
#include <OEMIRLEDController.h>
OEMIRLEDController* controller;

void setup() {
    controller = new OEMIRLEDController();
}
```

After you do that, you can use the following controls and the corresponding pulse for them will be sent.
```
controller->powerOn();                       // powers on the led bulb/strip
controller->powerOff();                      // powers off the led bulb/strip
controller->brighten();                      // increases brightness level of bulb/strip (10 levels supported)
controller->darken();                        // decreases brightness level of bulb/strip (10 levels supported)
controller->setBrightness(8);                // sets the brightness to 8 (max 10)
controller->setMode(CONTROLLER_STATE_STROBE);// sets the special mode (see reference below)
controller->setColour(LSIR_PEAGREEN);        // sets the colour of the RGB leds (see reference below)
controller->reset();                         // synchronises the state of the controller with the actual state of LED bulb/strip
```

### Controller Mode Reference
These are the modes currently supported to be used with <b> setMode() </b> :
```
CONTROLLER_STATE_IDLE                        // idle state, continuous light
CONTROLLER_STATE_STROBE                      // strobing mode
CONTROLLER_STATE_FADE                        // fading mode, fades in and out
CONTROLLER_STATE_SMOOTH                      // smooth mode
```

### Colour Reference
These are the colours currently supported to be used with <b> setColour() </b> :
```
LSIR_RED
LSIR_ORANGE
LSIR_DARKYELLOW
LSIR_YELLOW
LSIR_LIGHTYELLOW
LSIR_GREEN
LSIR_PEAGREEN
LSIR_CYAN
LSIR_LIGHTBLUE
LSIR_SKYBLUE
LSIR_BLUE
LSIR_DARKBLUE
LSIR_BROWN
LSIR_PURPLE
LSIR_MAGENTA
LSIR_WHITE
```

### Example Use Case
In this example, we want to create a special effect, where we would cycle through Red, Green and Blue colours, while fading in/out in between. You can find this sketch in the Examples folder.
```
#include <OEMIRLEDController.h>
OEMIRLEDController* controller;

void setup() {
  controller = new OEMIRLEDController();
  controller->powerOn(); // start our sketch with the LED powered on.
}

// set a colour, and then fade in / out
void loop() {
    controller->setColour(LSIR_BLUE);
    fadeInfadeOut();
    controller->setColour(LSIR_GREEN);
    fadeInfadeOut();
    controller->setColour(LSIR_RED);
    fadeInfadeOut();
}

// our utility method that pumps up the brightness all the way up and then 
// all the way down
void fadeInfadeOut() {
    for (int i = 0; i < 10; i++) {
        controller->brighten();
    }
    for (int i = 0; i < 10; i++) {
        controller->darken();
    }
}
```


### Notes.
 - Using <b>setColour()</b> while the LED is not in CONTROLLER_STATE_IDLE will cause the LED bulb/strip to reset to  idle.
 - By default, when creating the controller, it will attempt to reset the LED and after it finishes, it switches the bulb off. If you want to start with the LED on, you need to add <b>controller->powerOn()</b> to your <b>setup()</b> method

## Supported devices
Please consult with the SupportedDevices.md. Right now only the LedSavers LED strip is confirmed to be supported,
however, it seems like this library is already compatible with many IR controlled LEDs out there. You can help by testing this with your own LED brand.