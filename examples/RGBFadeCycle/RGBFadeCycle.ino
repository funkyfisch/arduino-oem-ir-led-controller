#include <OEMIRLEDController.h>

OEMIRLEDController* controller;

void setup() {
  controller = new OEMIRLEDController();
  controller->powerOn();
}

void loop() {
    controller->setColour(LSIR_BLUE);
    fadeInfadeOut();
    controller->setColour(LSIR_GREEN);
    fadeInfadeOut();
    controller->setColour(LSIR_RED);
    fadeInfadeOut();
}

void fadeInfadeOut() {
    for (int i = 0; i < 10; i++) {
        controller->brighten();
    }
    for (int i = 0; i < 10; i++) {
        controller->darken();
    }
}