#ifndef _LED_H
#define _LED_H

#include <Thread.h>
#include <Adafruit_NeoPixel.h>

uint32_t pallet[60] = {
    0x00FFFB,
    0x04FBF8,
    0x09F6F6,
    0x0DF2F3,
    0x12EDF0,
    0x16E9ED,
    0x1AE5EB,
    0x1FE0E8,
    0x23DCE5,
    0x27D8E2,
    0x2CD3E0,
    0x30CFDD,
    0x35CADA,
    0x39C6D8,
    0x3DC2D5,
    0x42BDD2,
    0x46B9CF,
    0x4AB5CD,
    0x4FB0CA,
    0x53ACC7,
    0x58A7C5,
    0x5CA3C2,
    0x609FBF,
    0x659ABC,
    0x6996BA,
    0x6D92B7,
    0x728DB4,
    0x7689B1,
    0x7B84AF,
    0x7F80AC,
    0x7F80AC,
    0x837CA9,
    0x8877A7,
    0x8C73A4,
    0x906EA1,
    0x956A9E,
    0x99669C,
    0x9D6199,
    0xA25D96,
    0xA65893,
    0xAA5491,
    0xAF4F8E,
    0xB34B8B,
    0xB74789,
    0xBC4286,
    0xC03E83,
    0xC53980,
    0xC9357E,
    0xCD317B,
    0xD22C78,
    0xD62876,
    0xDA2373,
    0xDF1F70,
    0xE31A6D,
    0xE7166B,
    0xEC1268,
    0xF00D65,
    0xF40962,
    0xF90460,
    0xFD005D,
};
class LEDThread : public Thread
{
private:
    bool *event;
    Adafruit_NeoPixel *pixels;

    int get_current_value()
    {
        static float value = 0;
        value += 0.1;
        return int(((sin(value) + 1) / 2.0) * 60);
    }

    void animation(void)
    {
        int current_value = this->get_current_value();
        for (int led = 0; led < this->pixels->numPixels(); led++)
        {
            this->pixels->setPixelColor(led, pallet[current_value]);
        }
        this->pixels->show();
    }

public:
    LEDThread(Adafruit_NeoPixel *pixels, bool *event) : Thread()
    {
        this->pixels = pixels;
        this->event = event;
    }
    void init(void)
    {
        this->pixels->begin();
        this->pixels->clear();
    }
    void run(void)
    {
        if (*this->event)
        {
            this->animation();
        }
        else
        {
            this->pixels->clear();
            this->pixels->show();
        }
        this->runned();
    }
};

#endif // _LED_H