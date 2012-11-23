#include <iostream>
#include <string>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

using namespace std;

static const int STRING_SIZE = 64;

typedef enum {
    STATUS_OK,
    STATUS_ERROR
} status_t;

typedef enum {
    testModeOn,
    testModeOff
} testMode_t;

typedef enum {
    leftWheel,
    rightWheel
} motor_t;
    

class XV25 {
public:
    XV25(string);
    ~XV25();

    status_t connect();
    void disconnect();

    status_t getVersion(string*);
    status_t setTestMode(testMode_t);

    status_t setMotor(motor_t, int, int);
    status_t getEncoder(motor_t);

private:
    status_t command(string);
    status_t commandWithResponse(string, string*);

    status_t send(string);
    string receive();
    void getEof();

    int port;
    string portName;
};
