#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include "ESP8266TimerInterrupt.h"
#include <Adafruit_NeoPixel.h>

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

class LEDThread : public Thread
{
private:
  Adafruit_NeoPixel pixels;
  bool *event;
  int pixel_number;

public:
  LEDThread(int pixel_number, int data_pin, bool *event) : Thread()
  {
    this->pixel_number = pixel_number;
    this->event = event;

    this->pixels = Adafruit_NeoPixel(pixel_number, data_pin, NEO_GRB + NEO_KHZ800);
  }

  void init()
  {
    this->pixels.begin();
    this->pixels.clear();
  }

  void change_color()
  {
    uint32_t color = pixels.Color(random(0, 255), random(0, 255), random(0, 255));

    for (int led = 0; led < this->pixel_number; led++)
    {
      pixels.setPixelColor(led, color);
    }

    pixels.show();
  }

  void run()
  {
    if (*this->event)
    {
      *this->event = false;
      this->change_color();
    }
    runned();
  }
};

class ButtonThread : public Thread
{
private:
  bool *event;
  int button_pin;
  bool pressed;

public:
  ButtonThread(int button_pin, bool *event) : Thread()
  {
    this->button_pin = button_pin;
    this->event = event;
    this->pressed = false;
  }

  void init()
  {
    pinMode(this->button_pin, INPUT);
  }

  void run()
  {

    if (digitalRead(this->button_pin))
      this->pressed = true;

    if (pressed && !digitalRead(this->button_pin))
    {
      this->pressed = false;
      *this->event = true;
    }
    runned();
  }
};

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
