#include <stdlib.h>
#include <sstream>
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
    struct termios toptions;

    port = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (port <= 0) {
        cerr << "Failed to open \"" << portName << "\"" << endl;
        ret = STATUS_ERROR;
    }

    if (STATUS_OK == ret) {
        if (tcgetattr(port, &toptions) < 0) {
            cerr << "Couldn't get term attributes" << endl;
            ret = STATUS_ERROR;
        } else {
            speed_t brate = B9600;
            cfsetispeed(&toptions, brate);
            cfsetospeed(&toptions, brate);

            // 8N1
            toptions.c_cflag &= ~PARENB;
            toptions.c_cflag &= ~CSTOPB;
            toptions.c_cflag &= ~CSIZE;
            toptions.c_cflag |= CS8;
            // no flow control
            toptions.c_cflag &= ~CRTSCTS;

            toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
            toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

            toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
            toptions.c_oflag &= ~OPOST; // make raw

            // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
            toptions.c_cc[VMIN]  = 0;
            toptions.c_cc[VTIME] = 20;
    
            if (tcsetattr(port, TCSANOW, &toptions) < 0) {
                cerr << "Couldn't set term attributes" << endl;
                ret = STATUS_ERROR;
            }
        }
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
    int size = cmd.size();

    if (0 >= port || size == write(port, cmd.c_str(), size)) {
        ret = STATUS_ERROR;
        cerr << "Failed to write \"" << cmd << "\" to \"" << portName << "\"" << endl;
    }

    return ret;
}

string XV25::receive(void)
{
    string response;
    char b[1];
    int n;

    if (0 < port) {
        do { 
            n = read(port, b, 1);
            if (-1 == n) {
                cerr << "Failed to read from \"" << portName << "\", already got \"" << response << "\"" << endl;
                break;
            }
            if (0 == n) {
                usleep(10 * 1000);
                continue;
            }
            if (EOF != b[0])
                response += b[0]; 
        } while (EOF != b[0]);
    } else {
        cerr << "Failed to read from \"" << portName << "\"" << endl;
    }

    return response;
}

status_t XV25::getVersion(string* version)
{
    status_t ret = STATUS_OK;

    if (STATUS_OK == send("GetVersion"))
    	*version = receive();
    else
        ret = STATUS_ERROR;

    return ret;
}

status_t XV25::setTestMode(testMode_t testMode)
{
    status_t ret;
    string cmd = "TestMode ";
    cmd += (testModeOn == testMode) ? "On" : "Off";

    ret = send(cmd);

    return ret;
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
        ret = send(cmd);
    }

    return ret;
}

status_t XV25::getEncoder(motor_t motor)
{
    status_t ret = STATUS_OK;
    string cmd = "getMotor ";

    switch (motor) {
        case leftWheel: cmd += "LeftWheel"; break;
        case rightWheel: cmd += "RightWheel"; break;
        default: ret = STATUS_ERROR;
    }
    
    if (STATUS_OK == ret)
        ret = send(cmd);

    if (STATUS_OK == ret) {
        string encoderString = "Encoder,";
        string result = receive();
        
        size_t pos = result.find(encoderString);
        pos += encoderString.size();
        size_t end = result.substr(pos).find('\n');
        atoi(result.substr(pos, end).c_str());
    }

    return ret;
}
