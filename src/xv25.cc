#include <stdlib.h>
#include <sstream>
#include <errno.h>
#include "xv25.hh"
#include <string.h>
#include "serial.h"


XV25::XV25(string portName)
{
    this->portName = portName;
}

XV25::~XV25()
{
    if (0 <= port)
        close(port);
}

status_t XV25::connect()
{
    status_t ret = STATUS_OK;

    port = serial_open(portName.c_str(), B9600);
    if (port <= 0) {
        cerr << "Failed to open \"" << portName << "\"" << endl;
        ret = STATUS_ERROR;
    }

    return ret;
}

void XV25::disconnect()
{
    if (0 < port)
        close(port);
}


status_t XV25::send(string cmd)
{
    status_t ret = STATUS_OK;

    cerr << "Sending command \"" << (cmd.substr(0, cmd.size()-1)) << "\"" << endl;

    if (port > 0)
        for (uint8_t i = 0; i < cmd.size(); i++)
            serial_send(port, cmd[i]);

    return ret;
}

string XV25::receive(void)
{
    string response;
    uint8_t byte;

    do {
        byte = serial_recv(port);
    } while (0 == byte);
    response += (char)byte;

    cerr << "read " << byte << endl;

    if (0 < port) {
        do {
            byte = serial_recv(port);

            cerr << "read " << byte << endl;

            if (0 != byte)
                response += (char)byte;
        } while (0 != byte);
        if (response.size() > 0)
            response = response.substr(0, response.size()-1);
        for (uint32_t i = 0; i < 1000; i++) {
            usleep(1000);
            serial_recv(port);
        }
    } else {
        cerr << "Failed to read from \"" << portName << "\"" << endl;
    }

    return response;
}

status_t XV25::command(string cmd)
{
    status_t ret = STATUS_OK;

    if (STATUS_OK == send(cmd+"\n"))
        receive();
    else
        ret = STATUS_ERROR;
    
    return ret;
}

status_t XV25::commandWithResponse(string cmd, string *response)
{
    status_t ret = STATUS_OK;

    if (STATUS_OK == send(cmd+"\n"))
    	*response = receive();
    else
        ret = STATUS_ERROR;
    
    return ret;
}

status_t XV25::getVersion(string* version)
{
    return commandWithResponse("GetVersion", version);
}

status_t XV25::setTestMode(testMode_t testMode)
{
    string tmp;
    string cmd = "TestMode ";
    cmd += (testModeOn == testMode) ? "On" : "Off";
    return commandWithResponse(cmd, &tmp);
}

status_t XV25::setMotor(motor_t motor, int speed, int distance)
{
    string tmp;
    status_t ret = STATUS_OK;;
    string cmd = "SetMotor ";

    switch (motor) {
        case leftWheel: cmd += "LWheelDist "; break;
        case rightWheel: cmd += "RWheelDist "; break;
        default: ret = STATUS_ERROR;
    }
    
    if (STATUS_OK == ret) {
        ostringstream oss;
        oss << distance << " Speed " << speed;
        cmd += oss.str();
        ret = commandWithResponse(cmd, &tmp);
    }

    return ret;
}

status_t XV25::getEncoder(motor_t motor)
{
    status_t ret = STATUS_OK;
    string cmd = "getMotor ";
    string result;

    switch (motor) {
        case leftWheel: cmd += "LeftWheel"; break;
        case rightWheel: cmd += "RightWheel"; break;
        default: ret = STATUS_ERROR;
    }
    
    if (STATUS_OK == ret)
        ret = commandWithResponse(cmd, &result);

    if (STATUS_OK == ret) {
        string encoderString = "Encoder,";        
        size_t pos = result.find(encoderString);
        pos += encoderString.size();
        size_t end = result.substr(pos).find('\n');
        atoi(result.substr(pos, end).c_str());
    }

    return ret;
}
