#include <iostream>
#include <string>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "pointsLibrary.hh"

using namespace std;

#ifndef __XV25_H__
#define __XV25_H__

static const int STRING_SIZE = 64;

/* Hash codes */
static const uint32_t GET_VERSION = 1746928034U;
static const uint32_t GET_BATTERY_LEVEL = 1971829418U;
static const uint32_t SET_TEST_MODE = 3587361541U;
static const uint32_t SET_MOTORS = 1037014863U;
static const uint32_t GET_POSITIONS = 2247116388U;
static const uint32_t START_LDS = 3406550930U;
static const uint32_t STOP_LDS = 1882252425U;

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

typedef struct {
    int distInMM[360];
    int intensity[360];
    int errorCode[360];
    double rotationFrequency;
} ldsScan_t;

class XV25 {
public:
    XV25(string);
    ~XV25();

    status_t connect();
    void disconnect();

    status_t getVersion(string*);
    status_t setTestMode(testMode_t);

    status_t setMotor(motor_t, int, int);
    status_t setMotors(int, int, int, int);
    status_t getPosition(motor_t, int*);
    status_t getPositions(int*, int*);
    status_t getVelocities(int*, int*);

    status_t getBatteryLevel(int*);

    status_t startLDS();
    status_t stopLDS();
    status_t getLDSScan(ldsScan_t*);

    uint32_t getDistanceAtAngle(ldsScan_t*, uint32_t);

    string interpretCommand(string cmd);

private:
    status_t command(string);
    status_t commandWithResponse(string, string*);

    status_t send(string);
    string receive();
 
    status_t getLDSMesure(string, int*, int*, int*, int*);

    int port;
    string portName;
};


#endif /* __XV25_H__ */
