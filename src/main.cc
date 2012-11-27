#include <math.h>
#include <stdint.h>
#include "xv25.hh"
#include "pointsLibrary.hh"

static const string portName = "/dev/ttyACM0";


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
    sleep(1);

    xv25->startLDS();
    sleep(3);
    
    double lSpeed, rSpeed;
    ldsScan_t scan;
    int t = 0;
    while (t < 500) {
        xv25->getLDSScan(&scan);
        
        double dist = xv25->getDistanceAtAngle(&scan, 70);
        lSpeed = 50 + (400.0 - dist)/3;
        rSpeed = 50 - (400.0 - dist)/3;

        cerr << "dist = " << dist << " => cmd motor = [" << lSpeed << ", " << rSpeed << "]" << endl;
        xv25->setMotors(lSpeed, rSpeed, 30, 30);

        t++;
    }

    xv25->stopLDS();
    xv25->setTestMode(testModeOff);
}

void fastWallFollower(XV25 *xv25) 
{
    xv25->setTestMode(testModeOn);
    sleep(1);

    xv25->startLDS();
    sleep(3);
    
    double lSpeed, rSpeed;
    ldsScan_t scan;
    int t = 0;
    while (t < 500) {
        xv25->getLDSScan(&scan);
        
        double dist = xv25->getDistanceAtAngle(&scan, 70);
        lSpeed = 120 + (400.0 - dist)/3;
        rSpeed = 120 - (400.0 - dist)/3;

        cerr << "dist = " << dist << " => cmd motor = [" << lSpeed << ", " << rSpeed << "]" << endl;
        xv25->setMotors(lSpeed, rSpeed, 100, 50);

        t++;
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


int main (void)
{
    XV25 *xv25 = new XV25(portName);

    if (STATUS_ERROR == xv25->connect()) {
        cerr << "Failed to connect to \"" << portName << "\"" << endl;
	return -1;
    }

    xv25->setTestMode(testModeOn);
    xv25->startLDS();
    sleep(2);

    ldsScan_t scan;
    xv25->getLDSScan(&scan);

    xv25->stopLDS();
    xv25->setTestMode(testModeOff);

    xv25->disconnect();

    cerr << "End of program" << endl;
    
    return 0;
}
