#include <Arduino.h>
#include "light.h"

Time parseTime(const int timeInt) {
    Time time {0, 0};
    time.hours = floor(static_cast<double>(timeInt) / 60.0);
    time.min = timeInt % 60;
    return time;
}

//first turns everything off, then on alltogether
void Light::toggleDim() {

    const bool synced = right.state == left.state;
    const State state = right.state;

    if (synced) {
        if (state == on) {
            right.state = dimOff;
            left.state = dimOff;
            return;
        } if (state == off) {
            right.state = dimOn;
            left.state = dimOn;
            return;
        }
    }

    const bool left_on = (left.state == on || left.state == dimOn);
    const bool right_on = (right.state == on || right.state == dimOn);

    // prefer turning on instead of off, only turn off when both are on/ turning on
    if (left_on && right_on) {
        right.state = dimOff;
        left.state = dimOff;
        return;
    }
    if (left_on) {
        left.state = dimOff;
        return;
    }
    if (right_on) {
        right.state = dimOff;
        return;
    }
}

void Light::turnOff() {
    if (right.state != off || right.state != dimOff)
        right.state = dimOff;
    if (left.state != off ||left.state != dimOff)
        left.state = dimOff;
}

void Light::turnOn() {
    if (right.state != on || right.state != dimOn)
        right.state = dimOn;
    if (left.state != on || left.state != dimOn)
        left.state = dimOn;
}

void Light::dualToggle(const int left_power, const int right_power) {
    if (left_power == 1 && (left.state == off || left.state == dimOff)) {
            left.state = dimOn;
    } else if (left_power == 0 && (left.state == on || left.state == dimOn)) {
            left.state = dimOff;
    }

    if (right_power == 1 && (right.state == off || right.state == dimOff)) {
            right.state = dimOn;
    } else if (right_power == 0 && (right.state == on || right.state == dimOn)) {
            right.state = dimOff;
    }
}

/// toggles each side independently
void Light::toggleDimAbs() {
    if (left.state == on) {
        left.state = dimOff;
    }else if (left.state == off) {
        left.state = dimOn;
    }

    if (right.state == on) {
        right.state = dimOff;
    }else if (right.state == off) {
        right.state = dimOn;
    }
}

void Light::dimLights() {

    if (right.state == dimOn || right.state == dimOff) {
        right.manage_delta();
        right.dim();
    }
    if (left.state == dimOn || left.state == dimOff) {
        left.manage_delta();
        left.dim();
    }
}

void Light::dimLightsLeftLag(const bool resetTimer) {

    static unsigned long last_time = millis();
    int delay = 2000;

    if (right.profile == slow) delay *= SLOW_MULTIPLIER;

    if (resetTimer) last_time = millis();

    if (right.state == dimOn || right.state == dimOff) {
        right.manage_delta();
        right.dim();
    } if ((left.state == dimOn || left.state == dimOff) && (millis() - last_time) > static_cast<unsigned long>(delay)) {
        left.manage_delta();
        left.dim();
    }
}

void Light::dimLightsRightLag(const bool resetTimer) {

    static unsigned long last_time = millis();
    unsigned long delay = 2000;

    if (left.profile == slow) delay *= SLOW_MULTIPLIER;
    if (resetTimer) last_time = millis();

    if (left.state == dimOn || left.state == dimOff) {
        left.manage_delta();
        left.dim();
    } if ((right.state == dimOn || right.state == dimOff) && (millis() - last_time) > delay) {
        right.manage_delta();
        right.dim();
    }
}

/*
 * @param l_r: 00 = left left, 01 = left right, 10 = right left, 11 = right right
 */
void Light::setTriggerDelays(const int delay_on, const int delay_off, const int l_r) {

    const int delay_left_on = l_r <= 1 ? delay_on : 0;
    const int delay_left_off = (l_r == 0 || l_r == 10) ? delay_off : 0;

    const int delay_right_on = l_r >= 10 ? delay_on : 0;
    const int delay_right_off = (l_r == 1 || l_r == 11) ? delay_off : 0;

    left.set_trigger_delays(delay_left_on, delay_left_off);
    right.set_trigger_delays(delay_right_on, delay_right_off);
}
