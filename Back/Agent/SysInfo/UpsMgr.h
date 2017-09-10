#ifndef _UPS_MGR_H_
#define _UPS_MGR_H_

#include <string>
using std::string;

class UpsMgr {
public:
        UpsMgr();
        ~UpsMgr();

public:
        int GetState(int &state);
        int GetTime(int &time);
        int GetPower(int &power);
        int SetState(int state);
        int SetTime(int time);
        int SetPower(int power);
};

#endif //_UPS_MGR_H_
