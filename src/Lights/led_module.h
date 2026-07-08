//
// Created by ruben on 2025/12/23.
//

#ifndef LED_MODULE_H
#define LED_MODULE_H

#include <Arduino.h>
#include<time_custom.h>

#define MAX_BRIGHTNESS 255
#define SLOW_MULTIPLIER 10
#define SUPER_SLOW_MULTIPLIER 50

enum State{
    on = 1, dimOn = 2,
    off = 3, dimOff= 4,
    dimmed,
    waiting, set, unset,
};

enum DimProfile {
    fast,
    slow,
    superSlow,
};

class LedModule {
public:

    int delta = 0;
    State state = off;
    DimProfile profile = fast;

    int get() const { return brightness;}
    int get_max() const {return max;}
    int get_profile_steps() const;
    void set(int new_brightness) {
        verify_brightness(&new_brightness);
        brightness = new_brightness;

        if (brightness == 0) state = off;
        else if ((brightness == max || brightness == MAX_BRIGHTNESS) && state != dimOff) state = on;
    }
    void set_max(const int new_max, const bool follow_max = true) {
        const bool is_maxed = (brightness == max);
        max = new_max > MAX_BRIGHTNESS ? MAX_BRIGHTNESS : new_max;
        // ensure max is at least 1
        max = max <= 0 ? 1 : max;
        Serial.println(max);

        if (is_maxed && brightness < max) state = dimOn;
        if (brightness > max) state = dimOff;
        else if (brightness < max && follow_max) state = dimOn;
    }

    void update() {
        this->set(brightness + delta);
    }
    void analog_write() const {
        // Serial.println("Writing to pin " + String(pin_no) + " brightness " + String(brightness));
        analogWrite(pin_no, brightness);
    }

    void set_time_trigger(const Time time_on,const Time time_off) {

        on_time = time_on;
        off_time = time_off;

        if (currentTimeInRange(time_on, time_off)) {
            state = dimOn;
        } else {
            state = dimOff;
        }
    }
    void set_trigger_delays(const int delay_on, const int delay_off) {
        on_delay = delay_on;
        off_delay = delay_off;
    }

    void check_time_trigger() {
        Time current_time {0, 0};
        current_time.set(now());

        // Serial.println(pin_no);
        // Serial.println((on_time + on_delay).toString());
        // Serial.println((off_time + off_delay).toString());

        if (current_time == (on_time + on_delay) && (state == off || state == dimOff)) {
            profile = superSlow;
            state = dimOn;
        } else if (current_time == off_time + off_delay && (state == on || state == dimOn)) {
            profile = superSlow;
            state = dimOff;
        }
    }

    void dim();
    void manage_delta();
    void profile_linear();

    explicit LedModule(const int pin) : pin_no(pin){}

private:
    void verify_brightness(int *x) const {
        if (*x > MAX_BRIGHTNESS) *x =  MAX_BRIGHTNESS;
        if (*x < 0) *x = 0;
        if (*x < max + delta && *x > max - delta) *x = max;
    }

    Time on_time = {14, 0};
    Time off_time = {21, 0};

    int on_delay = 0;
    int off_delay = 0;

    int brightness = 0;
    int max = 255;
    unsigned long time = millis();
    const int pin_no;
};

void manageDeltaBright(LedModule& module);
void profileLinear(LedModule& module);

#endif //LED_MODULE_H
