#include <StaticThreadController.h>
#include <ThreadController.h>
#include "ESP8266TimerInterrupt.h"

#include "libraries/led/led.hpp"
#include "libraries/button/button.hpp"

#define TIMER_INTERVAL_MS 1

#define BUTTON_PIN 14
#define LED_DATA_PIN 15

#define PIXEL_NUMBER 6
#define INTENSITY 0.02

ESP8266Timer ITimer;
ThreadController controll = ThreadController();

void IRAM_ATTR TimerHandler()
{
  controll.run();
}

bool EVENT = false;
AnimationSettings animation_settings = AnimationSettings(PIXEL_NUMBER, INTENSITY);
LEDThread led_thread = LEDThread(LED_DATA_PIN, &animation_settings, &EVENT);
ButtonThread button_thread = ButtonThread(BUTTON_PIN, &EVENT);
// 00FFFB -> #FD005D
void setup()
{
  Serial.begin(115200);
  // Configure myThread
  led_thread.init();
  led_thread.setInterval(10);

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
