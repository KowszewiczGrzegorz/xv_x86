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


status_t XV25::send(string cmd)
{
    status_t ret = STATUS_OK;

    cerr << "Sending command \"" << (cmd.substr(0, cmd.size()-1)) << "\"" << endl;

    if (port > 0) {
        for (uint8_t i = 0; i < cmd.size(); i++)
            write(port, &cmd[i], 1);
        /*
        uint32_t nbBytes = 0;
        string bytes;
        do {
            bytes = cmd.substr(nbBytes, cmd.size());
            nbBytes += write(port, bytes.c_str(), bytes.size());
        } while (nbBytes < cmd.size());        
        */
    } else {
        ret = STATUS_ERROR;
    }

    return ret;
}

string XV25::receive(void)
{
    string response;
    uint8_t byte;

    do {
        read(port, &byte, 1);
    } while (0 == byte);
    response += (char)byte;

    cerr << "read " << byte << endl;

    if (0 < port) {
        do {
            read(port, &byte, 1);        

            cerr << "read " << byte << endl;

            if (0 != byte)
                response += (char)byte;
        } while (0 != byte);
        
        if (response.size() > 0)
            response = response.substr(0, response.size()-1);
        for (uint32_t i = 0; i < 1000; i++) {
            usleep(1000);
            read(port, &byte, 1);
        }
        /*
        uint8_t nbBytes;
        uint8_t nbBytesTotal = 0;
        char *bytes = (char*) malloc (64 * sizeof(char));
        do {
            nbBytes = read(port, &bytes, 63);
            if (nbBytes > 0) {
                nbBytesTotal += nbBytes;
                for (uint8_t i = 0; i < nbBytes; i++)
                    response += bytes[i];
            }
        } while (nbBytes > 0);
        */
    } else {
        cerr << "Failed to read from \"" << portName << "\"" << endl;
    }

    return response;
}

void XV25::getEof(void)
{
    uint8_t byte;

    if (port > 0) {
        do {
            read(port, &byte, 1);
            // Look at multiple byte reading
        } while (0 == byte);
    }

    cerr << "getEof() : \"" << byte << "\" (should be EOF=\"" << EOF << "\")" << endl;
}

status_t XV25::command(string cmd)
{
    status_t ret;

    if (STATUS_OK == (ret = send(cmd+"\n"))) 
        getEof();
    
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
