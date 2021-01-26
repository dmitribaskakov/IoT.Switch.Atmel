#include <Arduino.h>

#include "StatusLed.h"
uint32_t timeToStartMainLoop = 0;
#define timeMainLoopMax 25  // время для сна
StatusLed statusLed(13);

void setup() {
  
}

void loop() {
  timeToStartMainLoop = millis();
	statusLed.update();		//мигнем светодиодом что контроллер жив

  timeToStartMainLoop = millis() - timeToStartMainLoop;
	if (timeToStartMainLoop<timeMainLoopMax) delay(timeMainLoopMax - timeToStartMainLoop);

}