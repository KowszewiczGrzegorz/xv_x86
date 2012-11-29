#include <math.h>
#include <sys/time.h>
#include <stdint.h>
#include <signal.h>
#include "xv25.hh"
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
    
    double lSpeed, rSpeed, dist, dist2;
    ldsScan_t scan;
    timestamp_t t0, t1;
    int lWheelDist, rWheelDist;
    position_t pos;

    while (!signalCatched) {
        t0 = get_timestamp();
        xv25->getLDSScan(&scan);
        
        dist = xv25->getDistanceAtAngle(&scan, 90);
        if (0 != dist) {
            lSpeed = 100 + min((dist/fabs(dist))*70.0, (500.0 - dist)/3);
            rSpeed = 100 - min((dist/fabs(dist))*70.0, (500.0 - dist)/3);
        }

        dist2 = xv25->getDistanceAtAngle(&scan, 45);
        if (dist < 600 && dist2 < 800 && dist2 != 0) {
            lSpeed += min(40.0, (800-dist2)/3);
            rSpeed -= min(40.0, (800-dist2)/3);
        }

        xv25->setMotors(lSpeed, rSpeed, 200, 100);

        xv25->getPositions(&lWheelDist, &rWheelDist);
        odometry->update(lWheelDist, rWheelDist);

        t1 = get_timestamp();

        cerr << "dist = " << dist << ", dist2 = " << dist2 << " => cmd motor = [" << lSpeed << ", ";
        cerr << rSpeed << "] in " << ((t1-t0)/1000.0L) << "ms";
        pos = odometry->getCurrentPosition();
        cerr << " in pos [" << pos.x << ", " << pos.y << ", " << pos.theta << "]" << endl;
        
        while ((get_timestamp()-t0) < 100000.0L) {
            usleep(1000);
            xv25->getPositions(&lWheelDist, &rWheelDist);
            odometry->update(lWheelDist, rWheelDist);
            cerr << ".";
            // pos = odometry->getCurrentPosition();
            // cerr << " in pos [" << pos.x << ", " << pos.y << ", " << pos.theta << "]" << endl;
        }
        cerr << endl;
    }

    xv25->stopLDS();
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


void sighandler(int sig)
{
    cout << "Signal " << sig << " caught..." << endl;
    signalCatched = true;
}


int main (void)
{
    XV25 *xv25 = new XV25(portName);
    Odometry *odometry = new Odometry(300.0);

    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT, &sighandler);

    if (STATUS_ERROR == xv25->connect()) {
        cerr << "Failed to connect to \"" << portName << "\"" << endl;
	return -1;
    }

    fastWallFollower(xv25, odometry);
    /*
    int pos;
    xv25->getPosition(leftWheel, &pos);
    cerr << "LeftWheel position : " << pos << endl;
    */
    xv25->disconnect();

    cerr << "End of program" << endl;
    
    return 0;
}
