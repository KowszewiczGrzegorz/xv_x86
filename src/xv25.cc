#include <stdlib.h>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include "hash.hh"
#include "xv25.hh"

static const uint32_t bufferSize = 1024;


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

    flush();

    return ret;
}

void XV25::disconnect()
{
    if (port > 0)
        close(port);
}

status_t XV25::send(string cmd)
{
    status_t ret = STATUS_OK;
    char bytes[bufferSize];
    int nbWrite, nbRead;
    uint32_t nbSent = 0;

    while (-1 != read(port, &bytes, bufferSize))
        usleep(10);
    
    if (port > 0) {
        while (nbSent < cmd.size()) {
            nbWrite = write(port, cmd.substr(nbSent, bufferSize-nbSent).c_str(), 
                            min((size_t)bufferSize, (cmd.substr(nbSent, bufferSize-nbSent).size())));

            nbSent += nbWrite;

            do {
                usleep(10);
                nbRead = read(port, &bytes, nbWrite);
            } while (nbRead == -1 || nbWrite != nbRead);
            bytes[0] = 0;
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
    uint8_t bytes[bufferSize];
    int nb;
    bool gotEOF = false;

    if (0 < port) {
        do {
            nb = read(port, &bytes, bufferSize);
            usleep(1);
        } while (-1 == nb);
        bytes[nb] = '\0';
        response += (char*)bytes;
        for (int i = 0; i < nb && !gotEOF; i++)
            if (0 == bytes[i] || 26 == bytes[i]) 
                gotEOF = true;
                
        if (!gotEOF) {
            do {
                nb = read(port, &bytes, bufferSize-1);

                if (nb >= 0) {
                    usleep(1);
                    for (int i = 0; i < nb && !gotEOF; i++)
                        if (0 == bytes[i] || 26 == bytes[i])
                            gotEOF = true;
                    bytes[nb] = '\0';
                    response += (char*)bytes;
                } 

            } while (nb >= 0 || !gotEOF);
        }

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

    return response;
}

status_t XV25::flush()
{
    status_t ret = STATUS_OK;

    if (STATUS_OK == send("\n")) {
        uint8_t bytes[bufferSize];
        int nb; 
        usleep(500000);
        do {
            nb = read(port, &bytes, bufferSize);
            usleep(1);
        } while (-1 != nb);
       
    } else {
        ret = STATUS_ERROR;
    }

    return ret;
}

status_t XV25::command(string cmd)
{
    return send(cmd+"\n");
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

status_t XV25::setMotors(int lDist, int rDist, int speed, int acceleration)
{
    status_t ret = STATUS_OK;;
    string cmd = "SetMotor ";

    ostringstream oss;
    oss << " LWheelDist " << lDist;
    oss << " RWheelDist " << rDist; 
    oss << " Speed " << speed;
    oss << " Accel " << acceleration;
    cmd += oss.str();
    ret = command(cmd);

    return ret;
}

status_t XV25::getPosition(motor_t motor, int* position)
{
    status_t ret = STATUS_OK;
    string cmd = "GetMotors ";
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

status_t XV25::getPositions(int* leftPos, int* rightPos)
{
    status_t ret = STATUS_OK;
    string cmd = "GetMotors";
    string result;

    if (STATUS_OK == ret)
        ret = commandWithResponse(cmd, &result);

    if (STATUS_OK == ret) {
        string tmp = "LeftWheel_PositionInMM,";        
        size_t pos = result.find(tmp);
        pos += tmp.size();
        size_t end = result.substr(pos).find('\n');
        *leftPos = atoi(result.substr(pos, end).c_str());

        tmp = "RightWheel_PositionInMM,";        
        pos = result.find(tmp);
        pos += tmp.size();
        end = result.substr(pos).find('\n');
        *rightPos = atoi(result.substr(pos, end).c_str());
    }

    return ret;
}

status_t XV25::getVelocities(int* leftVel, int* rightVel)
{
    status_t ret = STATUS_OK;
    string cmd = "GetMotors";
    string result;

    if (STATUS_OK == ret)
        ret = commandWithResponse(cmd, &result);

    if (STATUS_OK == ret) {
        string tmp = "LeftWheel_Velocity,";
        size_t pos = result.find(tmp);
        pos += tmp.size();
        size_t end = result.substr(pos).find('\n');
        *leftVel = atoi(result.substr(pos, end).c_str());

        tmp = "RightWheel_Velocity,";
        pos = result.find(tmp);
        pos += tmp.size();
        end = result.substr(pos).find('\n');
        *rightVel = atoi(result.substr(pos, end).c_str());
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
        
        int subIndex = result.find_first_of("ROTATION_SPEED,");
        string sub = result.substr(subIndex, result.size()-subIndex);

        //        cerr << "Rotation line : \"" << sub << "\"" << endl;

        sub = sub.substr(16, sub.size()-16);

        // cerr << "Rotation line (updated) : \"" << sub << "\"" << endl;

        scan->rotationFrequency = atof(sub.c_str());
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

string XV25::interpretCommand(string command)
{
    string cmd, response, tmp;
    vector<string> parameters;
    unsigned long int start, end;

    if (string::npos != (end = command.find_first_of(' '))) {
        cmd = command.substr(0, end-1);
        start = end+1;

        do {
            end = command.substr(start).find_first_of(' ');
            if (string::npos != end)
                tmp = command.substr(start, end-start);
            else
                tmp = command.substr(start);
            parameters.push_back(tmp);
        } while (string::npos != end);
    } else {
        cmd = command;
    }

    cerr << "interpret command (" << cmd << ") --> hash=\"" << hash(cmd.c_str()) << "\"" << endl;

    switch(hash(cmd.c_str())) {
        case GET_VERSION:
            // response += "getVersion";
            cerr << "This is a get version" << endl;
            getVersion(&response);
            cerr << "received from xv \"" << response << "\"" << endl;
            break;
        case GET_BATTERY_LEVEL:
            response += "getBatteryLevel";
            /*
            int batteryLevel;
            getBatteryLevel(&batteryLevel);
            response += batteryLevel;
            */
            break;
        case SET_TEST_MODE:
            response += "setTestMode(" + parameters[0] + ")";
            /*
            if (0 == parameters[0].compare("On"))
                setTestMode(testModeOn);
            if (0 == parameters[0].compare("Off"))
                setTestMode(testModeOff);
            break;
            */
        case SET_MOTORS:
            if (4 == parameters.size()) {
                response += "setMotors(" + parameters[0] + ", " + parameters[1] +
                    + ", " + parameters[2] + ", " + parameters[3] + ")";
                /*
                int leftDist = atoi(parameters[0].c_str());
                int rightDist = atoi(parameters[1].c_str());
                int speed = atoi(parameters[2].c_str());
                int accel = atoi(parameters[3].c_str());
                setMotors(leftDist, rightDist, speed, accel);
                */
            } else {
                cerr << "Wrong number of parameters for SetMotors !" << endl;
            }
            break;
        case GET_POSITIONS:
            response += "getPositions";
            /*
            int left, right;
            getPositions(&left, &right);
            response += left + " " + right;
            */
            break;
        case START_LDS:
            response += "startLDS";
            // startLDS();
            break;
        case STOP_LDS:
            response += "stopLDS";
            // stopLDS();
            break;
        default:;
    }

    return response;
}
