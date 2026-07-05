//
// Created by ruben on 2025/12/23.
//

#ifndef LIGHT_H
#define LIGHT_H

#include "led_module.h"
#include <Arduino.h>
#include <TimeLib.h>
#include <time_custom.h>

Time parseTime(int timeInt);

class Light {
public:

    void readBrightness() const {
        Serial.println(left.get());
        Serial.println(right.get());
    }

    void setProfile(const DimProfile profile) {
        left.profile = profile;
        right.profile = profile;
    }
    void setNewMax(const int new_max, const bool follow_max = true) {
        setProfile(fast);
        left.set_max(new_max, follow_max);
        right.set_max(new_max, follow_max);
    }
    void setNewMax(const int max_left, const int max_right, const bool follow_max = true) {
        setProfile(fast);
        left.set_max(max_left, follow_max);
        right.set_max(max_right, follow_max);
    }

    bool peak() const {
        const int left_br = left.get();
        const int right_br = right.get();

        if (left_br == left.get_max() || left_br == 0) return true;
        if (right_br == right.get_max() || right_br == 0) return true;

        return false;
    }


    void turnOff();
    void turnOn();
    void dualToggle(int left_power, int right_power);
    void toggleDim();
    void toggleDimAbs();

    void dimLights();
    void dimLightsLeftLag(bool resetTimer = false);
    void dimLightsRightLag(bool resetTimer);


    void synchronize() {
        const int brightness = (left.get() + right.get())/2;

        left.set(brightness);
        right.set(brightness);
    }

    void printStates() const {

        Serial.println("\nLeft brightness: " + String(left.get()) + " State: " + String(left.state));
        Serial.println("Right brightness: " + String(right.get()) + " State: " + String(right.state));
    }
    State getState() const {
        if (left.state == on || left.state == dimOn ||
            right.state == on || right.state == dimOn) {
            return on;
        }

        return off;
    }

    Light(const int left_pin, const int right_pin)
        : left(left_pin), right(right_pin){}

    void setTimeTriggers(const Time time_on, const Time time_off) {

        Serial.println(String(time_on.hours) + ":" + String(time_on.min));

        left.set_time_trigger(time_on, time_off);
        right.set_time_trigger(time_on, time_off);
    }

    void setTriggerDelays(int delay_on = 0, int delay_off = 0, int l_r = 0);

    void checkTimeTriggers() {
        left.check_time_trigger();
        right.check_time_trigger();
    }

private:

    Time onTime = {14, 0};
    Time offTime = {21, 0};

    LedModule left;
    LedModule right;
};


#endif //LIGHT_H
