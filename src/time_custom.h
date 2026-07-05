//
// Created by ruben on 2026/01/28.
//

#ifndef TIME_CUSTOM_H
#define TIME_CUSTOM_H
#include <TimeLib.h>


struct Time {
    int hours; //max 24;
    int min; // max 60;

    time_t toTime() const {
        return static_cast<time_t>(hours * 3600000 + min * 60000);
    }

    void set(const Time& other) {
        hours = other.hours;
        min = other.min;
    }

    void set(const time_t t) {
        hours = hour(t);
        min = minute(t);
    }

    bool operator == (const Time& other) const {
        return (hours == other.hours && min == other.min);
    }

    bool operator != (const Time& other) const {
        return !(hours == other.hours && min == other.min);
    }

    Time operator + (const Time& other) const {
        Time result {0,0};
        result.hours = hours + other.hours;
        result.min = min + other.min;

        if (result.min >= 60) {
            result.hours += result.min / 60;
            result.min = result.min % 60;
        }
        if (result.hours >= 24) {
            result.hours = result.hours % 24;
        }

        return result;
    }
    Time operator + (const int mins) const {
        Time result {hours, min + mins};

        if (result.min >= 60) {
            result.hours += result.min / 60;
            result.min = result.min % 60;
        }
        if (result.hours >= 24) {
            result.hours = result.hours % 24;
        }

        return result;
    }

    //lowk have no idea if this works or not, will have to see
    Time operator - (const Time& other) const {
        Time result {0,0};
        result.hours = hours - other.hours;
        result.min = min - other.min;

        if (result.min < 0) {
            result.hours += ceil(result.min / 60.0);
            result.min = 60 + result.min % 60;
        }
        if (result.hours < 0) {
            result.hours = 24 - result.hours % 24;
        }

        return result;
    }
    Time operator - (const int mins) const {
        Time result {hours, min - mins};

        if (result.min < 0) {
            result.hours += ceil(result.min / 60.0);
            result.min = 60 + result.min % 60;
        }
        if (result.hours < 0) {
            result.hours = 24 - result.hours % 24;
        }

        return result;
    }

    String toString() const {
        return String(hours) + ":" + String(min);
    }
};


#endif //TIME_CUSTOM_H
