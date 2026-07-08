#include "commandHandler.h"

extern Light light;

void commandHandler(char command[], const int param1, const int param2, const int param3) {

    if (strcmp(command, "TOGGLE_DIM") == 0) {
        light.setProfile(fast);
        light.toggleDim();
    }
    else if (strcmp(command, "TOGGLE_DIM_ABS") == 0) {
        light.setProfile(fast);
        light.toggleDimAbs();
    }
    else if (strcmp(command, "SET_DIM") == 0 && param2 == 0) {
        light.setNewMax(param1, false);
    }
    else if (strcmp(command, "SET_DIM") == 0 && param2 > 0) {
        light.setNewMax(param1, param2, false);
    }
    else if (strcmp(command, "SET_TIMES") == 0) {
        light.setTimeTriggers(parseTime(param1), parseTime(param2));
    }
    else if (strcmp(command, "UPDATE_TIME") == 0) {
        // param1 = hour, param2 = minute
        setTime(param1, param2, param3);
    }
    else if (strcmp(command, "SET_DELAY") == 0){
        light.setTriggerDelays(param1, param2, param3);
    }
    else if (strcmp(command, "SEND_STATE") == 0) {
        sendState();
    }
    else if (strcmp(command, "TURN_ON") == 0) {
        light.turnOn();
    }
    else if (strcmp(command, "TURN_OFF") == 0) {
        light.turnOff();
    }
    else if (strcmp("TOGGLE_ADV", command) == 0) {
        light.dualToggle(param1, param2);
    }
    else if (strcmp(command, "HANDSHAKE") == 0) {
        Serial.println("*HANDSHAKE_ACK#");
    }
}

void sendState() {
    Serial.print("#CURRENT_STATE:");
    Serial.print(light.getState() == on ? 1 : 0);
    Serial.println("#");
}

void setTime(const int hours, const int minutes, const int seconds) {
    setTime(hours, minutes, seconds, 0, 0, 25);
    Serial.println(String(hour(now())) + ":" + String(minute(now())));
}
