//
// Created by ruben on 2025/12/23.
//

#ifndef COMMANDS_H
#define COMMANDS_H

#include <arduino.h>
#include <WString.h>
#include "commandHandler.h"

void readCommand(const char command[], int length);
void buildCommand(char commandChar);
void sendCommand(const char* command, int param1 = -1, int param2 = -1, int param3 = -1);
void readSerial();

#endif //COMMANDS_H
