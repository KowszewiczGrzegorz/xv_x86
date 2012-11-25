#include <math.h>
#include <stdint.h>
#include "xv25.hh"
#include "pointsLibrary.hh"

static const string portName = "/dev/ttyACM0";

int main (void)
{
    XV25 *xv25 = new XV25(portName);
    PointsLibrary *pl = new PointsLibrary();
    string version;

    if (STATUS_ERROR == xv25->connect()) {
        cerr << "Failed to connect to \"" << portName << "\"" << endl;
	return -1;
    }

    
    xv25->getVersion(&version);
    cout << "XV25 version " << endl;
    cout << "---------------------------------------" << endl;
    cout << version << endl;
    cout << "---------------------------------------" << endl;
    

    /*
    int position;
    xv25->getPosition(leftWheel, &position);
    cerr << "Position de demarrage: " << position << endl;

    xv25->setMotor(leftWheel, 300, 1000);

    for (int i = 0; i < 7; i++) {
        sleep(1);
        xv25->getPosition(leftWheel, &position);
        cerr << "Position (" << i << "): " << position << endl;
    }

    xv25->setTestMode(testModeOff);
    */

    int battery;
    xv25->getBatteryLevel(&battery);
    cerr << "Battery level : " << battery << "%" << endl;

    usleep(1000);
    xv25->setTestMode(testModeOn);
    usleep(1000);

    ldsScan_t scan;
    xv25->startLDS();
    sleep(3);

    double lSpeed, rSpeed;
    double dist90;
    // double dist45;
    int t = 0;;
    while (t < 1) {
        xv25->getLDSScan(&scan);
        
        dist90 = xv25->getDistanceAtAngle(&scan, 90);
        lSpeed = 150.0 + (400.0 - dist90)/100;
        rSpeed = 150.0 - (400.0 - dist90)/100;

        /*
        dist45 = xv25->getDistanceAtAngle(45.0);
        if (dist45 < 400.0) {
            lSpeed += 400 - dist45;
            rSpeed -= 400 - dist45;
        }
        */

        cerr << "dist = " << dist90 << " => cmd motor = [" << lSpeed << ", " << rSpeed << "]" << endl;
        // xv25->setMotor(leftWheel, lSpeed, 10000);
        // xv25->setMotor(rightWheel, rSpeed, 10000);

        usleep(10000);
        t++;
    }

    xv25->stopLDS();

    FILE *fp = fopen("scan.csv", "w");
    vector<point_t> points;
    point_t p;
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
    vector<line_t> lines = pl->ropeAlgorithm(points);
    for (uint32_t i = 0; i < lines.size(); i++)
        cerr << "Line " << i << " = " << lines[i].a << "*x + " << lines[i].b << "*y + " << lines[i].c << endl;

    xv25->setTestMode(testModeOff);

    xv25->disconnect();

    cerr << "End of program" << endl;
    
    return 0;
}
