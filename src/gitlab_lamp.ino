#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <StaticThreadController.h>
#include <ThreadController.h>
#include "ESP8266TimerInterrupt.h"

#include "libraries/led/led.hpp"
#include "libraries/button/button.hpp"
#include "libraries/http/http.hpp"

#define TIMER_INTERVAL_MS 1

#define BUTTON_PIN 14
#define LED_DATA_PIN 15

#define PIXEL_NUMBER 6
#define INTENSITY 0.02

#define SSID "VM2381623"
#define PASSWORD "6qfsbsKFzws4"

ESP8266Timer ITimer;
ThreadController controll = ThreadController();
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_NUMBER, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

bool EVENT = false;
LEDThread led_thread = LEDThread(&pixels, &EVENT);
ButtonThread button_thread = ButtonThread(BUTTON_PIN, &EVENT);
HttpThread http_tread = HttpThread(SSID, PASSWORD);

void setup()
{
  Serial.begin(115200);
  // Configure myThread
  led_thread.init();
  led_thread.setInterval(10);

  button_thread.init();
  button_thread.setInterval(100);

  http_tread.init();
  http_tread.setInterval(1000);

  // if (!ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler))
  //   Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));

  controll.add(&led_thread);
  controll.add(&button_thread);
  controll.add(&http_tread);
}

void loop()
{
  controll.run();
}
