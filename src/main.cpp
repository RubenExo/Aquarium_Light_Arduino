#include <Arduino.h>
#include <TimerOne.h> // Library for easier timer management
#include <Commands/commands.h>
#include <Lights/light.h>
#include <TimeLib.h>

#define LED_PIN 13
#define LEFT 5
#define RIGHT 3



Light light(LEFT, RIGHT);
time_t t = now(); // store the current time in time variable t
bool checkAlarms = false;

void flagHandler();
void setCheckFlags();

void checkBrightness() {
    light.readBrightness();
}

void setup() {

    Serial.begin(9600);

    // if (!rtc.begin()) {
    //     Serial.println("Couldn't find rtc, system will fail!");
    // }

    pinMode(LEFT, OUTPUT);
    pinMode(RIGHT, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    // light.setProfile(SlowLinear);
    Timer1.initialize(15000000); // Set timer to trigger every 15 seconds
    Timer1.attachInterrupt(setCheckFlags);
    // Timer1.initialize(5000000); // Set timer to trigger every 15 seconds
    // Timer1.attachInterrupt(checkBrightness);

}

void loop() {

    light.dimLights();
    readSerial();

    flagHandler();
}

void flagHandler() {

    if (checkAlarms) {
        light.checkTimeTriggers();
        checkAlarms = false;
    }
}

void setCheckFlags() {
    checkAlarms = true;
}