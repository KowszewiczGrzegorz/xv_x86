
#include "hash.hh"

uint32_t hash(const char* cmd)
{
    return HASH(cmd);
}

void getFunctionHashes() 
{
    cerr << "GetVersion : \"" << hash("GetVersion") << "\"" << endl;
    cerr << "GetBatteryLevel : \"" << hash("GetBatteryLevel") << "\"" << endl;

    cerr << "SetTestMode : \"" << hash("SetTestMode") << "\"" << endl;
    cerr << "SetMotors : \"" << hash("SetMotors") << "\"" << endl;
    cerr << "GetPositions : \"" << hash("GetPositions") << "\"" << endl;

    cerr << "StartLDS : \"" << hash("StartLDS") << "\"" << endl;
    cerr << "StopLDS : \"" << hash("StopLDS") << "\"" << endl;
}
