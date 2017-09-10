#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <set>
#include <string>

#include "Filter.h"

using std::set;
using std::string;

class Scanner {
public:
        Scanner(const struct _FilterArg &config);
        ~Scanner();

public:
        int Scan(const string &scanPath, set<string> &fileSet);

private:
        struct _FilterArg m_Config;
};

#endif //_SCANNER_H_
