#include "commands.h"
#include <RTClib.h>
#define COMMAND_LENGTH 128
/// builds command string from incoming characters
/// @param commandChar incoming character
void buildCommand(const char commandChar) {

    static char commandBuffer[COMMAND_LENGTH] = {};
    static int index = 0;
    static bool receiving = false;

    // Serial.println(String(commandBuffer));
    if (commandChar == '*'){
        memset(commandBuffer, 0, sizeof(commandBuffer));
        commandBuffer[index] = '\0'; // null-terminate the string
        index = 0;
        receiving = true;
        return;
    }

    if ((commandChar == '\n' || commandChar == '\r') && index > 0 && commandBuffer[index-1] == '#') {
        commandBuffer[index-1] = '\0'; // null-terminate the string
        receiving = false;
        Serial.println("Received command: " + String(commandBuffer));
        readCommand(commandBuffer, index);
        return;
    }

    if (commandChar == '\n') {
        memset(commandBuffer, 0, sizeof(commandBuffer));
        receiving = false;
        index = 0;
        return;
    }

    if (receiving == true && index < static_cast<int>(sizeof(commandBuffer) - 1)) {
        commandBuffer[index++] = commandChar;
        commandBuffer[index] = '\0'; // keep buffer safely null-terminated
    }

}

/// reads incoming serial data and sends data to buildCommand
void readSerial() {
    while (Serial.available() > 0) {
        const char incomingChar = static_cast<char>(Serial.read());
        Serial.print(incomingChar);
        buildCommand(incomingChar);
    }
}

void readCommand(const char command[], const int length) {
    //parse command and set flags accordingly
    char parsedCommand[COMMAND_LENGTH] {};
    int param1 {0};
    int param2 {0};
    int param3 {0};
    int numParams {0};

    for (int i = 0; i < length && i < COMMAND_LENGTH - 1; ++i) {

        //fills in int parameters
        if (command[i] >= '0' && command[i] <= '9'){
            if (numParams == 1) {
                param1 = param1 * 10 + (command[i] - '0');
            }
            else if (numParams == 2) {
                param2 = param2 * 10 + (command[i] - '0');
            }
            else if (numParams == 3) {
                param3 = param3 * 10 + (command[i] - '0');
            }
        }

        if (command[i] == ':') {
            if (numParams == 0) parsedCommand[i] = '\0';
            numParams += 1;
        }

        if (numParams == 0) parsedCommand[i] = command[i];
    }

    Serial.println(String(parsedCommand) + "\n" + String(param1) + "\n" + String(param2) + "\n" + String(param3));

    commandHandler(parsedCommand, param1, param2, param3);
}

void sendCommand(const char* command, const int param1, const int param2, const int param3) {

    Serial.print("*");
    Serial.print(command);
    if (param1 != -1) {
        Serial.print(":");
        Serial.print(param1);
    }
    if (param2 != -1) {
        Serial.print(":");
        Serial.print(param2);
    }
    if (param3 != -1) {
        Serial.print(":");
        Serial.print(param3);
    }
    Serial.println("#");
}