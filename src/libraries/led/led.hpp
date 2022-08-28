#ifndef _LED_H
#define _LED_H

#include <Thread.h>
#include <Adafruit_NeoPixel.h>

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

#endif // _LED_H