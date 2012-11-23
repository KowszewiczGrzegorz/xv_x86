#include <stdlib.h>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "xv25.hh"


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
    struct termios tio;

    memset(&tio, 0, sizeof(tio));
    tio.c_cflag = CS8 | CREAD | CLOCAL;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 5;

    port = open(portName.c_str(), O_RDWR | O_NONBLOCK);

    cfsetospeed(&tio, B9600);
    cfsetispeed(&tio, B9600);
    tcsetattr(port, TCSANOW, &tio);

    if (port <= 0) {
        cerr << "Failed to open \"" << portName << "\"" << endl;
        ret = STATUS_ERROR;
    }

    return ret;
}

void XV25::disconnect()
{
    if (port > 0)
        close(port);
}


status_t XV25::send(string cmd, bool needResponse)
{
    status_t ret = STATUS_OK;
    char byte;
    int nb;

    // cerr << "Sending command \"" << (cmd.substr(0, cmd.size()-2)) << "\"" << endl;

    if (port > 0) {
        for (uint8_t i = 0; i < cmd.size(); i++) {
            write(port, &cmd[i], 1);
            do {
                nb = read(port, &byte, 1);
            } while (nb == -1 && byte != cmd[i]);
            byte = 0;
        }

        if (!needResponse) {
            usleep(500000);
            while (-1 != read(port, &byte, 1));
        }
    } else {
        ret = STATUS_ERROR;
    }

    return ret;
}

string XV25::receive(void)
{
    string response;
    uint8_t byte;
    int nb;
    bool gotEOF = false;

    if (0 < port) {
        while (-1 == read(port, &byte, 1))
            usleep(1000);
        response += (char)byte;

        do {
            nb = read(port, &byte, 1);

            if (nb >= 0) {
                if (0 == byte)
                    gotEOF = true;
                else
                    response += (char)byte;
            }
        } while (nb >= 0);
        
        if (!gotEOF) {
            for (uint32_t i = 0; i < 1000; i++) {
                usleep(1000);
                read(port, &byte, 1);
            }
        }
    } else {
        cerr << "Failed to read from \"" << portName << "\"" << endl;
    }

    return response;
}

status_t XV25::command(string cmd)
{
    return send(cmd+"\r\n", false);
}

status_t XV25::commandWithResponse(string cmd, string *response)
{
    status_t ret = STATUS_OK;

    if (STATUS_OK == send(cmd+"\r\n", true))
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
    string cmd = "TestMode ";
    cmd += (testModeOn == testMode) ? "On" : "Off";
    return command(cmd);
}

status_t XV25::setMotor(motor_t motor, int speed, int distance)
{
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
        ret = command(cmd);
    }

    return ret;
}

status_t XV25::getPosition(motor_t motor, int* position)
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
        string tmp = "PositionInMM,";        
        size_t pos = result.find(tmp);
        pos += tmp.size();
        size_t end = result.substr(pos).find('\n');
        *position = atoi(result.substr(pos, end).c_str());
    }

    return ret;
}

status_t XV25::getBatteryLevel(int* battery)
{
    status_t ret = STATUS_OK;
    string cmd = "getCharger";
    string result;
    
    if (STATUS_OK == ret)
        ret = commandWithResponse(cmd, &result);

    if (STATUS_OK == ret) {
        string tmp = "FuelPercent,";        
        size_t pos = result.find(tmp);
        pos += tmp.size();
        size_t end = result.substr(pos).find('\n');
        *battery = atoi(result.substr(pos, end).c_str());
    }

    return ret;
}

status_t XV25::startLDS()
{
    string cmd = "SetLDSRotation On";
    return command(cmd);
}

status_t XV25::stopLDS()
{
    string cmd = "SetLDSRotation Off";
    return command(cmd);
}

status_t XV25::getLDSScan(ldsScan_t */*scan*/)
{
    status_t ret = STATUS_OK;
    string cmd = "getLDSScan";
    string result;
    
    if (STATUS_OK == ret)
        ret = commandWithResponse(cmd, &result);
    
    if (STATUS_OK == ret) {
        cerr << "+++++++++++++++++++++++++" << endl;
        cerr << result << endl;
        cerr << "+++++++++++++++++++++++++" << endl;
     }
    
    return ret;
}
