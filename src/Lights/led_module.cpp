
#include "led_module.h"

#define DELTA1 1
#define DELTA2 2
#define DELTA3 10
#define DELTA_DEFAULT 5

#define POINT1 20
#define POINT2 40
#define POINT3 190

/// everytime function is called, lights get dimmed more
void LedModule::dim() {
    unsigned long delay = 50;
    if (profile == slow) {
        delay *= SLOW_MULTIPLIER;
    } else if (profile == superSlow) {
        delay *= SUPER_SLOW_MULTIPLIER;
    }

    if (brightness == MAX_BRIGHTNESS  && state == dimOn) {
        state = on;
        return;
    }
    if (brightness == 0 && state == dimOff){
        state = off;
        return;
    }

    if (brightness == max && (state == dimOn)) {
        state = on;
        return;
    }

    if ((brightness + delta <= max && brightness > max)  && state == dimOff) {
        brightness = max;
        state = on;
    }


    //only return after checks for extra safety
    if ((millis() - time) > delay) {
        time = millis();
    } else return;

    if (brightness >= 0 && brightness <= MAX_BRIGHTNESS)
        this->update();

    set(brightness);
    analog_write();

    // Serial.println(brightness);
}

void LedModule::manage_delta() {

    profile_linear();

    if (state == dimOff) delta *= -1;
}

void LedModule::profile_linear() {

    if (brightness < POINT1) {
        delta = DELTA1;
    } else if (brightness < POINT2) {
        delta = DELTA2;
    } else if (brightness > POINT3) {
        delta = DELTA3;
    } else {
        delta = DELTA_DEFAULT;
    }
}

int LedModule::get_profile_steps() const {
    int bright {0};
    int count{0};

    for (bright = 0; bright <= max; ) {
        if (bright < POINT1) {
            bright += DELTA1;
        } else if (bright < POINT2) {
            bright += DELTA2;
        } else if (bright > POINT3) {
            bright += DELTA3;
        } else {
            bright += DELTA_DEFAULT;
        }
        count++;
    }

    return count;
}
