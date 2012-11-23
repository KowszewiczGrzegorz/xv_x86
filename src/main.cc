#include "xv25.hh"

static const string portName = "/dev/ttyACM0";

int main (void)
{
    XV25 *xv25 = new XV25(portName);
    string version;

    if (STATUS_ERROR == xv25->connect()) {
        cerr << "Failed to connect to \"" << portName << "\"" << endl;
	return -1;
    }

    /*
    xv25->getVersion(&version);
    cout << "XV25 version " << endl;
    cout << "---------------------------------------" << endl;
    cout << version << endl;
    cout << "---------------------------------------" << endl;
    */

    xv25->setTestMode(testModeOn);

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

    ldsScan_t scan;
    xv25->startLDS();
    sleep(1);
    xv25->getLDSScan(&scan);
    xv25->stopLDS();

    xv25->disconnect();

    cerr << "End of program" << endl;
    
    return 0;
}
