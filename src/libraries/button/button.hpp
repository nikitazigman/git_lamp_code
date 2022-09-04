
#ifndef _BUTTON_H
#define _BUTTON_H

#include <Thread.h>

class ButtonThread : public Thread
{
private:
    bool *event;
    int button_pin;
    int power_pin;
    bool pressed;

public:
    ButtonThread(int button_pin, int power_pin, bool *event) : Thread()
    {
        this->button_pin = button_pin;
        this->power_pin = power_pin;
        this->event = event;
        this->pressed = false;
    }
    void init(void)
    {
        pinMode(this->button_pin, INPUT);
        pinMode(this->power_pin, OUTPUT);
        digitalWrite(this->power_pin, HIGH);
    }

    void run()
    {
        if (digitalRead(this->button_pin))
            this->pressed = true;

        if (this->pressed && !digitalRead(this->button_pin))
        {
            this->pressed = false;
            *this->event = !*this->event;
            Serial.println(*this->event);
        }
        this->runned();
    }
};

#endif // _BUTTON_H