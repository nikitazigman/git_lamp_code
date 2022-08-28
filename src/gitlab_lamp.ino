#include <ThreadController.h>
#include "ESP8266TimerInterrupt.h"

#include "libraries/led/led.hpp"
#include "libraries/button/button.hpp"

#define TIMER_INTERVAL_MS 1
#define LED_DATA_PIN 15
#define PIXEL_NUMBER 10
#define BUTTON_PIN 14

ESP8266Timer ITimer;
ThreadController controll = ThreadController();

void IRAM_ATTR TimerHandler()
{
  controll.run();
}

bool EVENT = false;

LEDThread led_thread = LEDThread(PIXEL_NUMBER, LED_DATA_PIN, &EVENT);
ButtonThread button_thread = ButtonThread(BUTTON_PIN, &EVENT);

void setup()
{
  Serial.begin(115200);

  // Configure myThread
  led_thread.init();
  led_thread.setInterval(100);

  button_thread.init();
  button_thread.setInterval(100);

  controll.add(&led_thread);
  controll.add(&button_thread);

  if (!ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler))
    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));
}

void loop()
{
}
