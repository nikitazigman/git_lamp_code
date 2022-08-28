#ifndef _LED_ANIMATION_SETTINGS_H
#define _LED_ANIMATION_SETTINGS_H

class AnimationSettings
{
public:
    float intensity;
    int pixel_number;
    AnimationSettings(int pixel_number, float intensity)
    {
        this->intensity = intensity;
        this->pixel_number = pixel_number;
    }
};

#endif // _LED_ANIMATION_SETTINGS_H