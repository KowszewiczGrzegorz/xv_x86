
#include "hash.h"

uint32_t my_hash(const char* cmd)
{
    return HASH(cmd);
}

void getFunctionHashes() 
{
    cerr << "GetVersion : \"" << my_hash("GetVersion") << "\"" << endl;
    cerr << "GetBatteryLevel : \"" << my_hash("GetBatteryLevel") << "\"" << endl;

    cerr << "SetTestMode : \"" << my_hash("SetTestMode") << "\"" << endl;
    cerr << "SetMotors : \"" << my_hash("SetMotors") << "\"" << endl;
    cerr << "GetPositions : \"" << my_hash("GetPositions") << "\"" << endl;

    cerr << "StartLDS : \"" << my_hash("StartLDS") << "\"" << endl;
    cerr << "StopLDS : \"" << my_hash("StopLDS") << "\"" << endl;
}
