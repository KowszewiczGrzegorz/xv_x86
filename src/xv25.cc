#include <stdlib.h>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "xv25.hh"

static const uint32_t bufferSize = 64;


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

    cfsetospeed(&tio, B921600);
    cfsetispeed(&tio, B921600);
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

    while (-1 != read(port, &byte, 1))
        usleep(10);

    cerr << "Sending command \"" << (cmd.substr(0, cmd.size()-2)) << "\"" << endl;

    if (port > 0) {
        for (uint8_t i = 0; i < cmd.size(); i++) {
            write(port, &cmd[i], 1);
            do {
                usleep(10);
                nb = read(port, &byte, 1);
            } while (nb == -1 || byte != cmd[i]);
            byte = 0;
            usleep(10);
        }
        cerr << endl;

        if (!needResponse) {
            usleep(10);
            while (-1 != read(port, &byte, 1) || 0 == byte)
                usleep(10);
        }
    } else {
        ret = STATUS_ERROR;
    }

    return ret;
}

status_t XV25::sendMultiple(string cmd, bool needResponse)
{
    status_t ret = STATUS_OK;
    char bytes[bufferSize];
    int nbWrite, nbRead;
    uint32_t nbSent = 0;

    while (-1 != read(port, &bytes, bufferSize))
        usleep(10);

    cerr << "Sending command \"" << (cmd.substr(0, cmd.size()-2)) << "\"" << endl;

    if (port > 0) {
        while (nbSent < cmd.size()) {
            nbWrite = write(port, cmd.substr(nbSent, bufferSize-nbSent).c_str(), bufferSize);

            cerr << "    sent \"" << (cmd.substr(nbSent, nbWrite)) << "\"" << endl;

            nbSent += nbWrite;

            do {
                usleep(10);
                nbRead = read(port, &bytes, bufferSize);

                cerr << "    read " << nbRead << " bytes << : \"" << bytes << "\"" << endl;
            
            } while (nbRead == -1 || nbWrite != nbRead || bytes[nbRead-1] != cmd[nbSent]);
            bytes[0] = 0;
            usleep(10);
        }

        if (!needResponse) {
            usleep(10);
            do {
                nbRead = read(port, &bytes, 1);

                cerr << "    read(flush) " << nbRead << " bytes << : \"" << bytes << "\"" << endl;

            } while (-1 == nbRead || 0 == bytes[nbRead-1]);
            usleep(10);
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
            usleep(1);
        response += (char)byte;

        do {
            nb = read(port, &byte, 1);

            if (nb >= 0) {
                usleep(1);
                if (0 == byte)
                    gotEOF = true;
                else
                    response += (char)byte;
            } 

        } while (nb >= 0);
        
        if (!gotEOF) {
            for (uint32_t i = 0; i < 1000; i++) {
                usleep(1);
                read(port, &byte, 1);
            }
        }
    } else {
        cerr << "Failed to read from \"" << portName << "\"" << endl;
    }

    /*
    cerr << "Read ++++++++++++++++++" << endl;
    cerr << response << endl;
    cerr << "+++++++++++++++++++++++" << endl;
    */

    return response;
}

string XV25::receiveMultiple(void)
{
    string response;
    uint8_t bytes[bufferSize];
    int nb;
    bool gotEOF = false;

    cerr << "ReceiveMultiple()" << endl;

    if (0 < port) {
        do {
            nb = read(port, &bytes, bufferSize);
            usleep(1);
        } while (-1 == nb);
        bytes[nb] = '\0';
        response += (char*)bytes;

        cerr << "    received " << nb << " bytes : \"" << ((char*)bytes) << "\"" << endl;

        do {
            nb = read(port, &bytes, bufferSize-1);

            if (nb >= 0) {
                usleep(1);
                if (0 == bytes[nb-1])
                    gotEOF = true;
                bytes[nb] = '\0';
                response += (char*)bytes;

                cerr << "    received " << nb << " bytes : \"" << ((char*)bytes) << "\"" << endl;
            } 

        } while (nb >= 0);
        
        if (!gotEOF) {
            for (uint32_t i = 0; i < 1000 && !gotEOF; i++) {
                usleep(1);
                nb = read(port, &bytes, bufferSize-1);
                if (-1 != nb && 0 == bytes[nb-1])
                    gotEOF = true;
            }
        }
    } else {
        cerr << "Failed to read from \"" << portName << "\"" << endl;
    }

    cerr << "Read ++++++++++++++++++" << endl;
    cerr << response << endl;
    cerr << "+++++++++++++++++++++++" << endl;

    return response;
}

status_t XV25::command(string cmd)
{
    return sendMultiple(cmd+"\r\n", false);
}

status_t XV25::commandWithResponse(string cmd, string *response)
{
    status_t ret = STATUS_OK;

    if (STATUS_OK == sendMultiple(cmd+"\r\n", true))
    	*response = receiveMultiple();
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

status_t XV25::setMotors(int lDist, int rDist, int speed)
{
    status_t ret = STATUS_OK;;
    string cmd = "SetMotor ";

    ostringstream oss;
    oss << " LWheelDist " << lDist;
    oss << " RWheelDist " << rDist; 
    oss << " Speed " << speed;
    cmd += oss.str();
    ret = command(cmd);

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

status_t XV25::getLDSMesure(string line, int* angle, int* distance, int* intensity, int* error)
{
    status_t ret = STATUS_OK;
    int start = 0, current;

    current = line.find_first_of(',');

    *angle = atoi(line.substr(0, current).c_str());
    start += current + 1;

    current = line.substr(start).find_first_of(',');
    *distance = atoi(line.substr(start, current).c_str());
    start += current + 1;

    current = line.substr(start).find_first_of(',');
    *intensity = atoi(line.substr(start, current).c_str());
    start += current + 1;

    *error = atoi(line.substr(start, line.size()-1).c_str());

    return ret;
}

status_t XV25::getLDSScan(ldsScan_t *scan)
{
    status_t ret = STATUS_OK;
    string cmd = "getLDSScan";
    string result;
    result.reserve(4000);
    
    if (STATUS_OK == ret)
        ret = commandWithResponse(cmd, &result);
    
    if (STATUS_OK == ret) {
        int start = result.find_first_of('\n') + 1;
        int current;
        int angle, distance, intensity, error;
        do {
            current = result.substr(start).find_first_of('\n');
            getLDSMesure(result.substr(start, current-1), &angle, &distance, &intensity, &error);
            if (0 <= angle && angle < 360) {
                scan->distInMM[angle] = distance;
                scan->intensity[angle] = intensity;
                scan->errorCode[angle] = error;
            }
            start += current + 1;
        } while (angle < 359);
     }
    
    return ret;
}


uint32_t XV25::getDistanceAtAngle (ldsScan_t* scan, uint32_t angle)
{
    uint32_t minI = max((uint32_t)0, angle-2);
    uint32_t maxI = min(angle+2, (uint32_t)259);
    double average = 0;
    uint32_t cnt = 0;
    for (uint32_t i = minI; i < maxI; i++) {
        average += scan->distInMM[minI];
        cnt++;
    }
    return (average / cnt);    
}
