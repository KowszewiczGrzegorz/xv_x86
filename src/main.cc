#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <signal.h>
#include <stdlib.h>
#include "xv25.hh"
#include "webApi.hh"
#include "odometry.hh"
#include "pointsLibrary.hh"

static const string portName = "/dev/ttyACM0";

typedef unsigned long long timestamp_t;
static timestamp_t get_timestamp () {
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

static bool signalCatched = false;

void getVersion(XV25 *xv25) 
{
    string version;
    xv25->getVersion(&version);
    cout << "XV25 version " << endl;
    cout << "---------------------------------------" << endl;
    cout << version << endl;
    cout << "---------------------------------------" << endl;
}

void getBatteryLevel(XV25 *xv25)
{
    int battery;
    xv25->getBatteryLevel(&battery);
    cerr << "Battery level : " << battery << "%" << endl;
}

void getPosition(XV25 *xv25) 
{
    int position;

    xv25->setTestMode(testModeOn);
    xv25->getPosition(leftWheel, &position);
    cerr << "Position de demarrage: " << position << endl;

    xv25->setMotor(leftWheel, 300, 1000);

    for (int i = 0; i < 7; i++) {
        sleep(1);
        xv25->getPosition(leftWheel, &position);
        cerr << "Position (" << i << "): " << position << endl;
    }

    xv25->setTestMode(testModeOff);
}

void wallFollower(XV25 *xv25) 
{
    xv25->setTestMode(testModeOn);
    xv25->startLDS();
    sleep(2);
    
    double lSpeed, rSpeed, dist;
    ldsScan_t scan;
    timestamp_t t0, t1;
    while (!signalCatched) {
        t0 = get_timestamp();
        xv25->getLDSScan(&scan);
        
        dist = xv25->getDistanceAtAngle(&scan, 70);
        lSpeed = 50 + (400.0 - dist)/3;
        rSpeed = 50 - (400.0 - dist)/3;
        xv25->setMotors(lSpeed, rSpeed, 30, 30);
        t1 = get_timestamp();

        cerr << "dist = " << dist << " => cmd motor = [" << lSpeed << ", ";
        cerr << rSpeed << "] in " << ((t1-t0)/1000.0L) << "ms" << endl;
        
        while ((get_timestamp()-t0) < 500000.0L)
            usleep(100);
    }

    xv25->stopLDS();
    xv25->setTestMode(testModeOff);
}

void fastWallFollower(XV25 *xv25, Odometry *odometry) 
{
    xv25->setTestMode(testModeOn);
    xv25->startLDS();
    sleep(2);

    odometry->init();
    
    double lDist, rDist, dist, dist2;
    ldsScan_t scan;
    timestamp_t t0, t1;
    int lWheelDist, rWheelDist;

    while (!signalCatched) {
        t0 = get_timestamp();
        xv25->getLDSScan(&scan);
        
        dist = xv25->getDistanceAtAngle(&scan, 90);
        if (0 != dist) {
            lDist = 100 + min((dist/fabs(dist))*70.0, (500.0 - dist)/3);
            rDist = 100 - min((dist/fabs(dist))*70.0, (500.0 - dist)/3);
        } else {
            lDist = rDist = 0;
        }

        dist2 = xv25->getDistanceAtAngle(&scan, 45);
        if (dist < 600 && dist2 < 800 && dist2 != 0) {
            lDist += min(40.0, (800-dist2)/3);
            rDist -= min(40.0, (800-dist2)/3);
        }

        xv25->setMotors(lDist, rDist, 200, 100);

        xv25->getPositions(&lWheelDist, &rWheelDist);
        odometry->update(lWheelDist, rWheelDist);

        t1 = get_timestamp();

        cerr << "dist = " << dist << ", dist2 = " << dist2 << " => cmd motor = [" << lDist << ", ";
        cerr << rDist << "] in " << ((t1-t0)/1000.0L) << "ms";
        odometry->printPosition(" -- ");
        
        while ((get_timestamp()-t0) < 100000.0L) {
            usleep(1000);
            xv25->getPositions(&lWheelDist, &rWheelDist);
            odometry->update(lWheelDist, rWheelDist);
            cerr << ".";
        }
        cerr << endl;
    }

    xv25->stopLDS();
    xv25->setTestMode(testModeOff);
}

void waitEndOfMovement (XV25* xv25, Odometry* odometry)
{
    int leftVel, rightVel;
    int leftPos, rightPos;
    bool movementStarted = false;

    do {
        xv25->getPositionsAndVelocities(&leftPos, &rightPos, &leftVel, &rightVel);
        odometry->update(leftPos, rightPos);

        if (0 != leftVel || 0 != rightVel)
            movementStarted = true;

        if (signalCatched)
            break;

    } while (!movementStarted || 0 != leftVel || 0 != rightVel);
}

void testOdometry (XV25* xv25, Odometry* odometry)
{
    xv25->setTestMode(testModeOn);
    odometry->init();
    usleep(500000);

    odometry->printPosition("[INITIAL] ");

    for (uint32_t i = 0; i < 4; i++) {
        // straight line
        xv25->setMotors(500, 500, 100, 100);
        usleep(10000);
        waitEndOfMovement(xv25, odometry);
        
        odometry->printPosition("[LINE] ");

        if (signalCatched)
            break;

        // quarter turn
        xv25->setMotors(190, -190, 100, 100);
        usleep(10000);
        waitEndOfMovement(xv25, odometry);

        odometry->printPosition("[TURN/4] ");

        if (signalCatched)
            break;
    }

    odometry->printPosition("[END] ");

    xv25->setTestMode(testModeOff);
}

void testFullTurn (XV25* xv25, Odometry* odometry)
{
    xv25->setTestMode(testModeOn);
    odometry->init();
    usleep(500000);

    odometry->printPosition("[INITIAL] ");

    xv25->setMotors(764, -764, 100, 100);
    usleep(10000);
    waitEndOfMovement(xv25, odometry);
        
    odometry->printPosition("[END] ");

    position_t pos = odometry->getCurrentPosition();
    double dTheta = pos.theta - 2*3.1415926535;
    cerr << " erreur d'angle = " << dTheta << endl;

    xv25->setTestMode(testModeOff);
}

void ropeAlgorithm (XV25 *xv25) 
{
    PointsLibrary *pl = new PointsLibrary();
    FILE *fp = fopen("scan.csv", "w");
    vector<point_t> points;
    point_t p;
    ldsScan_t scan;

    xv25->setTestMode(testModeOn);
    sleep(1);

    xv25->startLDS();
    sleep(3);

    xv25->getLDSScan(&scan);

    xv25->stopLDS();
    xv25->setTestMode(testModeOff);

    if (NULL != fp) {
        for (int i = 0; i < 359; i++) {
            if (0 < scan.distInMM[i] && scan.distInMM[i] < 1000.0) {
                p = pl->p2c(i, scan.distInMM[i]);
                if (p.y > 0) {
                    points.push_back(p);
                    fprintf(fp, "%g\t%g\n", p.x, p.y);
                }
            }
        }
        fclose(fp);
    }

    pl->setMaxDistanceRopeAlgorithm(50.0);

    vector <vector<point_t> > clusters = pl->scanSegmentation(points, 50.0);
    for (uint32_t i = 0; i < clusters.size(); i++) {
        vector<line_t> lines = pl->ropeAlgorithm(clusters[i]);
        cerr << "Cluster " << i << endl;
        for (uint32_t i = 0; i < lines.size(); i++)
            cerr << "   Line " << i << " = " << lines[i].a << "*x + " << lines[i].b << "*y + " << lines[i].c << endl;
    }
}


void webTest(XV25* xv25, int portNumber)
{
    WebApi *webApi = new WebApi(xv25, portNumber);
    if (STATUS_OK == webApi->getStatus())
        webApi->run(&signalCatched);
}


void sighandler(int sig)
{
    cout << "Signal " << sig << " caught..." << endl;
    signalCatched = true;
}


//int main (int argc, char *argv[])
int main (void)
{
    XV25 *xv25 = new XV25(portName);

    // Signal stuff
    struct sigaction act;
    act.sa_handler = sighandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
    sigaction(SIGABRT, &act, 0);
    sigaction(SIGTERM, &act, 0);

    if (STATUS_ERROR == xv25->connect()) {
        cerr << "Failed to connect to \"" << portName << "\"" << endl;
	return -1;
    }

    /*
    if (2 != argc) {
        cerr << "Need 1 argument !" << endl;
        return -1;
    } else {
        webTest(xv25, atoi(argv[1]));
    }
    */

    /*
    string version;
    xv25->getVersion(&version);
    */

    Odometry *odometry = new Odometry(242.0);
    testOdometry(xv25, odometry);
    //testFullTurn(xv25, odometry);

    xv25->disconnect();

    cerr << "End of program" << endl;
    
    return 0;
}
