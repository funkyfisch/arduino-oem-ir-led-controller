#include <OEMIRLEDController.h>

OEMIRLEDController* controller;
void setup() {
  controller = new OEMIRLEDController();
  
}

void loop() {
    controller->powerOn();
    delay(2000);
    controller->powerOff();
    delay(2000);
}