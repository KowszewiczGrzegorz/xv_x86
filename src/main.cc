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
    xv25->GetVersion(&version);
    cout << "XV25 version " << endl;
    cout << "---------------------------------------" << endl;
    cout << version << endl;
    cout << "---------------------------------------" << endl;
    */

    xv25->setTestMode(testModeOn);
    sleep(1);

    xv25->setMotor(leftWheel, 300, 1000);
    sleep(5);

    xv25->setTestMode(testModeOff);
    sleep(1);

    xv25->disconnect();

    cerr << "End of program" << endl;
    
    return 0;
}
