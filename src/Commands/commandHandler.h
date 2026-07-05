//
// Created by ruben on 2025/12/23.
//

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <Arduino.h>
#include "../Lights/light.h"
#include <TimeLib.h>

void commandHandler(char command[], int param1 = -1, int param2 = -1, int param3 = -1);
void sendState();
void setTime(int hours, int minutes, int seconds);

#endif //COMMANDHANDLER_H
