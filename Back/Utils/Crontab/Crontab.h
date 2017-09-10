#ifndef _CRONTAB_H_
#define _CRONTAB_H_

#include <string>
using std::string;

class Crontab {
public:
        Crontab();
        ~Crontab();
public:
        bool Find(const string& cmd);
        int  Add(const string& cmd);
        int  Del(const string& cmd);
};

#endif //_CRONTAB_H_
