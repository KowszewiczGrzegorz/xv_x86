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

    xv25->getVersion(&version),
    cerr << "XV25 version : \n--------------------\n" << version << "\n--------------------\n" << endl;

    xv25->disconnect();

    return 0;
}
